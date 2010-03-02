// RobotFileDescriptor.cpp : CRobotFileDescriptor ��ʵ��

#include "stdafx.h"
#include "RobotFileDescriptor.h"
#include "Misc.h"

// CRobotFileDescriptor


STDMETHODIMP CRobotFileDescriptor::get_TransferId(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_transferId).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotFileDescriptor::get_Name(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_name).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotFileDescriptor::get_Size(LONG* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_size;
    return S_OK;
}

STDMETHODIMP CRobotFileDescriptor::get_Thumbnail(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_thumbnail).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

void CRobotFileDescriptor::setAll( Json::Value& val )
{
    if ( val.isNull() )
        return;

    if ( !val["transferId"].isNull() )
    {
        m_transferId = val["transferId"].asString();
    }

    if ( !val["name"].isNull() )
    {
        m_name = val["name"].asString();
    }

    if ( !val["thumbnail"].isNull() )
    {
        m_name = val["thumbnail"].asString();
    }

    if ( !val["size"].isNull() )
    {
        m_size = val["size"].asInt();
    }
}

