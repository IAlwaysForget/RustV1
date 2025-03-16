#include "pch.h"
#include "CheatThread.h"

#include "context.h"
#include "Features/ESP/PlayerEsp.h"
#include "Features/Misc Features/MiscFeatures.h"
#include "SDK/BaseNetworkable.h"
#include "SDK/consolesystem.h"
#include "SDK/ConvarAdmin.h"
#include "SDK/ConvarGraphics.h"
#include "SDK/FacepunchInput.h"
#include "SDK/maincamera.h"
#include "SDK/OcclusionCulling.h"
#include "SDK/TODSky.h"
#include "SDK/World.h"
#include "SDK/Client.h"
#include "SDK/PlayerConvar.h"
#include "Cheat/Features/Aimbot/Aimbot.h"
#include "SDK/Encryption.h"
#include <VMProtectSDK.h>
std::mutex ConfigMutex;
std::atomic<std::shared_ptr<BasePlayer>> BaseLocalPlayer = nullptr;
std::atomic<std::shared_ptr<BaseNetworkable>> Networkable = nullptr;
std::atomic<std::shared_ptr<MainCamera>> Camera = nullptr;
std::atomic<std::shared_ptr<TODSky>> TodSky = nullptr;
std::atomic<std::shared_ptr<FacepunchInput>> Input = nullptr;
std::atomic<std::shared_ptr<ConsoleSystem>> Console = nullptr;
std::atomic<std::shared_ptr<World>> MapWorld = nullptr;
std::atomic<std::shared_ptr<Client>> ClientInstance = nullptr;
std::atomic<std::shared_ptr<PlayerConvar>> PlayerConvarInstance = nullptr;
std::atomic<std::shared_ptr<EncryptionSystem>> Encryption;
VMMDLL_SCATTER_HANDLE MainReadScatterHandle;
VMMDLL_SCATTER_HANDLE MainWriteScatterHandle;
int MainScatterReadCount = 0;
int MainScatterWriteCount = 0;


void PerServerVariables()
{
	//VMProtectBeginVirtualization(xorstr_("ServerVar"));
	VMProtectBeginUltra("");
	Encryption.store(std::make_shared<EncryptionSystem>());
	if (Encryption.load() == nullptr)
	{
		std::printf(xorstr_("Failed To Load Encryption\n"));
		return;
	}
	Input.store(std::make_shared<FacepunchInput>());
	if (Input.load() == nullptr)
	{
		std::printf(xorstr_("Failed To Load Input\n"));
		return;
	}
	Networkable.store(std::make_shared<BaseNetworkable>());
	if (Networkable.load() == nullptr)
	{
		std::printf(xorstr_("Failed to load Networkable\n"));
	}
	Networkable.load()->ItterateEntities();
	auto handle = mem.CreateScatterHandle();
	BaseLocalPlayer.store(nullptr);
	BaseLocalPlayer.store(std::make_shared<BasePlayer>(0x0, handle, true));
	if (BaseLocalPlayer.load() == nullptr)
	{
		std::printf(xorstr_("Failed To Load LocalPlayer\n"));
		mem.CloseScatterHandle(handle);
		return;
	}
	BaseLocalPlayer.load()->InitializeClasses(handle);
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);
	BaseLocalPlayer.load()->InitializePlayerList();

	
	TodSky.store( std::make_shared<TODSky>());
	if (TodSky.load() == nullptr)
	{
		std::printf(xorstr_("Failed to load TodSky\n"));
	}
	Camera.store(std::make_shared<MainCamera>());
	if (Camera.load() == nullptr)
	{
		std::printf(xorstr_("Failed to load Camera\n"));
	}

	MapWorld.store(std::make_shared<World>());
	if (MapWorld.load() == nullptr)
	{
		std::printf(xorstr_("Failed to load World\n"));
	}

	ClientInstance.store(std::make_shared<Client>());
	if (ClientInstance.load() == nullptr)
	{
		std::printf(xorstr_("Failed to load Client\n"));
	}

	PlayerConvarInstance.store(std::make_shared<PlayerConvar>());
	if (PlayerConvarInstance.load() == nullptr)
	{
		std::printf(xorstr_("Failed to load Player Convar\n"));
	}


	ResetBasePlayerCaches();
}

