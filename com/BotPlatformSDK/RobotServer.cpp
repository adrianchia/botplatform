// RobotServer.cpp : CRobotServer ��ʵ��

#include "stdafx.h"
#include "RobotServer.h"
#include "ManagerBase.h"
#include "RobotSession.h"
#include "RobotResource.h"
#include "Misc.h"


// CRobotServer

// 6�ֽ�ͷ
struct PackHead
{
    BYTE startMarker;
    BYTE frameType;
    WORD sequenceNumber;
    WORD payloadLength;
};

void CRobotServer::FinalRelease()
{
    // �ǳ�
    Logout();

    // �ͷ�����session
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

bool CRobotServer::login( const std::string& strspid, const std::string& strsppwd, long timeout )
{
    bool wantConnect = true;
    bool firstFailed = true;

    Json::Value bodyLogin;

    bodyLogin["spid"]    = strspid;
    bodyLogin["version"] = "cpp-1.0";
    bodyLogin["token"]   = "";
    bodyLogin["state"]   = 0;

    while ( true )
    {
        // ����
        if ( wantConnect )
        {
            wantConnect = false;
            if ( !connect( m_parent->getIOService(), m_ip, m_port ) )
                break;
        }

        // ͬ������login
        if ( !sendCmd( "", "", "", "login", &bodyLogin, F_DATA, false ) )
            break;

        // ȡ��Ӧ
        Json::Value data;
        if ( !syncRecv(data) )
            break;

        int status;
        std::string connId, challenge;
        std::string newip;

        // �����Ӧ��Resp
        if ( checkLoginResp( data, status, connId, challenge ) )
        {
            // ��½�ɹ�
            if ( status == 1 )
            {
                // ��һ�ζ�
                if ( !recvNext() )
                    break;

                // �������
                clearFailed();

                // ����io����
                m_parent->startRun();

                // ע����
                if ( !m_checkToken )
                    m_checkToken = m_parent->registerCheck( this, boost::bind(&CRobotServer::checkNetwork, this, _1) );
                return true;
            }

            // ʧ��
            if ( firstFailed )
            {
                // �״�ʧ����token
                firstFailed = false;
                std::string token;
                makeToken( token, challenge, strspid, connId, strsppwd );
                bodyLogin["token"] = token;
            }
            else
            {
                // ��ʧ��
                break;
            }
        }
        // ������ض���
        else if ( checkRedirect(data, newip) )
        {
            // Ҫ����������
            wantConnect = true;
            m_ip = newip;
            bodyLogin["state"] = 1;
        }
        else
        {
            break;
        }
    }

    return false;
}

STDMETHODIMP CRobotServer::Login(BSTR spid, BSTR sppwd, LONG timeout)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !spid || !sppwd )
        return E_INVALIDARG;

    m_strspid  = unicToUtf8(spid);
    m_strsppwd = unicToUtf8(sppwd);
    m_timeout  = timeout;

    if ( !login( m_strspid, m_strsppwd, m_timeout ) )
        return E_FAIL;
    
    return S_OK;
}

void CRobotServer::unRegisterCheckToken()
{
    if ( m_checkToken )
    {
        m_parent->unRegisterCheck(this);
    }
}

