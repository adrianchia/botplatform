// RobotServer.cpp : CRobotServer ��ʵ��

#include "stdafx.h"
#include "RobotServer.h"
#include "ManagerBase.h"
#include "RobotSession.h"
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

bool CRobotServer::CheckLoginResp( Json::Value& data, int& status, std::string& connId, std::string& challenge )
{
    if ( data["type"].asString() != "loginresp" )
        return false;

    status    = data["body"]["status"].asInt();
    connId    = data["body"]["connId"].asString();
    challenge = data["body"]["challenge"].asString();
    return true;
}

bool CRobotServer::CheckRedirect( Json::Value& data, std::string& newip )
{
    if ( data["type"].asString() != "redirect" )
        return false;

    if ( !data["body"].empty() )
        newip = (*data["body"].begin()).asString();
    else
        newip = "";
    return true;
}

void CRobotServer::MakeToken( std::string& token, const std::string& challenge, const std::string& strspid, const std::string& connId, const std::string& strsppwd )
{
    std::string total = challenge + strspid + connId + strsppwd;
   
    token = MakeMd5( total );
}

bool CRobotServer::Login( const std::string& strspid, const std::string& strsppwd, long timeout )
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
            if ( !Connect( m_parent->GetIOService(), m_ip, m_port ) )
                break;
        }

        // ͬ������login
        if ( !Send( "", "", "", "login", &bodyLogin, F_DATA, false ) )
            break;

        // ȡ��Ӧ
        Json::Value data;
        if ( !SyncRecv(data) )
            break;

        int status;
        std::string connId, challenge;
        std::string newip;

        // �����Ӧ��Resp
        if ( CheckLoginResp( data, status, connId, challenge ) )
        {
            // ��½�ɹ�
            if ( status == 1 )
            {
                // ��һ�ζ�
                if ( !RecvNext() )
                    break;

                // �������
                ClearFailed();

                // ����io����
                m_parent->StartRun();

                // ע����
                if ( !m_checkToken )
                    m_checkToken = m_parent->RegisterCheck( this, boost::bind(&CRobotServer::CheckNetwork, this, _1) );
                return true;
            }

            // ʧ��
            if ( firstFailed )
            {
                // �״�ʧ����token
                firstFailed = false;
                std::string token;
                MakeToken( token, challenge, strspid, connId, strsppwd );
                bodyLogin["token"] = token;
            }
            else
            {
                // ��ʧ��
                break;
            }
        }
        // ������ض���
        else if ( CheckRedirect(data, newip) )
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

    m_strspid  = UnicToUtf8(spid);
    m_strsppwd = UnicToUtf8(sppwd);
    m_timeout  = timeout;

    if ( !Login( m_strspid, m_strsppwd, m_timeout ) )
        return E_FAIL;
    
    return S_OK;
}

void CRobotServer::UnRegisterCheckToken()
{
    if ( m_checkToken )
    {
        m_parent->UnRegisterCheck(this);
    }
}

