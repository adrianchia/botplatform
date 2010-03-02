// RobotServer.h : CRobotServer 的声明

#pragma once
#include "resource.h"       // 主符号

#include "BotPlatformSDK_i.h"
#include "_IRobotServerEvents_CP.h"

#include "ClientBase.h"
#include "ManagerBase.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

class CManagerBase;
class CRobotSession;

// CRobotServer

class ATL_NO_VTABLE CRobotServer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotServer, &CLSID_RobotServer>,
	public IConnectionPointContainerImpl<CRobotServer>,
     public CProxy_IRobotServerEvents<CRobotServer>,
    public IRobotServer,
    public CClientBase
{
public:
    enum 
    {
        F_SIGNON      = 1,
        F_DATA        = 2,
        F_ERROR       = 3,
        F_SIGNOFF     = 4,
        F_KEEP_ALIVE  = 5
    };

public:
    typedef CSafeHandlePtr<CRobotServer>                      HandleType;
    typedef boost::unordered_map<std::string, CRobotSession*> RobotSessionMap;

public:
    CRobotServer() : m_parent(NULL), m_sequenceNumber( rand() ), m_port(0), m_timeout(0)
	{
        m_handleThis.reset( new HandleType::SafeHandleType(this) );
	}

    ~CRobotServer()
    {
        m_handleThis->Clear();
    }

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTSERVER)


BEGIN_COM_MAP(CRobotServer)
	COM_INTERFACE_ENTRY(IRobotServer)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CRobotServer)
    CONNECTION_POINT_ENTRY(__uuidof(_IRobotServerEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

public:

    STDMETHOD(Login)(BSTR spid, BSTR sppwd, LONG timeout);
    STDMETHOD(Logout)(void);
    STDMETHOD(SetDisplayName)(BSTR robotAccount, BSTR displayName);
    STDMETHOD(SetPersonalMessage)(BSTR robotAccount, BSTR personalMessage);
    STDMETHOD(SetDisplayPicture)(BSTR robotAccount, BSTR displayPicture);
    STDMETHOD(SetDisplayPictureEx)(BSTR robotAccount, BSTR displayPicture, BSTR largePicture);
    STDMETHOD(SetScene)(BSTR robotAccount, BSTR scene);
    STDMETHOD(SetColorScheme)(BSTR robotAccount, LONG colorScheme);
    STDMETHOD(CreateSession)(BSTR robot, BSTR user);
    STDMETHOD(PushMessage)(BSTR robot, BSTR user, BSTR message);
    STDMETHOD(RequestContactList)(BSTR robot);
    

public:
    void Init( CManagerBase* man, BSTR ip, int port );

    void AddSession( const std::string& sessionId, CRobotSession* session );
    void RemoveSession( const std::string& sessionId );
    CRobotSession* GetSession( const std::string& sessionId );

    bool Send( const std::string& robotId, const std::string& userId, const std::string& sessionId, const std::string& type, Json::Value* body, 
        int frameType = F_DATA, bool asyn = true );

private:
    virtual bool OnRecv( const boost::system::error_code& error, size_t bytes_transferred );

    bool RecvNext();
    void OnProcessData( const std::string* data );
    void OnGetBody( const void* data, size_t len );
    bool SyncRecv( std::string& data );
    bool SyncRecv( Json::Value& val );
    bool SendBase( int frameType, const void* data, size_t len, bool asyn );
    bool SendKeepAlive();
    bool SendLoginOut();
    bool CheckLoginResp( Json::Value& data, int& status, std::string& connId, std::string& challenge );
    bool CheckRedirect( Json::Value& data, std::string& newip );
    void MakeToken( std::string& token, const std::string& challenge, const std::string& strspid, const std::string& connId, const std::string& strsppwd );
    bool Login( const std::string& spid, const std::string& sppwd, long timeout );
    
    bool Updaterobot( BSTR robotAccount, LONG* status = NULL, BSTR displayName = NULL, BSTR personalMessage = NULL,
        BSTR displayPicture = NULL, BSTR largePicture = NULL, BSTR scene = NULL, LONG* colorScheme = NULL );

    void UnRegisterCheckToken();
    WORD GetNextSequenceNum();
    void CheckNetwork( bool timeout );

private:
    static void ProcessData( HandleType handle, const std::string* data );
    static void SafeProcessData( CRobotServer* p, const std::string* data );

private:
    CManagerBase*   m_parent;
    WORD            m_sequenceNumber;
    std::string     m_ip;
    int             m_port;
    HandleType      m_handleThis;
    RobotSessionMap m_sessionMap;
    boost::mutex    m_sessionMutex;
    CheckTokenPtr   m_checkToken;
    boost::mutex    m_seqNumMutex;
    std::string     m_strspid;
    std::string     m_strsppwd;
    long            m_timeout;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotServer), CRobotServer)
