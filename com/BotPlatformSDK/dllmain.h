// dllmain.h 

class CBotPlatformSDKModule : public CAtlDllModuleT< CBotPlatformSDKModule >
{
public :
	DECLARE_LIBID(LIBID_BotPlatformSDKLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BOTPLATFORMSDK, "{07002944-A08F-4A21-A500-8D663D11E706}")
};

extern class CBotPlatformSDKModule _AtlModule;
