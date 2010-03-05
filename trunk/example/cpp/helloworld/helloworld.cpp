// helloworld.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <atlbase.h>
#include <atlcom.h>
#include <string>
#include <iostream>

#import "../../../com/Bin/BotPlatformSDK.dll" named_guids no_namespace


_ATL_FUNC_INFO  s_info_onMessageReceived = { CC_STDCALL, VT_EMPTY, 2, {  VT_UNKNOWN, VT_UNKNOWN } };

class RobotServerEventsImpl : public IDispEventSimpleImpl<1, RobotServerEventsImpl, &DIID__IRobotServerEvents>
{
public:
    BEGIN_SINK_MAP(RobotServerEventsImpl)
        SINK_ENTRY_INFO(1, DIID__IRobotServerEvents, 3, &RobotServerEventsImpl::onMessageReceived, &s_info_onMessageReceived)
    END_SINK_MAP()

    HRESULT __stdcall onMessageReceived( IRobotSession* session, IRobotMessage* message )
    {
        session->SendText("Hello World!");
        std::cout << message->Text << std::endl;
        return S_OK;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    {
        CComPtr<IRobotServerFactory> spRobotServerFactory;
        spRobotServerFactory.CoCreateInstance( CLSID_RobotServerFactory, NULL, CLSCTX_INPROC );
        spRobotServerFactory->Init( 2 );

        IRobotServerPtr spServer = spRobotServerFactory->CreateRobotServer( "bottest.com", 6602 );
        RobotServerEventsImpl eventImpl;
        eventImpl.DispEventAdvise( spServer );
        spServer->Login("SP000125", "test111", 60000);

        std::string cmd;
        while ( true )
        {
            std::cin >> cmd;
            if ( cmd == "exit" )
                break;
        }

        spServer->Logout();
        spRobotServerFactory->Destroy();
    }

    CoUninitialize();
	return 0;
}

