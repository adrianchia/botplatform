#include "stdafx.h"
#include "RobotFileDescriptor.h"


// CRobotFileDescriptor


STDMETHODIMP CRobotFileDescriptor::get_TransferId(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_transferId)
}

STDMETHODIMP CRobotFileDescriptor::get_Name(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_name)
}

STDMETHODIMP CRobotFileDescriptor::get_Size(LONG* pVal)
{
    IMPL_GET_LONG(pVal, m_size)
}

STDMETHODIMP CRobotFileDescriptor::get_Thumbnail(BSTR* pVal)
{
    IMPL_GET_BSTR(pVal, m_thumbnail)
}

void CRobotFileDescriptor::setAll( Json::Value& val )
{
    BEGIN_JSON_PARSE(val)
        JSON_BIND_STR(m_transferId, transferId)
        JSON_BIND_STR(m_name,       name)
        JSON_BIND_STR(m_thumbnail,  thumbnail)
        JSON_BIND_INT(m_size,       size)
    END_JSON_PARSE()
}

