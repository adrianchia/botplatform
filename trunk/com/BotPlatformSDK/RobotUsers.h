#pragma once
#include "resource.h"
#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


class CRobotUser;

// CRobotUsers

class ATL_NO_VTABLE CRobotUsers :
	public CComObjectRootEx<CComMultiThreadModel>,
	//public CComCoClass<CRobotUsers, &CLSID_RobotUsers>,
	public IRobotUsers
{
public:
    typedef boost::unordered_map<std::string, CRobotUser*> RobotUserMap;
    typedef std::vector<CRobotUser*> RobotUserVector;

public:
	CRobotUsers()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTUSERS)


BEGIN_COM_MAP(CRobotUsers)
	COM_INTERFACE_ENTRY(IRobotUsers)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

public:
    STDMETHOD(Item)(LONG idx, IRobotUser** ppUser);
    STDMETHOD(Count)(LONG* pVal);

public:
    void addUser( CRobotUser* user );
    void removeUser( const std::string& userId );
    
    CRobotUser* getUser( const std::string& userId );
    CRobotUser* getUser( size_t idx );

    size_t getUserCount();

private:
    RobotUserMap    m_userMap;
    RobotUserVector m_userVector;
    boost::mutex    m_userMutex;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotUsers), CRobotUsers)

