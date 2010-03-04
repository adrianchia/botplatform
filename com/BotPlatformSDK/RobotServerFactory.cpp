#include "stdafx.h"
#include "RobotServerFactory.h"
#include "RobotServer.h"
#include "Misc.h"


void RegisterAllJSonCmds( ManagerBase* man );


// CRobotServerFactory
STDMETHODIMP CRobotServerFactory::CreateRobotServer(BSTR ip, LONG port, IRobotServer** ppRobotServer)
{
    CRobotServer* realServer = NULL;
    HRESULT hr = createInnerObject<CRobotServer>( ppRobotServer, &realServer );
    if ( FAILED(hr) )
        return hr;

    realServer->init( this, ip, port );
    return S_OK;
}

CRobotServerFactory::CRobotServerFactory()
{
    RegisterAllJSonCmds(this);
}


STDMETHODIMP CRobotServerFactory::Init(LONG threadCount)
{
    if ( threadCount <= 0 )
        return E_INVALIDARG;

    srand( (unsigned)time( NULL ) );
    ManagerBase::init( threadCount );
    return S_OK;
}

STDMETHODIMP CRobotServerFactory::Destroy()
{
    ManagerBase::close();
    ManagerBase::clearCmds();
    return S_OK;
}

