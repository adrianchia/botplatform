#include "stdafx.h"
#include "RobotMessage.h"

// CRobotMessage


STDMETHODIMP CRobotMessage::get_Signature(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_signature)
}

STDMETHODIMP CRobotMessage::put_Signature(BSTR newVal)
{
    IMPL_SET_BSTR(m_signature, newVal)
}

STDMETHODIMP CRobotMessage::get_FontStyle(LONG* pVal)
{
    IMPL_GET_LONG(pVal, m_fontStyle)
}

STDMETHODIMP CRobotMessage::put_FontStyle(LONG newVal)
{
    IMPL_SET_LONG(m_fontStyle, newVal)
}

STDMETHODIMP CRobotMessage::get_FontName(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_fontName)
}

STDMETHODIMP CRobotMessage::put_FontName(BSTR newVal)
{
    IMPL_SET_BSTR(m_fontName, newVal)
}

STDMETHODIMP CRobotMessage::get_FontColor(LONG* pVal)
{
    IMPL_GET_LONG(pVal, getFontColor())
}

STDMETHODIMP CRobotMessage::put_FontColor(LONG newVal)
{
    setFontColor(newVal);
    return S_OK;
}

STDMETHODIMP CRobotMessage::get_Text(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_text)
}

STDMETHODIMP CRobotMessage::put_Text(BSTR newVal)
{
    IMPL_SET_BSTR(m_text, newVal)
}

STDMETHODIMP CRobotMessage::RegisterEmoticon(BSTR shortcut, BSTR filename)
{
    if ( !shortcut || !filename )
        return E_INVALIDARG;

    std::string u8_shortcut = unicToUtf8(shortcut);
    std::string u8_filename = unicToUtf8(filename);

    bool b = m_emoticons.insert( StringMap::value_type(u8_shortcut, u8_filename) ).second;
    if ( !b )
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP CRobotMessage::DeregisterEmoticon(BSTR shortcut)
{
    if ( !shortcut )
        return E_INVALIDARG;

    std::string u8_shortcut = unicToUtf8(shortcut);

    StringMap::iterator it = m_emoticons.find( u8_shortcut );

    if ( it != m_emoticons.end() )
    {
        m_emoticons.erase(it);
    }

    return S_OK;
}

void CRobotMessage::setFontColor( int c )
{
    if ( !m_fontColor )
        m_fontColor = new int;

    *m_fontColor = c;
}

void CRobotMessage::setAll( Json::Value& val )
{
    BEGIN_JSON_PARSE(val)
        JSON_BIND_STR(m_signature,  signature)
        JSON_BIND_STR(m_text,       text)
        JSON_BIND_STR(m_fontName,   fontName)
        JSON_BIND_INT(m_fontStyle,  fontStyle)
        {
            if ( !val["fontColor"].isNull() )
                setFontColor( val["fontColor"].asInt() );
        }
    END_JSON_PARSE()
}

bool CRobotMessage::toValue( Json::Value& val ) const
{
    if ( getText().empty() )
        return false;

    val["text"] = getText();
    
    if ( !getFontName().empty() )
        val["fontName"] = getFontName();
    
    if ( hasFontColor() )
        val["fontColor"] = getFontColor();
    
    if ( getFontStyle() != 0 )
        val["fontStyle"] = getFontStyle();

    if ( !getSignature().empty() )
        val["signature"] = getSignature();

    for ( StringMap::const_iterator it = getEmoticons().begin(); it != getEmoticons().end(); ++it )
    {
        val["emoticons"][it->first] = it->second;
    }

    return true;
}

