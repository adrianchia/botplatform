#pragma once
#include "resource.h"
#include "BotPlatformSDK_i.h"
#include "Misc.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CRobotResource

class ATL_NO_VTABLE CRobotResource :
	public CComObjectRootEx<CComMultiThreadModel>,
	//public CComCoClass<CRobotResource, &CLSID_RobotResource>,
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
    void setAll( Json::Value& val );

private:
    DEFINE_STR_PROP(m_name,   Name)
    DEFINE_STR_PROP(m_digest, Digest)
    DEFINE_INT_PROP(m_size,   Size)
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotResource), CRobotResource)

