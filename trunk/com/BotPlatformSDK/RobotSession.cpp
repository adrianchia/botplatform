// RobotSession.cpp : CRobotSession 的实现

#include "stdafx.h"
#include "RobotSession.h"
#include "RobotUsers.h"
#include "RobotServer.h"
#include "RobotUser.h"
#include "RobotMessage.h"
#include "Misc.h"

// CRobotSession

CRobotSession::CRobotSession() : m_pParent(0), m_pRobotUsers(0), m_mode(0), m_closed(false), m_active(false)
{
}

HRESULT CRobotSession::FinalConstruct()
{
    IRobotUsers* users = 0;
    return CreateRealObject( &users, &m_pRobotUsers );
}

void CRobotSession::FinalRelease()
{
    Close();

    if ( m_pRobotUsers )
        m_pRobotUsers->Release();
}

STDMETHODIMP CRobotSession::CreateMessage(IRobotMessage** message)
{
    return CreateRealObject<CRobotMessage>( message );
}

STDMETHODIMP CRobotSession::SendText(BSTR message)
{
    // TODO: 在此添加实现代码
    if ( !message )
        return E_INVALIDARG;

    Json::Value body;

    body["text"] = UnicToUtf8(message);

    if ( !Send( "msg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendIM(IRobotMessage* message)
{
    // TODO: 在此添加实现代码
    if ( !message )
        return E_INVALIDARG;

    Json::Value body;

    CRobotMessage* msg = static_cast<CRobotMessage*>(message);

    if ( !msg->m_text.empty() )
        body["text"] = msg->m_text;

    if ( !msg->m_fontName.empty() )
        body["fontName"] = msg->m_fontName;
    
    if ( msg->m_fontColor )
        body["fontColor"] = *(msg->m_fontColor);
    
    if ( msg->m_fontStyle > 0 )
        body["fontStyle"] = msg->m_fontStyle;

    if ( !msg->m_signature.empty() )
        body["signature"] = msg->m_signature;

    for ( CRobotMessage::StringMap::iterator it = msg->m_emoticons.begin(); it != msg->m_emoticons.end(); ++it )
    {
        body["emoticons"][it->first] = it->second;
    }

    if ( !Send( "msg", &body ) )
        return E_FAIL;

    return S_OK;
}

bool CRobotSession::SendBase( const std::string& userId, const std::string& type, Json::Value* body )
{
    if ( !m_pParent )
        return false;

    return m_pParent->Send( m_robotId, userId, m_sessionId, type, body );
}

bool CRobotSession::Send( const std::string& type, Json::Value* body )
{
    return SendBase( m_userId, type, body );
}

STDMETHODIMP CRobotSession::SendNudge(void)
{
    // TODO: 在此添加实现代码
    if ( !Send( "nudge", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendActivity(BSTR url, BSTR friendlyName)
{
    // TODO: 在此添加实现代码
    if ( !url || !friendlyName )
        return E_INVALIDARG;

    Json::Value body;

    body["name"] = UnicToUtf8(friendlyName);
    body["data"] = UnicToUtf8(url);

    if ( !Send( "appmsg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendActivityEx(BSTR appid, BSTR appname, BSTR data)
{
    // TODO: 在此添加实现代码
    if ( !appid || !appname || !data )
        return E_INVALIDARG;

    Json::Value body;

    body["id"]   = UnicToUtf8(appid);
    body["name"] = UnicToUtf8(appname);
    body["data"] = UnicToUtf8(data);

    if ( !Send( "appmsg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::get_ActivityOpened(VARIANT_BOOL* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_active;
    return S_OK;
}

STDMETHODIMP CRobotSession::SendTyping(void)
{
    // TODO: 在此添加实现代码
    if ( !Send( "typing", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::GetUser(BSTR userid, IRobotUser** ppUser)
{
    // TODO: 在此添加实现代码
    if ( !userid || !ppUser )
        return E_INVALIDARG;

    *ppUser = 0;

    CRobotUser* user = m_pRobotUsers->getUser( UnicToUtf8(userid) );
    if ( !user )
        return E_INVALIDARG;

    user->AddRef();
    *ppUser = user;
    return S_OK;
}

STDMETHODIMP CRobotSession::get_Users(IRobotUsers** ppUsers)
{
    // TODO: 在此添加实现代码
    if ( !ppUsers )
        return E_INVALIDARG;

    m_pRobotUsers->AddRef();
    *ppUsers = m_pRobotUsers;
    return S_OK;
}

STDMETHODIMP CRobotSession::get_Robot(BSTR* pStr)
{
    // TODO: 在此添加实现代码
    if ( !pStr )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_robotId).c_str() );
    *pStr = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotSession::get_OpenMode(LONG* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_mode;
    return S_OK;
}

STDMETHODIMP CRobotSession::get_Closed(VARIANT_BOOL* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_FAIL;

    *pVal = m_closed;
    return S_OK;
}

STDMETHODIMP CRobotSession::Close(void)
{
    // TODO: 在此添加实现代码
    if ( m_closed )
        return S_OK;

    m_closed = true;

    if ( !Send( "closesession", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::InviteUser(BSTR user)
{
    // TODO: 在此添加实现代码
    if ( !m_pParent )
        return E_FAIL;

    if ( !user )
        return E_INVALIDARG;

    if ( !SendBase( UnicToUtf8(user), "invite", NULL ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendFile(BSTR uri, BSTR friendlyName)
{
    // TODO: 在此添加实现代码
    if ( !m_pParent )
        return E_FAIL;

    if ( !uri || !friendlyName )
        return E_INVALIDARG;

    Json::Value body;
    body["location"] = UnicToUtf8(uri);

    if ( friendlyName && *friendlyName != 0 )
        body["name"] = UnicToUtf8(friendlyName);

    if ( !Send( "file", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendInk(BSTR inkData)
{
    // TODO: 在此添加实现代码
    if ( !inkData )
        return E_INVALIDARG;

    Json::Value body;
    body = UnicToUtf8(inkData);

    if ( !Send( "inkmsg", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendWink(BSTR uri, BSTR stamp)
{
    // TODO: 在此添加实现代码
    if ( !uri || !stamp )
        return E_INVALIDARG;

    Json::Value body;
    body["location"] = UnicToUtf8(uri);
    body["stamp"]    = UnicToUtf8(stamp);

    if ( !Send( "wink", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendVoiceclip(BSTR uri)
{
    // TODO: 在此添加实现代码
    if ( !uri )
        return E_INVALIDARG;

    Json::Value body;
    body["location"] = UnicToUtf8(uri);

    if ( !Send( "voiceclip", &body ) )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotSession::SendWebcam(BSTR serverIP, LONG serverPort, LONG recipientid, LONG sessionid)
{
    // TODO: 在此添加实现代码
    if ( !serverIP )
        return E_INVALIDARG;

    Json::Value body;
    body["host"] = UnicToUtf8(serverIP);
    body["port"] = serverPort;
    body["rid"]  = recipientid;
    body["wid"]  = sessionid;

    if ( !Send( "webcam", &body ) )
        return E_FAIL;

    return S_OK;
}

void CRobotSession::init( CRobotServer* server, const std::string& robotId, const std::string& userId, const std::string& sessionId, int openMode ) 
{
    m_pParent   = server;
    m_robotId   = robotId;
    m_sessionId = sessionId;
    m_userId    = userId;
    m_mode      = openMode;
}



