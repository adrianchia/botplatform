// RobotUsers.h : CRobotUsers 的声明

#pragma once
#include "resource.h"       // 主符号

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif


class CRobotUser;

// CRobotUsers

class ATL_NO_VTABLE CRobotUsers :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotUsers, &CLSID_RobotUsers>,
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
