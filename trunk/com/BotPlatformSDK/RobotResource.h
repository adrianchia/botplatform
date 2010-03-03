// RobotResource.h : CRobotResource ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CRobotResource

class ATL_NO_VTABLE CRobotResource :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotResource, &CLSID_RobotResource>,
	public IRobotResource
{
public:
    CRobotResource() : m_size(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTRESOURCE)


BEGIN_COM_MAP(CRobotResource)
	COM_INTERFACE_ENTRY(IRobotResource)
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
    STDMETHOD(get_Name)(BSTR* pVal);
    STDMETHOD(get_Digest)(BSTR* pVal);
    STDMETHOD(get_Size)(LONG* pVal);

public:
    void setName( const std::string& name ) { m_name = name; }
    void setDigest( const std::string& digest ) { m_digest = digest; }
    void setSize( int size ) { m_size = size; }

    const std::string& getName()   const { return m_name; }
    const std::string& getDigest() const { return m_digest; }
    int                getSize()   const { return m_size; }

    void setAll( Json::Value& val );

private:
    std::string m_name;
    std::string m_digest;
    int         m_size;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotResource), CRobotResource)
