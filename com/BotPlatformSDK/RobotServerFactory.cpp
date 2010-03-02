// RobotServerFactory.cpp : CRobotServerFactory ��ʵ��

#include "stdafx.h"
#include "RobotServerFactory.h"
#include "RobotServer.h"
#include "Misc.h"


void RegisterAllJSonCmds( CManagerBase* man );

// CRobotServerFactory
STDMETHODIMP CRobotServerFactory::CreateRobotServer(BSTR ip, LONG port, IRobotServer** ppRobotServer)
{
    // TODO: �ڴ����ʵ�ִ���
    CRobotServer* realServer = NULL;
    HRESULT hr = CreateRealObject<CRobotServer>( ppRobotServer, &realServer );
    if ( FAILED(hr) )
        return hr;

    realServer->Init( this, ip, port );
    return S_OK;
}

CRobotServerFactory::CRobotServerFactory()
{
    RegisterAllJSonCmds(this);
}


STDMETHODIMP CRobotServerFactory::Init(LONG threadCount)
{
    // TODO: �ڴ����ʵ�ִ���
    if ( threadCount <= 0 )
        return E_INVALIDARG;

    srand( (unsigned)time( NULL ) );
    CManagerBase::Init( threadCount );
    return S_OK;
}

STDMETHODIMP CRobotServerFactory::Destroy()
{
    CManagerBase::Close();
    CManagerBase::ClearCmds();
    return S_OK;
}