// RobotResource.cpp : CRobotResource ��ʵ��

#include "stdafx.h"
#include "RobotResource.h"
#include "Misc.h"

// CRobotResource


STDMETHODIMP CRobotResource::get_Name(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_name).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotResource::get_Digest(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_digest).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotResource::get_Size(LONG* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_size;
    return S_OK;
}

void CRobotResource::setAll( Json::Value& val )
{
    if ( val.isNull() )
        return;

    if ( !val["name"].isNull() )
    {
        setName( val["name"].asString() );
    }

    if ( !val["digest"].isNull() )
    {
        setDigest( val["digest"].asString() );
    }

    if ( !val["size"].isNull() )
    {
        setSize( val["size"].asInt() );
    }
}

