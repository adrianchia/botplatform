// RobotTransInvitation.cpp : CRobotTransInvitation ��ʵ��

#include "stdafx.h"
#include "RobotTransInvitation.h"
#include "Misc.h"

// CRobotTransInvitation


STDMETHODIMP CRobotTransInvitation::get_Name(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_name).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotTransInvitation::get_Size(LONG* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_size;
    return S_OK;
}

STDMETHODIMP CRobotTransInvitation::get_Thumbnail(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_thumbnail).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

void CRobotTransInvitation::setAll( Json::Value& val )
{
    if ( !val["name"].isNull() )
        m_name = val["name"].asString();
    
    if ( !val["size"].isNull() )
        m_size = val["size"].asInt();

    if ( !val["thumbnail"].isNull() )
        m_thumbnail = val["thumbnail"].asString();
}

