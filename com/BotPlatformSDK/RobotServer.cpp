#include "stdafx.h"
#include "RobotServer.h"
#include "ManagerBase.h"
#include "RobotSession.h"
#include "RobotResource.h"
#include "Misc.h"


// CRobotServer

// 6 bytes pack head
struct PackHead
{
    BYTE startMarker;
    BYTE frameType;
    WORD sequenceNumber;
    WORD payloadLength;
};

STDMETHODIMP CRobotServer::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_IRobotServer
    };

    for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}

void CRobotServer::FinalRelease()
{
    Logout();

    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    for ( RobotSessionMap::iterator it = m_sessionMap.begin(); it != m_sessionMap.end(); ++it )
    {
        CRobotSession* p = it->second;
        if ( p )
            p->Release();
    }

    m_sessionMap.clear();
}

bool CRobotServer::checkLoginResp( Json::Value& data, int& status, std::string& connId, std::string& challenge )
{
    if ( data["type"].asString() != "loginresp" )
        return false;

    status    = data["body"]["status"].asInt();
    connId    = data["body"]["connId"].asString();
    challenge = data["body"]["challenge"].asString();
    return true;
}

bool CRobotServer::checkRedirect( Json::Value& data, std::string& newip )
{
    if ( data["type"].asString() != "redirect" )
        return false;

    if ( !data["body"].empty() )
        newip = (*data["body"].begin()).asString();
    else
        newip = "";
    return true;
}

void CRobotServer::makeToken( std::string& token, const std::string& challenge, const std::string& strspid, const std::string& connId, const std::string& strsppwd )
{
    std::string total = challenge + strspid + connId + strsppwd;
   
    token = makeMd5( total );
}

bool CRobotServer::login( const std::string& strspid, const std::string& strsppwd, long timeout, std::wstring* errInfo )
{
#define SET_ERROR_INFO(s) if ( errInfo ) *errInfo = s;

    if ( strspid.empty() )
    {
        SET_ERROR_INFO(L"invalid spid");
        return false;
    }

    bool wantConnect = true;
    bool firstFailed = true;

    Json::Value bodyLogin;

    bodyLogin["spid"]    = strspid;
    bodyLogin["version"] = "cpp-1.0";
    bodyLogin["token"]   = "";
    bodyLogin["state"]   = 0;

    while ( true )
    {
        // connect
        if ( wantConnect )
        {
            wantConnect = false;
            if ( !connect( m_serverMan->getIOService(), m_ip, m_port ) )
            {
                SET_ERROR_INFO(L"connect failed");
                break;
            }
        }

        // login
        if ( !sendCmd( "", "", "", "login", &bodyLogin, F_DATA, false ) )
        {
            SET_ERROR_INFO(L"request failed");
            break;
        }

        // receive reply
        Json::Value data;
        if ( !syncRecv(data) )
        {
            SET_ERROR_INFO(L"server disconnected, invalid spid or password");
            break;
        }

        int status;
        std::string connId, challenge;
        std::string newip;

        // if cmd is loginresp
        if ( checkLoginResp( data, status, connId, challenge ) )
        {
            // login ok
            if ( status == 1 )
            {
                // receive data first
                if ( !recvNext() )
                {
                    SET_ERROR_INFO(L"receive failed");
                    break;
                }

                // clear fail flag
                clearFailed();

                // start io ser
                m_serverMan->startRun();

                // register my check routine
                registerCheckToken();
                return true;
            }

            // login failed
            if ( firstFailed )
            {
                // if first failed get token 
                firstFailed = false;
                std::string token;
                makeToken( token, challenge, strspid, connId, strsppwd );
                bodyLogin["token"] = token;
            }
            else
            {
                // failed again
                SET_ERROR_INFO(L"server disconnected, invalid spid or password");
                break;
            }
        }
        else if ( checkRedirect(data, newip) ) // if need redirect
        {
            // connect again
            wantConnect = true;
            m_ip = newip;
            bodyLogin["state"] = 1;
        }
        else // unkown
        {
            SET_ERROR_INFO(L"unkown error");
            break;
        }
    }

    return false;
}

STDMETHODIMP CRobotServer::Login(BSTR spid, BSTR sppwd, LONG timeout)
{
    if ( !isValidStr(spid) || !sppwd )
        return E_INVALIDARG;

    m_strspid = unicToUtf8(spid);

    if ( sppwd )
        m_strsppwd = unicToUtf8(sppwd);
    else
        m_strsppwd.clear();

    m_timeout = timeout;

    std::wstring errInfo;
    if ( !login( m_strspid, m_strsppwd, m_timeout, &errInfo ) )
        return Error( errInfo.c_str() );
    
    return S_OK;
}

