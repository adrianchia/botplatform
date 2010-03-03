#include "stdafx.h"
#include "RobotResource.h"

// CRobotResource


STDMETHODIMP CRobotResource::get_Name(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_name)
}

STDMETHODIMP CRobotResource::get_Digest(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_digest)
}

STDMETHODIMP CRobotResource::get_Size(LONG* pVal)
{
    IMPL_GET_LONG(pVal, m_size)
}

void CRobotResource::setAll( Json::Value& val )
{
    BEGIN_JSON_PARSE(val)
        JSON_BIND_STR(m_name,   name)
        JSON_BIND_STR(m_digest, digest)
        JSON_BIND_INT(m_size,   size)
    END_JSON_PARSE()
}