STDMETHODIMP CRobotServer::Logout(void)
{
    // TODO: �ڴ����ʵ�ִ���
    unRegisterCheckToken();
    sendLoginOut();

    // �ر�socket
    //__super::Close();
    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayName(BSTR robotAccount, BSTR displayName)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !updaterobot( robotAccount, NULL, displayName ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetPersonalMessage(BSTR robotAccount, BSTR personalMessage)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !updaterobot( robotAccount, NULL, NULL, personalMessage ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayPicture(BSTR robotAccount, BSTR displayPicture)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, displayPicture ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayPictureEx(BSTR robotAccount, BSTR displayPicture, BSTR largePicture)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, displayPicture, largePicture ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::CreateSession(BSTR robot, BSTR user)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !robot || !user )
        return E_INVALIDARG;

    if ( !sendCmd( unicToUtf8(robot), unicToUtf8(user), "", "createsession", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::PushMessage(BSTR robot, BSTR user, BSTR message)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !robot || !user || ! message )
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
    if ( !robot )
        return E_INVALIDARG;

    std::string u8_robot = unicToUtf8(robot);

    if ( !sendCmd( u8_robot, "", "", "getuserlist", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::RequestResource(BSTR robot, BSTR user, IRobotResource* resource, BSTR saveUrl)
{
    if ( !robot || !user || !resource || !saveUrl )
        return E_INVALIDARG;

    std::string u8_robot = unicToUtf8(robot);
    std::string u8_user  = unicToUtf8(user);

    CRobotResource* realRes = static_cast<CRobotResource*>(resource);

    Json::Value body;

    body["name"]    = realRes->getName();
    body["digest"]  = realRes->getDigest();
    body["size"]    = numToStr(realRes->getSize());
    
    if ( !sendCmd( u8_robot, u8_user, "", "getresource", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetScene(BSTR robotAccount, BSTR scene)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !updaterobot( robotAccount, NULL, NULL, NULL, NULL, NULL, scene ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetColorScheme(BSTR robotAccount, LONG colorScheme)
{
    // TODO: �ڴ����ʵ�ִ���
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
    safeProcessData( handle->GetPtr(), data );
}

void CRobotServer::onProcessData( const std::string* data )
{
    ATLASSERT(m_parent);

    // ����data
    Json::Value  root;
    Json::Reader reader;

    if ( !reader.parse( *data, root ) )
        return;

    // �õ�����
    std::string strType = root["type"].asString();
    
    // ������
    JSonCmdBase* cmd = m_parent->findJsonCmd( strType );
    if ( !cmd )
        return;

    // ִ��
    cmd->execute( root, this );

    // ���
    delete data;
}

void CRobotServer::onGetBody( const void* data, size_t len )
{
    // �ŵ��̶߳����ﴦ��
    ATLASSERT(m_parent && data);
    std::string* tmpData = new std::string( (const char*)data, len );
    m_parent->addTask( boost::bind(&CRobotServer::processData, m_handleThis, tmpData) );
}

bool CRobotServer::onRecv( const boost::system::error_code& error, size_t bytes_transferred )
{
    if ( !__super::onRecv( error, bytes_transferred ) )
        return false;

    if ( bytes_transferred == sizeof(PackHead) )
    {
        const PackHead* packHead = (const PackHead*)m_recvBuf.data();
        
        // ������ȡbody
        return recv( ntohs( packHead->payloadLength ) );
    }
    else
    {
        // �õ�һ�������İ���
        onGetBody( m_recvBuf.data(), bytes_transferred );

        // ������һ�ζ�ȡͷ
        return recvNext();
    }
}

void CRobotServer::init( ManagerBase* man, BSTR ip, int port )
{
    m_parent = man;
    m_ip     = CW2A(ip);
    m_port   = port;

    __super::init( man->getIOService() );
    //m_parent->startRun();
}

WORD CRobotServer::getNextSequenceNum()
{
    boost::lock_guard<boost::mutex> guard_(m_seqNumMutex);

    ++m_sequenceNumber;
    return m_sequenceNumber;
}

bool CRobotServer::sendCmdBase( int frameType, const void* data, size_t len, bool asyn )
{
    // head
    PackHead head;
    head.startMarker    = 0x69;
    head.frameType      = frameType;
    head.sequenceNumber = htons( getNextSequenceNum() );
    head.payloadLength  = htons(len);

    std::string str;
    str.assign( (char*)&head, sizeof(head) );

    // ��������
    if ( data && len > 0 )
        str.append( (const char*)data, len );

    // �첽��ͬ������
    bool ret = false;

    if ( asyn )
        ret = __super::send( str.c_str(), str.size() );
    else
        ret = __super::syncSend( str.c_str(), str.size() );

    if ( ret && m_checkToken )
    {
        m_checkToken->reset();
    }

    return ret;
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

void CRobotServer::checkNetwork( bool timeout )
{
    if ( isFailed() )
    {
        if ( !login( m_strspid, m_strsppwd, m_timeout ) )
            return;
    }

    if ( timeout )
        sendKeepAlive();
}

bool CRobotServer::sendLoginOut()
{
    return sendCmdBase( F_SIGNOFF, NULL, 0, false );
}

bool CRobotServer::syncRecv( std::string& data )
{
    // ͬ�����ܵ�һ��������
    if ( !__super::syncRecv( sizeof(PackHead) ) )
    {
        return false;
    }

    // ������body����
    const PackHead* packHead = (const PackHead*)m_recvBuf.data();
    WORD bodyLen = ntohs( packHead->payloadLength );
    if ( !__super::syncRecv( bodyLen ) )
    {
        return false;
    }

    // �õ�һ��������
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
    if ( !robotAccount )
        return false;

    std::string strRobotAcc = unicToUtf8(robotAccount);

    Json::Value body;

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

    return sendCmd( strRobotAcc, "", "", "updaterobot", &body );
}

void CRobotServer::addSession( const std::string& sessionId, CRobotSession* session )
{
    if ( !session )
        return;

    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    m_sessionMap.insert( RobotSessionMap::value_type(sessionId, session) );
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

