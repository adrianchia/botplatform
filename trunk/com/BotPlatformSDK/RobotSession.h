#pragma once
#include "resource.h"
#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class CRobotServer;
class CRobotUsers;

// CRobotSession

class ATL_NO_VTABLE CRobotSession :
	public CComObjectRootEx<CComMultiThreadModel>,
	//public CComCoClass<CRobotSession, &CLSID_RobotSession>,
	public IRobotSession
{
public:
	CRobotSession();


DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTSESSION)


BEGIN_COM_MAP(CRobotSession)
	COM_INTERFACE_ENTRY(IRobotSession)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();

	void FinalRelease();

public:
    STDMETHOD(CreateMessage)(IRobotMessage** message);
    STDMETHOD(SendText)(BSTR message);
    STDMETHOD(SendIM)(IRobotMessage* message);
    STDMETHOD(SendNudge)(void);
    STDMETHOD(SendActivity)(BSTR url, BSTR friendlyName);
    STDMETHOD(SendActivityEx)(BSTR appid, BSTR appname, BSTR data);
    STDMETHOD(get_ActivityOpened)(VARIANT_BOOL* pVal);
    STDMETHOD(SendTyping)(void);
    STDMETHOD(GetUser)(BSTR userid, IRobotUser** ppUser);
    STDMETHOD(get_Users)(IRobotUsers** ppUsers);
    STDMETHOD(get_Robot)(BSTR* pStr);
    STDMETHOD(get_OpenMode)(LONG* pVal);
    STDMETHOD(get_Closed)(VARIANT_BOOL* pVal);
    STDMETHOD(Close)(void);
    STDMETHOD(InviteUser)(BSTR user);
    STDMETHOD(SendFile)(BSTR uri, BSTR friendlyName);
    STDMETHOD(SendFileAcceptance)(BSTR transferId, BSTR saveUrl);
    STDMETHOD(SendFileRejection)(BSTR transferId);
    STDMETHOD(SendFileCancellation)(BSTR transferId);
    STDMETHOD(SendInk)(BSTR inkData);
    STDMETHOD(SendWink)(BSTR uri, BSTR stamp);
    STDMETHOD(SendVoiceclip)(BSTR uri);
    STDMETHOD(SendWebcam)(BSTR serverIP, LONG serverPort, LONG recipientid, LONG sessionid);
    
private:
    bool sendCmd( const std::string& userId, const std::string& type, Json::Value* body );

    bool sendCmd( const std::string& type, Json::Value* body );

public:
    void init( CRobotServer* server, const std::string& robotId, const std::string& userId, const std::string& sessionId, int openMode );

    void setActive( bool b ) { m_active = b; }

    CRobotUsers* getUsers() const { return m_robotUsers; }

private:
    CRobotServer* m_server;
    CRobotUsers*  m_robotUsers;

    std::string   m_robotId;
    std::string   m_userId;
    std::string   m_sessionId;

    int           m_mode;
    bool          m_closed;
    bool          m_active;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotSession), CRobotSession)

