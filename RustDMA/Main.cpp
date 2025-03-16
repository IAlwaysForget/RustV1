#include <thread>

#include "context.h"
#include "Config/Config.h"
#include "Fuser/fuser.h"
#include <DMALibrary/Memory/Memory.h>
#include <dbghelp.h> 
#include "CheatThread.h"
#include "Globals.h"
#include "Features/Aimbot/Aimbot.h"
#include "Features/ESP/AnimalEsp.h"
#include "Features/ESP/CollectableEsp.h"
#include "Features/ESP/DeployableEsp.h"
#include "Features/ESP/DroppedItemEsp.h"
#include "Features/ESP/LootableEsp.h"
#include "Features/ESP/OreESP.h"
#include "Features/ESP/PlayerEsp.h"
#include "Features/ESP/VehicleEsp.h"
#include "Features/ESP/visuals.h"
#include "Features/Misc Features/VisualFeatures.h"
#include "Features/Misc Features/MiscWeapons.h"
#include "Features/Misc Features/MovementFeatures.h"
#include "Misc/Checksum.h"
#include "UI/Plugins/map.h"
#include "VMProtectSDK.h"
#include "misc/kmbox.h"

#pragma comment(lib, "dbghelp.lib")


c_context ctx;

#ifdef DO_AUTH

#endif
//std::chrono::steady_clock::time_point vorpal_last_run_time_heartbeat;
//std::atomic<int> vorpal_fail_count = 0;
//uint64_t last_time_check = 1;

//std::atomic<std::chrono::steady_clock::time_point> LastHeartbeatCheck;


#pragma warning(push)
#pragma warning(disable : 4789)
// this is to allow the crashing to work
inline bool HeartbeatCheck()
{

#ifdef DO_AUTH
	//	VMProtectBeginMutation("");

		//LastHeartbeatCheck.store(std::chrono::steady_clock::now());

		// this causes the cheat to lag like all hell, its basically unusable. 
	/*if (vorpal_last_run_time_heartbeat + std::chrono::seconds(60) < std::chrono::steady_clock::now())
		{

			// just incase this is patched out heres another one.
		//	int* ptr = (int*)((char*)malloc(sizeof(int)) + 1);
		//	*ptr = 42;
		}*/
#pragma warning(pop)
	return true;

#endif
	return false;
}
std::shared_ptr<CheatFunction> DoHeartbeatCheck = std::make_shared<CheatFunction>(0, []()
	{
#ifdef DO_AUTH
		if (!HeartbeatCheck())
		{
			RefreshViewMatrix(); // this will be unreachable even with auth off.
		}
#endif
	});
void RenderLoop()
{

	if (ctx.m_settings.Overlay_Transparent)
	{
		SetWindowPos(fuser.GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	if (BaseLocalPlayer.load() != nullptr && Networkable.load() != nullptr && (Networkable.load()->GetListSize() != 0))
	{
		//font for overlay drawings
	//if(!ctx.m_settings.Misc_MultiThreadedViewUpdate)

		UpdatePlayers->Execute();
		UpdatePlayersPerFrame->Execute();
		UpdateAnimals->Execute();
		UpdateVehicles->Execute();
		RefreshViewMatrix();
		VehicleEsp();
		DeployableEsp();
		CollectableEsp();
		LootableEsp();
		OreEsp();
		DroppedItemEsp();
		AnimalEsp();
		RefreshViewMatrix();
		PlayerEsp();
		DrawAimbot();
		DrawRecoil();
		DrawAntiAim();
		DrawBattleModeIndicator();
		DrawCrosshair();

	}

	ctx.m_plugin->Run();
	ctx.m_draw->Run();
#ifdef DO_AUTH
	//DoHeartbeat->Execute();
	//DoHeartbeatCheck->Execute();

#endif
}
void VMUpdate()
{
	while (true)
	{
		RefreshViewMatrix();
	}
}
//uncomment to remove auth

void CreateMiniDump(EXCEPTION_POINTERS* pExceptionPointers) {
	std::string userProfile = getenv("USERPROFILE");
	std::string pathh = (userProfile + "\\Documents\\Moe\\MiniDump.dmp");
	HANDLE hFile = CreateFile(pathh.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) {
		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = pExceptionPointers;
		exceptionInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &exceptionInfo, nullptr, nullptr);
		CloseHandle(hFile);
	}
}
std::thread CachingThread;
std::thread VMThread;
LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo) {


	std::thread(Run).detach();
	//if (ctx.m_settings.Misc_MultiThreadedViewUpdate)
	//std::thread(VMUpdate).detach();
	ctx.m_draw->Init(fuser); //Initialize font ect after we have initialized fuser.
	fuser.Run(&RenderLoop);
	printf(xorstr_("(1/3)FATAL ERROR, If Issues Persist Restart\n"));
	printf(xorstr_("(2/3)FATAL ERROR, If Issues Persist Restart\n"));
	printf(xorstr_("(3/3)FATAL ERROR, If Issues Persist Restart\n"));
	DWORD exceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	CreateMiniDump(ExceptionInfo);
	return EXCEPTION_CONTINUE_EXECUTION;
}

