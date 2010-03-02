// RobotMessage.h : CRobotMessage 的声明

#pragma once
#include "resource.h"       // 主符号

#include "BotPlatformSDK_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CRobotMessage

class ATL_NO_VTABLE CRobotMessage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRobotMessage, &CLSID_RobotMessage>,
	public IRobotMessage
{
public:
    typedef boost::unordered_map<std::string, std::string> StringMap;

public:
    CRobotMessage() : m_fontStyle(0), m_fontColor(0)
	{
	}

    ~CRobotMessage()
    {
        delete m_fontColor;
    }

DECLARE_REGISTRY_RESOURCEID(IDR_ROBOTMESSAGE)


BEGIN_COM_MAP(CRobotMessage)
	COM_INTERFACE_ENTRY(IRobotMessage)
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

    STDMETHOD(get_Signature)(BSTR* pVal);
    STDMETHOD(put_Signature)(BSTR newVal);
    STDMETHOD(get_FontStyle)(LONG* pVal);
    STDMETHOD(put_FontStyle)(LONG newVal);
    STDMETHOD(get_FontName)(BSTR* pVal);
    STDMETHOD(put_FontName)(BSTR newVal);
    STDMETHOD(get_FontColor)(LONG* pVal);
    STDMETHOD(put_FontColor)(LONG newVal);
    STDMETHOD(get_Text)(BSTR* pVal);
    STDMETHOD(put_Text)(BSTR newVal);
    STDMETHOD(RegisterEmoticon)(BSTR shortcut, BSTR filename);
    STDMETHOD(DeregisterEmoticon)(BSTR shortcut);

public:
    void setAll( Json::Value& val );

public:
    std::string m_signature;
    int         m_fontStyle;
    std::string m_fontName;
    int*        m_fontColor;
    std::string m_text;
    StringMap   m_emoticons;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotMessage), CRobotMessage)
