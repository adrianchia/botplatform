#pragma once
#include "resource.h"
#include "BotPlatformSDK_i.h"
#include "_IRobotServerEvents_CP.h"
#include "ClientBase.h"
#include "ManagerBase.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class ManagerBase;
class CRobotSession;

// CRobotServer

class ATL_NO_VTABLE CRobotServer :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRobotServer, &CLSID_RobotServer>,
	public IConnectionPointContainerImpl<CRobotServer>,
    public IRobotServer,
    public ClientBase,
    public CProxy_IRobotServerEvents<CRobotServer>
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
    typedef SafeHandlePtr<CRobotServer>                      HandleType;
    typedef boost::unordered_map<std::string, CRobotSession*> RobotSessionMap;

public:
    CRobotServer() : m_serverMan(NULL), m_sequenceNumber(rand()), m_port(0), m_timeout(0)
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
    STDMETHOD(RequestResource)(BSTR robot, BSTR user, IRobotResource* resource, BSTR saveUrl);

public:
    void init( ManagerBase* man, BSTR ip, int port );

    void addSession( const std::string& sessionId, CRobotSession* session );
    void removeSession( const std::string& sessionId );
    CRobotSession* getSession( const std::string& sessionId );

    bool sendCmd( const std::string& robotId, const std::string& userId, const std::string& sessionId, const std::string& type, Json::Value* body, 
        int frameType = F_DATA, bool asyn = true );

private:
    virtual bool onRecv( const boost::system::error_code& error, size_t bytes_transferred );

    bool recvNext();
    void onProcessData( const std::string* data );
    void onGetBody( const void* data, size_t len );
    bool syncRecv( std::string& data );
    bool syncRecv( Json::Value& val );
    bool sendCmdBase( int frameType, const void* data, size_t len, bool asyn );
    bool sendKeepAlive();
    bool sendLoginOut();
    bool checkLoginResp( Json::Value& data, int& status, std::string& connId, std::string& challenge );
    bool checkRedirect( Json::Value& data, std::string& newip );
    void makeToken( std::string& token, const std::string& challenge, const std::string& strspid, const std::string& connId, const std::string& strsppwd );
    bool login( const std::string& spid, const std::string& sppwd, long timeout );
    
    bool updaterobot( BSTR robotAccount, LONG* status = NULL, BSTR displayName = NULL, BSTR personalMessage = NULL,
        BSTR displayPicture = NULL, BSTR largePicture = NULL, BSTR scene = NULL, LONG* colorScheme = NULL );

    void unRegisterCheckToken();
    WORD getNextSequenceNum();
    void checkNetwork( bool timeout );

private:
    static void processData( HandleType handle, const std::string* data );
    static void safeProcessData( CRobotServer* p, const std::string* data );

private:
    ManagerBase*    m_serverMan;
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

