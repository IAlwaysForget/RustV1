#include "MiscFeatures.h"

#include "context.h"
#include "MiscWeapons.h"
#include "SkinChanger.h"
#include "VisualFeatures.h"
#include "Globals.h"
#include "MovementFeatures.h"
#include "DMALibrary/Memory/Memory.h"
#include "Features/Aimbot/Aimbot.h"
#include "Misc/CheatFunction.h"
#include <VMProtectSDK.h>

VMMDLL_SCATTER_HANDLE MiscScatterHandle;
VMMDLL_SCATTER_HANDLE MiscWriteScatterHandle;
int MiscScatterCount = 0;
int MiscWriteScatterCount = 0;
std::shared_ptr<CheatFunction> UpdateTeam = std::make_shared<CheatFunction>(2000, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;

	BaseLocalPlayer.load()->SetTeam(MiscScatterHandle); // this needs to be called to know the active item
	MiscScatterCount++;
});
std::shared_ptr<CheatFunction> UpdateBelt = std::make_shared<CheatFunction>(900, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	BaseLocalPlayer.load()->SetupBeltContainerList(); // this needs to be called to know the active item
});

std::shared_ptr<CheatFunction> UpdateWeapons = std::make_shared<CheatFunction>(500, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	MiscScatterCount++;
	BaseLocalPlayer.load()->UpdateModelState(MiscScatterHandle);
	BaseLocalPlayer.load()->UpdatePoseType(MiscScatterHandle);
});

std::shared_ptr<CheatFunction> UpdateBattlemode = std::make_shared<CheatFunction>(150, []()
{
	if (Input.load() == nullptr)
		return;
	if (Input.load()->IsKeyClicked(ctx.m_settings.Battlemode_key))
	{
		ctx.m_settings.Toggle(&ctx.m_settings.Battlemode_Enable);
	}
});
std::shared_ptr<CheatFunction> CameraUpdater = std::make_shared<CheatFunction>(50, []()
{
	if (Camera.load() == nullptr)
		return;
	Camera.load()->UpdatePosition(MiscScatterHandle);
	MiscScatterCount++;
});
std::shared_ptr<CheatFunction> UpdateTime = std::make_shared<CheatFunction>(5000, []()
{
	if (TodSky.load() == nullptr)
		return;
	//This needs to be done in a fast af loop, Heavily intensive
	TodSky.load()->SetHour(MiscScatterHandle);
	MiscScatterCount++;
});

void MiscFunctions()
{


	if (BaseLocalPlayer.load() == nullptr)
		return;
#ifdef DO_AUTH
	/*if (LastHeartbeatCheck.load() + std::chrono::seconds(60) < std::chrono::steady_clock::now())
	{
		return;
	}*/
#endif
	UpdateBelt->Execute();
	MiscScatterHandle = mem.CreateScatterHandle();
	MiscWriteScatterHandle = mem.CreateScatterHandle();
	UpdateWeapons->Execute();
	UpdateBattlemode->Execute();
	AimbotKeyManager->Execute();
	CameraUpdater->Execute();
	UpdateTime->Execute();
	if (MiscScatterCount != 0)
	{
		mem.ExecuteReadScatter(MiscScatterHandle);
		MiscScatterCount = 0;
	}
	if (MiscWriteScatterCount != 0)
	{
		mem.ExecuteWriteScatter(MiscWriteScatterHandle);
		MiscWriteScatterCount = 0;
	}
	Aimbot();
	UpdateTeam->Execute();
	VisualFunctions();
	WeaponFunctions();
	SkinChangerFunction();
	MovementFunctions();
	if (MiscScatterCount != 0)
	{
		mem.ExecuteReadScatter(MiscScatterHandle);
		MiscScatterCount = 0;
	}
	if (MiscWriteScatterCount != 0)
	{
		mem.ExecuteWriteScatter(MiscWriteScatterHandle);
		MiscWriteScatterCount = 0;
	}
	mem.CloseScatterHandle(MiscScatterHandle);
	mem.CloseScatterHandle(MiscWriteScatterHandle);
}
