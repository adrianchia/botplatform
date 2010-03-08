#pragma once
#include "resource.h"
#include "BotPlatformSDK_i.h"
#include "ManagerBase.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CRobotServerFactory

class ATL_NO_VTABLE CRobotServerFactory :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRobotServerFactory, &CLSID_RobotServerFactory>,
	public IRobotServerFactory,
    public ManagerBase
{
public:

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

