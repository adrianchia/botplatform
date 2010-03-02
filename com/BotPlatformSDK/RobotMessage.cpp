// RobotMessage.cpp : CRobotMessage 的实现

#include "stdafx.h"
#include "RobotMessage.h"
#include "Misc.h"

// CRobotMessage


STDMETHODIMP CRobotMessage::get_Signature(BSTR* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_signature).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotMessage::put_Signature(BSTR newVal)
{
    // TODO: 在此添加实现代码
    if ( !newVal )
        return E_INVALIDARG;

    m_signature = UnicToUtf8(newVal);
    return S_OK;
}

STDMETHODIMP CRobotMessage::get_FontStyle(LONG* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_fontStyle;
    return S_OK;
}

STDMETHODIMP CRobotMessage::put_FontStyle(LONG newVal)
{
    // TODO: 在此添加实现代码
    m_fontStyle = newVal;
    return S_OK;
}

STDMETHODIMP CRobotMessage::get_FontName(BSTR* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_fontName).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotMessage::put_FontName(BSTR newVal)
{
    // TODO: 在此添加实现代码
    if ( !newVal )
        return E_INVALIDARG;

    m_fontName = UnicToUtf8(newVal);
    return S_OK;
}

STDMETHODIMP CRobotMessage::get_FontColor(LONG* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_fontColor ? *m_fontColor : 0;
    return S_OK;
}

STDMETHODIMP CRobotMessage::put_FontColor(LONG newVal)
{
    // TODO: 在此添加实现代码
    if ( !m_fontColor )
        m_fontColor = new int;

    *m_fontColor = newVal;
    return S_OK;
}

STDMETHODIMP CRobotMessage::get_Text(BSTR* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_text).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotMessage::put_Text(BSTR newVal)
{
    // TODO: 在此添加实现代码
    if ( !newVal )
        return E_INVALIDARG;

    m_text = UnicToUtf8(newVal);
    return S_OK;
}

STDMETHODIMP CRobotMessage::RegisterEmoticon(BSTR shortcut, BSTR filename)
{
    // TODO: 在此添加实现代码
    if ( !shortcut || !filename )
        return E_INVALIDARG;

    std::string u8_shortcut = UnicToUtf8(shortcut);
    std::string u8_filename = UnicToUtf8(filename);

    bool b = m_emoticons.insert( StringMap::value_type(u8_shortcut, u8_filename) ).second;
    if ( !b )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotMessage::DeregisterEmoticon(BSTR shortcut)
{
    // TODO: 在此添加实现代码
    if ( !shortcut )
        return E_INVALIDARG;

    std::string u8_shortcut = UnicToUtf8(shortcut);

    StringMap::iterator it = m_emoticons.find( u8_shortcut );

    if ( it != m_emoticons.end() )
    {
        m_emoticons.erase(it);
    }

    return S_OK;
}

void CRobotMessage::setAll( Json::Value& val )
{
    if ( val.isNull() )
        return;

    if ( !val["text"].isNull() )
        m_text = val["text"].asString();

    if ( !val["fontName"].isNull() )
        m_fontName = val["fontName"].asString();

    if ( !val["fontColor"].isNull() )
        put_FontColor( val["fontColor"].asInt() );

    if ( !val["fontStyle"].isNull() )
        m_fontStyle = val["fontStyle"].asInt();

    if ( !val["signature"].isNull() )
        m_signature = val["signature"].asString();
}

