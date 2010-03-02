// RobotSession.h : CRobotSession ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

class CRobotServer;
class CRobotUsers;

// CRobotSession

class ATL_NO_VTABLE CRobotSession :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotSession, &CLSID_RobotSession>,
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
    STDMETHOD(SendInk)(BSTR inkData);
    STDMETHOD(SendWink)(BSTR uri, BSTR stamp);
    STDMETHOD(SendVoiceclip)(BSTR uri);
    STDMETHOD(SendWebcam)(BSTR serverIP, LONG serverPort, LONG recipientid, LONG sessionid);
    

private:
    bool SendBase( const std::string& userId, const std::string& type, Json::Value* body );
    bool Send( const std::string& type, Json::Value* body );

public:
    void init( CRobotServer* server, const std::string& robotId, const std::string& userId, const std::string& sessionId, int openMode );
    void setActive( bool b ) { m_active = b; }
    CRobotUsers* getUsers() const { return m_pRobotUsers; }

private:
    CRobotServer* m_pParent;
    CRobotUsers*  m_pRobotUsers;

    std::string   m_robotId;
    std::string   m_userId;
    std::string   m_sessionId;

    int           m_mode;
    bool          m_closed;
    bool          m_active;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotSession), CRobotSession)