STDMETHODIMP CRobotServer::Logout(void)
{
    unRegisterCheckToken();
    sendLoginOut();
    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayName(BSTR robotAccount, BSTR displayName)
{
    if ( !updaterobot( robotAccount, NULL, displayName ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetPersonalMessage(BSTR robotAccount, BSTR personalMessage)
{
    if ( !updaterobot( robotAccount, NULL, NULL, personalMessage ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayPicture(BSTR robotAccount, BSTR displayPicture)
{
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, displayPicture ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayPictureEx(BSTR robotAccount, BSTR displayPicture, BSTR largePicture)
{
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, displayPicture, largePicture ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::CreateSession(BSTR robot, BSTR user)
{
    if ( !isValidStr(robot) || !isValidStr(user) )
        return E_INVALIDARG;

    if ( !sendCmd( unicToUtf8(robot), unicToUtf8(user), "", "createsession", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::PushMessage(BSTR robot, BSTR user, BSTR message)
{
    if ( !isValidStr(robot) || !isValidStr(user) || !isValidStr(message) )
        return E_INVALIDARG;

    Json::Value body;

    body = unicToUtf8(message);

    std::string u8_robot = unicToUtf8(robot);
    std::string u8_user  = unicToUtf8(user);

    if ( !sendCmd( u8_robot, u8_user, "", "push", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::RequestContactList(BSTR robot)
{
    if ( !isValidStr(robot) )
        return E_INVALIDARG;

    std::string u8_robot = unicToUtf8(robot);

    if ( !sendCmd( u8_robot, "", "", "getuserlist", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::RequestResource(BSTR robot, BSTR user, IRobotResource* resource, BSTR saveUrl)
{
    if ( !isValidStr(robot) || !isValidStr(user) || !resource )
        return E_INVALIDARG;

    std::string u8_robot = unicToUtf8(robot);
    std::string u8_user  = unicToUtf8(user);

    CRobotResource* realRes = static_cast<CRobotResource*>(resource);

    Json::Value body;

    body["name"]    = realRes->getName();
    body["digest"]  = realRes->getDigest();
    body["size"]    = numToStr(realRes->getSize());

    if ( isValidStr(saveUrl) )
        body["saveUrl"] = unicToUtf8(saveUrl);

    if ( !sendCmd( u8_robot, u8_user, "", "getresource", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetScene(BSTR robotAccount, BSTR scene)
{
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, NULL, NULL, scene ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetColorScheme(BSTR robotAccount, LONG colorScheme)
{
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, NULL, NULL, NULL, &colorScheme ) )
        return E_FAIL;

    return S_OK;
}

bool CRobotServer::recvNext()
{
    BOOST_STATIC_ASSERT( sizeof(PackHead) == 6 );
    return recv( sizeof(PackHead) );
}

void CRobotServer::safeProcessData( CRobotServer* p, const std::string* data )
{
    BEGIN_SAFE

    if ( p )
        p->onProcessData(data);

    END_SAFE
}

void CRobotServer::processData( HandleType handle, const std::string* data )
{
    safeProcessData( handle->getPtr(), data );
}

void CRobotServer::onProcessData( const std::string* data )
{
    ATLASSERT(m_serverMan);

    // parse data
    Json::Value  root;
    Json::Reader reader;

    if ( !reader.parse( *data, root ) )
        return;

    // get cmd type
    std::string strType = root["type"].asString();
    
    // find cmd
    JSonCmdBase* cmd = m_serverMan->findJsonCmd( strType );
    if ( !cmd )
        return;

    // execute cmd
    cmd->execute( root, this );

    // free data
    delete data;
}

void CRobotServer::onGetBody( const void* data, size_t len )
{
    // push data in work thread pool
    ATLASSERT(m_serverMan && data);
    std::string* tmpData = new std::string( (const char*)data, len );
    m_serverMan->addTask( boost::bind(&CRobotServer::processData, m_handleThis, tmpData) );
}

bool CRobotServer::onRecv( const boost::system::error_code& error, size_t bytes_transferred )
{
    if ( !__super::onRecv( error, bytes_transferred ) )
        return false;

    // if get pack head
    if ( bytes_transferred == sizeof(PackHead) )
    {
        const PackHead* packHead = (const PackHead*)m_recvBuf.data();
        
        // continue receive body data
        return recv( ntohs( packHead->payloadLength ) );
    }
    else // a full pack received
    {
        onGetBody( m_recvBuf.data(), bytes_transferred );

        // receive next
        return recvNext();
    }
}

void CRobotServer::init( ManagerBase* man, BSTR ip, int port )
{
    ATLASSERT(ip);

    m_ip     = CW2A(ip);
    m_port   = port;

    __super::init( man );
}

WORD CRobotServer::getNextSequenceNum()
{
    boost::lock_guard<boost::mutex> guard_(m_seqNumMutex);

    ++m_sequenceNumber;
    return m_sequenceNumber;
}

bool CRobotServer::sendCmdBase( int frameType, const void* data, size_t len, bool asyn )
{
    // pack head
    PackHead head;
    head.startMarker    = 0x69;
    head.frameType      = frameType;
    head.sequenceNumber = htons( getNextSequenceNum() );
    head.payloadLength  = htons(len);

    std::string str;
    str.assign( (char*)&head, sizeof(head) );

    // body data
    if ( data && len > 0 )
        str.append( (const char*)data, len );

    // send by sync or asyn mode
    if ( asyn )
        return __super::send( str.c_str(), str.size() );
    else
        return __super::syncSend( str.c_str(), str.size() );
}

bool CRobotServer::sendCmd( const std::string& robotId, const std::string& userId, const std::string& sessionId, const std::string& type, 
                        Json::Value* body, int frameType /*= F_DATA*/, bool asyn /*= true*/ )
{
    Json::Value val;

    if ( !robotId.empty() )
        val["robotId"] = robotId;

    if ( !userId.empty() )
        val["userId"] = userId;

    if ( !sessionId.empty() )
        val["sessionId"] = sessionId;

    if ( !type.empty() )
        val["type"] = type;

    if ( body )
        val["body"] = *body;

    Json::FastWriter writer;
    std::string strmain = writer.write( val );

    return sendCmdBase( frameType, strmain.c_str(), strmain.size(), asyn );
}

bool CRobotServer::sendKeepAlive()
{
    return sendCmdBase( F_KEEP_ALIVE, NULL, 0, true );
}

void CRobotServer::onCheckNetwork( bool needKeepAlive )
{
    if ( isFailed() )
    {
        if ( !login( m_strspid, m_strsppwd, m_timeout ) )
            return;
    }

    if ( needKeepAlive )
        sendKeepAlive();
}

bool CRobotServer::sendLoginOut()
{
    return sendCmdBase( F_SIGNOFF, NULL, 0, false );
}

bool CRobotServer::syncRecv( std::string& data )
{
    // receive head
    if ( !__super::syncRecv( sizeof(PackHead) ) )
    {
        return false;
    }

    // receive body
    const PackHead* packHead = (const PackHead*)m_recvBuf.data();
    WORD bodyLen = ntohs( packHead->payloadLength );
    if ( !__super::syncRecv( bodyLen ) )
    {
        return false;
    }

    // get full pack
    data.assign( m_recvBuf.data(), bodyLen );
    return true;
}

bool CRobotServer::syncRecv( Json::Value& root )
{
    std::string data;
    if ( !syncRecv( data ) )
        return false;

    Json::Reader reader;
    if ( !reader.parse( data, root ) )
        return false;

    return true;
}

bool CRobotServer::updaterobot( BSTR robotAccount, LONG* status, BSTR displayName, BSTR personalMessage,
                               BSTR displayPicture, BSTR largePicture, BSTR scene, LONG* colorScheme )
{
    std::string u8_robotAcc;
    Json::Value body;

    if ( robotAccount )
    {
        u8_robotAcc = unicToUtf8(robotAccount);
    }

    if ( status )
    {
        body["status"] = *status;
    }

    if ( displayName )
    {
        body["displayName"] = unicToUtf8(displayName);
    }
    
    if ( personalMessage )
    {
        body["personalMessage"] = unicToUtf8(personalMessage);
    }

    if ( displayPicture )
    {
        body["displayPicture"] = unicToUtf8(displayPicture);
    }

    if ( largePicture )
    {
        body["largePicture"] = unicToUtf8(largePicture);
    }

    if ( scene )
    {
        body["scene"] = unicToUtf8(scene);
    }

    if ( colorScheme )
    {
        body["colorScheme"] = *colorScheme;
    }
    
    if ( body.empty() )
        return false;

    return sendCmd( u8_robotAcc, "", "", "updaterobot", &body );
}

bool CRobotServer::addSession( const std::string& sessionId, CRobotSession* session )
{
    if ( !session )
        return false;

    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    return m_sessionMap.insert( RobotSessionMap::value_type(sessionId, session) ).second;
}

void CRobotServer::removeSession( const std::string& sessionId )
{
    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    RobotSessionMap::iterator it = m_sessionMap.find( sessionId );
    if ( it != m_sessionMap.end() )
    {
        CRobotSession* session = it->second;
        m_sessionMap.erase(it);

        if ( session )
            session->Release();
    }
}

CRobotSession* CRobotServer::getSession( const std::string& sessionId )
{
    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    RobotSessionMap::iterator it = m_sessionMap.find( sessionId );
    if ( it != m_sessionMap.end() )
    {
        return it->second;
    }

    return NULL;
}

