// RobotUser.cpp : CRobotUser ��ʵ��

#include "stdafx.h"
#include "RobotUser.h"
#include "Misc.h"

// CRobotUser


STDMETHODIMP CRobotUser::get_ID(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_id).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotUser::get_FriendlyName(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_name).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotUser::get_Status(BSTR* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    CComBSTR str( Utf8ToUnic(m_status).c_str() );
    *pVal = str.Detach();
    return S_OK;
}

STDMETHODIMP CRobotUser::get_ClientID(LONG* pVal)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = m_clientID;
    return S_OK;
}

void CRobotUser::setAll( Json::Value& val )
{
    if ( val.isNull() )
        return;

    if ( !val["id"].isNull() )
    {
        setID( val["id"].asString() );
    }

    if ( !val["friendlyName"].isNull() )
    {
        setFriendlyName( val["friendlyName"].asString() );
    }

    if ( !val["status"].isNull() )
    {
        setStatus( val["status"].asString() );
    }

    if ( !val["clientId"].isNull() )
    {
        setClientID( val["clientId"].asInt() );
    }
}

