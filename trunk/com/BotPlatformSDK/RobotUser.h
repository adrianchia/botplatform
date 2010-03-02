// RobotUser.h : CRobotUser 的声明

#pragma once
#include "resource.h"       // 主符号

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CRobotUser

class ATL_NO_VTABLE CRobotUser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotUser, &CLSID_RobotUser>,
	public IRobotUser
{
public:
    CRobotUser() : m_clientID(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTUSER)


BEGIN_COM_MAP(CRobotUser)
	COM_INTERFACE_ENTRY(IRobotUser)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

    STDMETHOD(get_ID)(BSTR* pVal);
    STDMETHOD(get_FriendlyName)(BSTR* pVal);
    STDMETHOD(get_Status)(BSTR* pVal);
    STDMETHOD(get_ClientID)(LONG* pVal);

public:
    void setID( const std::string& id ) { m_id = id; }
    void setFriendlyName( const std::string& name ) { m_name = name; }
    void setStatus( const std::string& s ) { m_status = s; }
    void setClientID( int id ) { m_clientID = id; }

    void setAll( Json::Value& val );

private:
    std::string m_id;
    std::string m_name;
    std::string m_status;
    int         m_clientID;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotUser), CRobotUser)