void SetupCvars()
{
	VMProtectBeginUltra("");
	std::shared_ptr<OcclusionCulling> occlusionculling = std::make_shared<OcclusionCulling>();
	if (occlusionculling == nullptr)
	{	
		std::printf(xorstr_("Failed to load OcclusionCulling\n"));
		return;
	}
	if (ctx.m_settings.Visuals_Misc_AdminEsp)
	{
		occlusionculling->WriteDebugSettings(DebugFilter::Dynamic);
		occlusionculling->WriteLayerMask(131072);
	}
	std::shared_ptr<ConvarGraphics> convargraphics = std::make_shared<ConvarGraphics>();
	if (convargraphics == nullptr)
	{
		std::printf(xorstr_("Failed to load ConvarGraphics\n"));
		return;
	}
	if (ctx.m_settings.Visuals_Misc_ChangeFOV)
		convargraphics->WriteFOV(ctx.m_settings.Visuals_Misc_FOV);
	std::shared_ptr<ConvarAdmin> convaradmin = std::make_shared<ConvarAdmin>();
	if (convaradmin == nullptr)
	{
		std::printf(xorstr_("Failed to load ConvarAdmin\n"));
		return;
	}
//	if (ctx.m_settings.Visuals_Misc_ClearWaterVision)
	//	convaradmin->ClearVisionInWater(true);
//	if (ctx.m_settings.Visuals_Misc_ChangeTime)
//		convaradmin->SetAdminTime(ctx.m_settings.Visuals_Misc_Time);
//	else
//		convaradmin->SetAdminTime(-1);
	if (ctx.m_settings.Visuals_Misc_ForceAdminFlag)
	{
		if (Console.load() == nullptr || !Console.load()->CommandsBlocked())
		{
			Console.store(std::make_shared<ConsoleSystem>());
		}
	}
	
}

bool Initialized = false;

void Initialize()
{
	VMProtectBeginUltra("");
	Initialized = false;
	PerServerVariables();
	while (!BaseLocalPlayer.load()->IsPlayerValid1())
	{
		printf(xorstr_("Waiting For You To Load Into A Server!\n"));
		Sleep(4000);
		Initialize(); // wait till localplayer is valid.
	}
	SetupCvars();

	Initialized = true;
}

bool cached = false;

std::shared_ptr<CheatFunction> CacheNetworkable = std::make_shared<CheatFunction>(9000, []()
{
	if (Networkable.load() == nullptr)
		return;
	Networkable.load()->ItterateEntities();
});
std::shared_ptr<CheatFunction> UpdateStaticNetworkables = std::make_shared<CheatFunction>(4000, []()
{
	if (Networkable.load() == nullptr)
		return;
	Networkable.load()->UpdateNearbyEntities();
});

void CachingThread()
{

	while (true)
	{
		if (BaseLocalPlayer.load() == nullptr || Networkable.load() == nullptr || (BaseLocalPlayer.load()->GetListSize() == 0 && Networkable.load()->GetListSize() == 0))
		{
		//	printf("TOOMA\n");
			Initialize();
			Sleep(2000);
			PerServerVariables(); // got to reset the player another time after joining
			CachePlayers->Execute();
			printf(xorstr_("Waiting For Players To Load\n"));
		}
	//	printf(xorstr_("Caching\n"));
		if (CachePlayers)
			CachePlayers->Execute();
		if (CacheNetworkable)
			CacheNetworkable->Execute();
		if (UpdateStaticNetworkables)
			UpdateStaticNetworkables->Execute();
		MiscFunctions();
		if(ctx.m_settings.Misc_PerformanceSleep)
		std::this_thread::sleep_for(std::chrono::milliseconds(ctx.m_settings.Misc_SleepAmount));
	}
}
void StartCheat()
{
	VMProtectBeginUltra("");
	AllocConsole();

	while (!mem.Init("RustClient.exe", true, true))
	{
		std::printf(xorstr_("Cant Attach To Game\n"));
		Sleep(2000);
		continue;
	}
	mem.FixCr3();
	std::printf(xorstr_("Game Assembly: 0x%X\n"), mem.GetBaseAddress(xorstr_("GameAssembly.dll")));
	std::printf(xorstr_("Unity Player: 0x%X\n"), mem.GetBaseAddress(xorstr_("UnityPlayer.dll")));
	Initialize();
}

void Run()
{
	StartCheat();
	CachingThread();
}