STDMETHODIMP CRobotServer::Logout(void)
{
    // TODO: �ڴ����ʵ�ִ���
    UnRegisterCheckToken();
    SendLoginOut();

    // �ر�socket
    //__super::Close();
    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayName(BSTR robotAccount, BSTR displayName)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !Updaterobot( robotAccount, NULL, displayName ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetPersonalMessage(BSTR robotAccount, BSTR personalMessage)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !Updaterobot( robotAccount, NULL, NULL, personalMessage ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayPicture(BSTR robotAccount, BSTR displayPicture)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !Updaterobot( robotAccount, NULL, NULL, NULL, displayPicture ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetDisplayPictureEx(BSTR robotAccount, BSTR displayPicture, BSTR largePicture)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !Updaterobot( robotAccount, NULL, NULL, NULL, displayPicture, largePicture ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::CreateSession(BSTR robot, BSTR user)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !robot || !user )
        return E_INVALIDARG;

    if ( !Send( UnicToUtf8(robot), UnicToUtf8(user), "", "createsession", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::PushMessage(BSTR robot, BSTR user, BSTR message)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !robot || !user || ! message )
        return E_INVALIDARG;

    Json::Value body;

    body = UnicToUtf8(message);

    std::string u8_robot = UnicToUtf8(robot);
    std::string u8_user  = UnicToUtf8(user);

    if ( !Send( u8_robot, u8_user, "", "push", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::RequestContactList(BSTR robot)
{
    // TODO: �ڴ����ʵ�ִ���

    return E_NOTIMPL;
}

STDMETHODIMP CRobotServer::SetScene(BSTR robotAccount, BSTR scene)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !Updaterobot( robotAccount, NULL, NULL, NULL, NULL, NULL, scene ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotServer::SetColorScheme(BSTR robotAccount, LONG colorScheme)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !Updaterobot( robotAccount, NULL, NULL, NULL, NULL, NULL, NULL, &colorScheme ) )
        return E_FAIL;

    return S_OK;
}

bool CRobotServer::RecvNext()
{
    BOOST_STATIC_ASSERT( sizeof(PackHead) == 6 );
    return Recv( sizeof(PackHead) );
}

void CRobotServer::SafeProcessData( CRobotServer* p, const std::string* data )
{
    BEGIN_SAFE

    if ( p )
        p->OnProcessData(data);

    END_SAFE
}

void CRobotServer::ProcessData( HandleType handle, const std::string* data )
{
    SafeProcessData( handle->GetPtr(), data );
}

void CRobotServer::OnProcessData( const std::string* data )
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
    JSonCmdBase* cmd = m_parent->FindJsonCmd( strType );
    if ( !cmd )
        return;

    // ִ��
    cmd->Execute( root, this );

    // ���
    delete data;
}

void CRobotServer::OnGetBody( const void* data, size_t len )
{
    // �ŵ��̶߳����ﴦ��
    ATLASSERT(m_parent && data);
    std::string* tmpData = new std::string( (const char*)data, len );
    m_parent->AddTask( boost::bind(&CRobotServer::ProcessData, m_handleThis, tmpData) );
}

bool CRobotServer::OnRecv( const boost::system::error_code& error, size_t bytes_transferred )
{
    if ( !__super::OnRecv( error, bytes_transferred ) )
        return false;

    if ( bytes_transferred == sizeof(PackHead) )
    {
        const PackHead* packHead = (const PackHead*)m_recvBuf.data();
        
        // ������ȡbody
        return Recv( ntohs( packHead->payloadLength ) );
    }
    else
    {
        // �õ�һ�������İ���
        OnGetBody( m_recvBuf.data(), bytes_transferred );

        // ������һ�ζ�ȡͷ
        return RecvNext();
    }
}

void CRobotServer::Init( CManagerBase* man, BSTR ip, int port )
{
    m_parent = man;
    m_ip     = CW2A(ip);
    m_port   = port;

    __super::Init( man->GetIOService() );
    //m_parent->StartRun();
}

WORD CRobotServer::GetNextSequenceNum()
{
    boost::lock_guard<boost::mutex> guard_(m_seqNumMutex);

    ++m_sequenceNumber;
    return m_sequenceNumber;
}

bool CRobotServer::SendBase( int frameType, const void* data, size_t len, bool asyn )
{
    // head
    PackHead head;
    head.startMarker    = 0x69;
    head.frameType      = frameType;
    head.sequenceNumber = htons( GetNextSequenceNum() );
    head.payloadLength  = htons(len);

    std::string str;
    str.assign( (char*)&head, sizeof(head) );

    // ��������
    if ( data && len > 0 )
        str.append( (const char*)data, len );

    // �첽��ͬ������
    bool ret = false;

    if ( asyn )
        ret = __super::Send( str.c_str(), str.size() );
    else
        ret = __super::SyncSend( str.c_str(), str.size() );

    if ( ret && m_checkToken )
    {
        m_checkToken->Reset();
    }

    return ret;
}

bool CRobotServer::Send( const std::string& robotId, const std::string& userId, const std::string& sessionId, const std::string& type, 
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

    return SendBase( frameType, strmain.c_str(), strmain.size(), asyn );
}

bool CRobotServer::SendKeepAlive()
{
    return SendBase( F_KEEP_ALIVE, NULL, 0, true );
}

void CRobotServer::CheckNetwork( bool timeout )
{
    if ( IsFailed() )
    {
        if ( !Login( m_strspid, m_strsppwd, m_timeout ) )
            return;
    }

    if ( timeout )
        SendKeepAlive();
}

bool CRobotServer::SendLoginOut()
{
    return SendBase( F_SIGNOFF, NULL, 0, false );
}

bool CRobotServer::SyncRecv( std::string& data )
{
    // ͬ�����ܵ�һ��������
    if ( !__super::SyncRecv( sizeof(PackHead) ) )
    {
        return false;
    }

    // ������body����
    const PackHead* packHead = (const PackHead*)m_recvBuf.data();
    WORD bodyLen = ntohs( packHead->payloadLength );
    if ( !__super::SyncRecv( bodyLen ) )
    {
        return false;
    }

    // �õ�һ��������
    data.assign( m_recvBuf.data(), bodyLen );
    return true;
}

bool CRobotServer::SyncRecv( Json::Value& root )
{
    std::string data;
    if ( !SyncRecv( data ) )
        return false;

    Json::Reader reader;
    if ( !reader.parse( data, root ) )
        return false;

    return true;
}

bool CRobotServer::Updaterobot( BSTR robotAccount, LONG* status, BSTR displayName, BSTR personalMessage,
                               BSTR displayPicture, BSTR largePicture, BSTR scene, LONG* colorScheme )
{
    if ( !robotAccount )
        return false;

    std::string strRobotAcc = UnicToUtf8(robotAccount);

    Json::Value body;

    if ( status )
    {
        body["status"] = *status;
    }

    if ( displayName )
    {
        body["displayName"] = UnicToUtf8(displayName);
    }
    
    if ( personalMessage )
    {
        body["personalMessage"] = UnicToUtf8(personalMessage);
    }

    if ( displayPicture )
    {
        body["displayPicture"] = UnicToUtf8(displayPicture);
    }

    if ( largePicture )
    {
        body["largePicture"] = UnicToUtf8(largePicture);
    }

    if ( scene )
    {
        body["scene"] = UnicToUtf8(scene);
    }

    if ( colorScheme )
    {
        body["colorScheme"] = *colorScheme;
    }
    
    if ( body.empty() )
        return false;

    return Send( strRobotAcc, "", "", "updaterobot", &body );
}

void CRobotServer::AddSession( const std::string& sessionId, CRobotSession* session )
{
    if ( !session )
        return;

    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    m_sessionMap.insert( RobotSessionMap::value_type(sessionId, session) );
}

void CRobotServer::RemoveSession( const std::string& sessionId )
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

CRobotSession* CRobotServer::GetSession( const std::string& sessionId )
{
    boost::lock_guard<boost::mutex> guard_(m_sessionMutex);

    RobotSessionMap::iterator it = m_sessionMap.find( sessionId );
    if ( it != m_sessionMap.end() )
    {
        return it->second;
    }

    return NULL;
}

