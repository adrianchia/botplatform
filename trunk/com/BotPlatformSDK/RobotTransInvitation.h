// RobotTransInvitation.h : CRobotTransInvitation ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CRobotTransInvitation

class ATL_NO_VTABLE CRobotTransInvitation :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotTransInvitation, &CLSID_RobotTransInvitation>,
	public IRobotTransInvitation
{
public:
    CRobotTransInvitation() : m_size(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTTRANSINVITATION)


BEGIN_COM_MAP(CRobotTransInvitation)
	COM_INTERFACE_ENTRY(IRobotTransInvitation)
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
    STDMETHOD(get_Size)(LONG* pVal);
    STDMETHOD(get_Thumbnail)(BSTR* pVal);

    void setAll( Json::Value& val );

private:
    std::string m_name;
    long        m_size;
    std::string m_thumbnail;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotTransInvitation), CRobotTransInvitation)