int main()
{


	srand(time(NULL));
	VMProtectBeginUltra("main");
#ifdef DO_AUTH
	char username[50];
	char password[50];
	auto currentPath = std::filesystem::temp_directory_path();
	if (std::filesystem::exists(currentPath.string() + xorstr_("\\login.txt")))
	{
		std::ifstream infile(currentPath.string() + xorstr_("\\login.txt"));

		std::string line;
		std::getline(infile, line);
		if (line.find(xorstr_(":")) != std::string::npos)
		{
			std::string User = line.substr(0, line.find(":"));
			std::string Pass = line.substr(line.find(":") + 1);
			User.copy(username, User.size() + 1);
			Pass.copy(password, Pass.size() + 1);

			// null-terminate the char arrays
			username[User.size()] = '\0';
			password[Pass.size()] = '\0';
		}

		infile.close();
	}
	ctx.m_logger->Info(xorstr_("Username: %s\n"), username);
	ctx.m_logger->Info(xorstr_("Logging in...\n"));










	//Login request to application
	ctx.m_logger->Info(xorstr_("login but second time...\n"));


	ctx.m_logger->Info(xorstr_("hashed id matches\n"));
	//HWID is also checked on server, if hwid mismatches in the request send to server, it will throw a error and have results also be false.

	ctx.m_logger->Info(xorstr_("client matches\n"));

	//	vorpal_last_run_time_heartbeat = std::chrono::steady_clock::now();
//			LastHeartbeatCheck.store(std::chrono::steady_clock::now());
#endif
	Map::CreateMapDirectory();
	kmbox::KmboxInitialize("");

	ctx.m_logger->Info("Loading Default Config\n");
	//Check if default config exists, if it doesn't we just create one with the default config settings.
	if (!ctx.m_settings.exists("Default.conf"))
		ctx.m_settings.save("Default.conf");
	//Load the default config.
	ctx.m_settings.load("Default.conf");
	ctx.m_logger->Succes("Loaded Default Config\n");


	DownloadAssets();
	StartCheat();
	if (ctx.m_settings.Misc_HeapMemoryCheck)
		mem.WriteHeapCheck(true);
	ctx.m_logger->Info("Checking Files\n");

	ctx.m_logger->Info("Initializing Cheat...\n");

	CachingThread = std::thread(Run);
	CachingThread.detach();
	//if (ctx.m_settings.Misc_MultiThreadedViewUpdate)
	//{
	//	VMThread = std::thread(VMUpdate);
	//	VMThread.detach();
	//}
	if (ctx.m_settings.Misc_ExceptionCatching)
		SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	//If you wanna make use of SDF set this to true, but usually not needed unless you wanna make icons really crisp when we have them scaled small (example for when on a map & we're zoomed in)
	//Turning on SDF also allows you to make use of shadows.
	fuser.Initialize(ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)), false);
	ctx.m_draw->Init(fuser); //Initialize font ect after we have initialized fuser.
	fuser.Run(&RenderLoop);
#ifdef DO_AUTH












#endif
	return 0;
}
