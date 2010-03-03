#include "stdafx.h"
#include "RobotUser.h"
#include "Misc.h"

// CRobotUser


STDMETHODIMP CRobotUser::get_ID(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_id)
}

STDMETHODIMP CRobotUser::get_FriendlyName(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_name)
}

STDMETHODIMP CRobotUser::get_Status(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_status)
}

STDMETHODIMP CRobotUser::get_ClientID(LONG* pVal)
{
    IMPL_GET_LONG(pVal, m_clientID)
}

void CRobotUser::setAll( Json::Value& val )
{
    BEGIN_JSON_PARSE(val)
        JSON_BIND_STR(m_id,         id)
        JSON_BIND_STR(m_name,       friendlyName)
        JSON_BIND_STR(m_status,     status)
        JSON_BIND_INT(m_clientID,   clientId)
    END_JSON_PARSE()
}

