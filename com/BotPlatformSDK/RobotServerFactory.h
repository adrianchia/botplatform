// RobotServerFactory.h : CRobotServerFactory ������

#pragma once
#include "resource.h"       // ������

#include "BotPlatformSDK_i.h"

#include "ManagerBase.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CRobotServerFactory

class ATL_NO_VTABLE CRobotServerFactory :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotServerFactory, &CLSID_RobotServerFactory>,
	public IRobotServerFactory,
    public CManagerBase
{
public:
	CRobotServerFactory();

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTSERVERFACTORY)


BEGIN_COM_MAP(CRobotServerFactory)
	COM_INTERFACE_ENTRY(IRobotServerFactory)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
        Destroy();
	}

public:
    STDMETHOD(Init)(LONG threadCount);
    STDMETHOD(Destroy)();
    STDMETHOD(CreateRobotServer)(BSTR ip, LONG port, IRobotServer** ppRobotServer);

};

OBJECT_ENTRY_AUTO(__uuidof(RobotServerFactory), CRobotServerFactory)
