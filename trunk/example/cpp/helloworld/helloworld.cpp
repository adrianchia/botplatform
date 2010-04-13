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

        IRobotServerPtr spRobotServer = spRobotServerFactory->CreateRobotServer( "server.botplatform.com", 6602 );
        RobotServerEventsImpl eventImpl;
        eventImpl.DispEventAdvise( spRobotServer );
        spRobotServer->Login( "YourSP", "Password", 60000 );

        std::string cmd;
        while ( true )
        {
            std::cin >> cmd;
            if ( cmd == "exit" )
                break;
        }

        spRobotServer->Logout();
        spRobotServerFactory->Destroy();
    }

    CoUninitialize();
	return 0;
}

