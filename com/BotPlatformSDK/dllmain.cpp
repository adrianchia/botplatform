// dllmain.cpp

#include "stdafx.h"
#include "resource.h"
#include "BotPlatformSDK_i.h"
#include "dllmain.h"

CBotPlatformSDKModule _AtlModule;

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
