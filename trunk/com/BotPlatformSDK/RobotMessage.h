#pragma once
#include "resource.h"
#include "BotPlatformSDK_i.h"
#include "Misc.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CRobotMessage

class ATL_NO_VTABLE CRobotMessage :
	public CComObjectRootEx<CComMultiThreadModel>,
	//public CComCoClass<CRobotMessage, &CLSID_RobotMessage>,
	public IRobotMessage
{
public:
    typedef boost::unordered_map<std::string, std::string> StringMap;

public:
    CRobotMessage() : m_fontStyle(0), m_fontColor(NULL)
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

    bool toValue( Json::Value& val ) const;

    bool hasFontColor() const { return m_fontColor != NULL; }

    int  getFontColor() const { return m_fontColor ? *m_fontColor : 0; }

    void setFontColor( int c );

private:
    DEFINE_STR_PROP(m_signature, Signature)
    DEFINE_STR_PROP(m_fontName,  FontName)
    DEFINE_INT_PROP(m_fontStyle, FontStyle)
    DEFINE_STR_PROP(m_text,      Text)

    DEFINE_TYPE_PROP(StringMap, m_emoticons, Emoticons)

private:
    int* m_fontColor;
};

//OBJECT_ENTRY_AUTO(__uuidof(RobotMessage), CRobotMessage)

