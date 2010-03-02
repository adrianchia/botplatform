// RobotFileDescriptor.h : CRobotFileDescriptor ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CRobotFileDescriptor

class ATL_NO_VTABLE CRobotFileDescriptor :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotFileDescriptor, &CLSID_RobotFileDescriptor>,
	public IRobotFileDescriptor
{
public:
    CRobotFileDescriptor() : m_size(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTFILEDESCRIPTOR)


BEGIN_COM_MAP(CRobotFileDescriptor)
	COM_INTERFACE_ENTRY(IRobotFileDescriptor)
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
    STDMETHOD(get_TransferId)(BSTR* pVal);
    STDMETHOD(get_Name)(BSTR* pVal);
    STDMETHOD(get_Size)(LONG* pVal);
    STDMETHOD(get_Thumbnail)(BSTR* pVal);

public:
    void setAll( Json::Value& val );

private:
    std::string m_transferId;
    std::string m_name;
    std::string m_thumbnail;
    long        m_size;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotFileDescriptor), CRobotFileDescriptor)
