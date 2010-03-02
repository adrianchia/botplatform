// RobotUser.h : CRobotUser ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
