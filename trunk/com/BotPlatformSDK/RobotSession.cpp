#include "stdafx.h"
#include "RobotSession.h"
#include "RobotUsers.h"
#include "RobotServer.h"
#include "RobotUser.h"
#include "RobotMessage.h"
#include "Misc.h"

// CRobotSession

CRobotSession::CRobotSession() : m_server(NULL), m_robotUsers(NULL), m_mode(0), m_closed(false), m_active(false)
{
}

HRESULT CRobotSession::FinalConstruct()
{
    IRobotUsers* users = 0;
    return CreateRealObject( &users, &m_robotUsers );
}

void CRobotSession::FinalRelease()
{
    Close();
    SafeRelease(m_robotUsers);
}

STDMETHODIMP CRobotSession::CreateMessage(IRobotMessage** message)
{
    return CreateRealObject<CRobotMessage>( message );
}

STDMETHODIMP CRobotSession::SendText(BSTR message)
{
    if ( !message )
        return E_INVALIDARG;

    Json::Value body;

    body["text"] = UnicToUtf8(message);

    if ( !sendCmd( "msg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendIM(IRobotMessage* message)
{
    if ( !message )
        return E_INVALIDARG;

    Json::Value body;

    CRobotMessage* msg = static_cast<CRobotMessage*>(message);

    if ( !msg->getText().empty() )
        body["text"] = msg->getText();

    if ( !msg->getFontName().empty() )
        body["fontName"] = msg->getFontName();
    
    if ( msg->hasFontColor() )
        body["fontColor"] = msg->getFontColor();
    
    if ( msg->getFontStyle() != 0 )
        body["fontStyle"] = msg->getFontStyle();

    if ( !msg->getSignature().empty() )
        body["signature"] = msg->getSignature();

    for ( CRobotMessage::StringMap::const_iterator it = msg->getEmoticons().begin(); it != msg->getEmoticons().end(); ++it )
    {
        body["emoticons"][it->first] = it->second;
    }

    if ( !sendCmd( "msg", &body ) )
        return E_FAIL;

    return S_OK;
}

bool CRobotSession::sendCmd( const std::string& userId, const std::string& type, Json::Value* body )
{
    if ( !m_server )
        return false;

    return m_server->Send( m_robotId, userId, m_sessionId, type, body );
}

bool CRobotSession::sendCmd( const std::string& type, Json::Value* body )
{
    return sendCmd( m_userId, type, body );
}

STDMETHODIMP CRobotSession::SendNudge(void)
{
    if ( !sendCmd( "nudge", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendActivity(BSTR url, BSTR friendlyName)
{
    if ( !url || !friendlyName )
        return E_INVALIDARG;

    Json::Value body;

    body["name"] = UnicToUtf8(friendlyName);
    body["data"] = UnicToUtf8(url);

    if ( !sendCmd( "appmsg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendActivityEx(BSTR appid, BSTR appname, BSTR data)
{
    if ( !appid || !appname || !data )
        return E_INVALIDARG;

    Json::Value body;

    body["id"]   = UnicToUtf8(appid);
    body["name"] = UnicToUtf8(appname);
    body["data"] = UnicToUtf8(data);

    if ( !sendCmd( "appmsg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::get_ActivityOpened(VARIANT_BOOL* pVal)
{
    IMPL_GET_LONG(pVal, m_active)
}

STDMETHODIMP CRobotSession::SendTyping(void)
{
    if ( !sendCmd( "typing", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::GetUser(BSTR userid, IRobotUser** ppUser)
{
    if ( !userid || !ppUser )
        return E_INVALIDARG;

    *ppUser = NULL;

    CRobotUser* user = m_robotUsers->getUser( UnicToUtf8(userid) );
    if ( !user )
        return E_INVALIDARG;

    user->AddRef();
    *ppUser = user;
    return S_OK;
}

STDMETHODIMP CRobotSession::get_Users(IRobotUsers** ppUsers)
{
    if ( !ppUsers )
        return E_INVALIDARG;

    m_robotUsers->AddRef();
    *ppUsers = m_robotUsers;
    return S_OK;
}

STDMETHODIMP CRobotSession::get_Robot(BSTR* pStr)
{
    IMPL_GET_BSTR(pStr, m_robotId)
}

STDMETHODIMP CRobotSession::get_OpenMode(LONG* pVal)
{
    IMPL_GET_LONG(pVal, m_mode)
}

STDMETHODIMP CRobotSession::get_Closed(VARIANT_BOOL* pVal)
{
    IMPL_GET_LONG(pVal, m_closed)
}

STDMETHODIMP CRobotSession::Close(void)
{
    if ( m_closed )
        return S_OK;

    m_closed = true;

    if ( !sendCmd( "closesession", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::InviteUser(BSTR user)
{
    if ( !user )
        return E_INVALIDARG;

    if ( !sendCmd( UnicToUtf8(user), "invite", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendFile(BSTR uri, BSTR friendlyName)
{
    if ( !uri || !friendlyName )
        return E_INVALIDARG;

    Json::Value body;

    body["location"] = UnicToUtf8(uri);

    if ( friendlyName && *friendlyName != 0 )
        body["name"] = UnicToUtf8(friendlyName);

    if ( !sendCmd( "file", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendFileAcceptance(BSTR transferId, BSTR saveUrl)
{
    if ( !transferId || !saveUrl )
        return E_INVALIDARG;

    Json::Value body;

    body["cmd"]         = "accept";
    body["transferId"]  = UnicToUtf8(transferId);
    body["saveUrl"]     = UnicToUtf8(saveUrl);

    if ( !sendCmd( "filecmd", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendFileRejection(BSTR transferId)
{
    if ( !transferId )
        return E_INVALIDARG;

    Json::Value body;

    body["cmd"]         = "reject";
    body["transferId"]  = UnicToUtf8(transferId);

    if ( !sendCmd( "filecmd", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendFileCancellation(BSTR transferId)
{
    if ( !transferId )
        return E_INVALIDARG;

    Json::Value body;

    body["cmd"]         = "cancel";
    body["transferId"]  = UnicToUtf8(transferId);

    if ( !sendCmd( "filecmd", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendInk(BSTR inkData)
{
    if ( !inkData )
        return E_INVALIDARG;

    Json::Value body;

    body = UnicToUtf8(inkData);

    if ( !sendCmd( "inkmsg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendWink(BSTR uri, BSTR stamp)
{
    if ( !uri || !stamp )
        return E_INVALIDARG;

    Json::Value body;

    body["location"] = UnicToUtf8(uri);
    body["stamp"]    = UnicToUtf8(stamp);

    if ( !sendCmd( "wink", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendVoiceclip(BSTR uri)
{
    if ( !uri )
        return E_INVALIDARG;

    Json::Value body;

    body["location"] = UnicToUtf8(uri);

    if ( !sendCmd( "voiceclip", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendWebcam(BSTR serverIP, LONG serverPort, LONG recipientid, LONG sessionid)
{
    if ( !serverIP )
        return E_INVALIDARG;

    Json::Value body;

    body["host"] = UnicToUtf8(serverIP);
    body["port"] = serverPort;
    body["rid"]  = recipientid;
    body["wid"]  = sessionid;

    if ( !sendCmd( "webcam", &body ) )
        return E_FAIL;

    return S_OK;
}

void CRobotSession::init( CRobotServer* server, const std::string& robotId, const std::string& userId, const std::string& sessionId, int openMode ) 
{
    m_server   = server;
    m_robotId   = robotId;
    m_sessionId = sessionId;
    m_userId    = userId;
    m_mode      = openMode;
}

