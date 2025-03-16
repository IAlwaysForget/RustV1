#include "pch.h"
#include "VisualFeatures.h"

#include "context.h"
#include "Globals.h"
#include "MiscFeatures.h"
#include "DMALibrary/Memory/Memory.h"
#include "Features/Aimbot/Aimbot.h"
#include "Misc/CheatFunction.h"
#include "SDK/BasePlayer.h"
#include "SDK/consolesystem.h"
#include "SDK/ConvarGraphics.h"
#include "UI/Plugins/map.h"
#include "SDK/Client.h"
#include "SDK/ConvarAdmin.h"
#include "UI/Plugins/map.h"
#include <VMProtectSDK.h>
std::shared_ptr<CheatFunction> ApplyChams = std::make_shared<CheatFunction>(1000, []()
{
	/*	auto handle = mem.CreateScatterHandle();
		ActiveSkin->UpdateAllSkins(handle);
		mem.ExecuteScatterRead(handle);
		mem.CloseScatterHandle(handle);
	
		handle = mem.CreateScatterHandle();
		ActiveSkin->SetSkins(handle);
		mem.ExecuteScatterRead(handle);
		mem.CloseScatterHandle(handle);
	
		handle = mem.CreateScatterHandle();
		ActiveSkin->SetSkinVariables(handle);
		mem.ExecuteScatterRead(handle);
		mem.CloseScatterHandle(handle);
	
		handle = mem.CreateScatterHandle();
		ActiveSkin->SetGroups(handle);
		mem.ExecuteScatterRead(handle);
		mem.CloseScatterHandle(handle);
	
		handle = mem.CreateScatterHandle();
		ActiveSkin->SetGroupList(handle);
		mem.ExecuteScatterRead(handle);
		mem.CloseScatterHandle(handle);
	
	
		//	handle = mem.CreateScatterHandle();
		//	ActiveSkin->GetMaterial(handle);
		//	mem.ExecuteScatterRead(handle);
		//	mem.CloseScatterHandle(handle);
	
		//	handle = mem.CreateScatterHandle();
		ActiveSkin->SetMaterial(handle, TodSky.load()->ScatterMaterial);
		//	mem.ExecuteScatterWrite(handle);
		//	mem.CloseScatterHandle(handle);*/
});

void UpdateCaves()
{
	if (TodSky.load() == nullptr)
		return;
	if (ctx.m_settings.Visuals_Misc_BrightCave)
	{
		TodSky.load()->WriteDayAmbientMultiplier(MiscWriteScatterHandle, 2.0f);
		TodSky.load()->WriteNightAmbientMultiplier(MiscWriteScatterHandle, 4.0f);
		MiscWriteScatterCount++;
	}
	
}

std::shared_ptr<CheatFunction> RadarKey = std::make_shared<CheatFunction>(60, []()
	{
		if (!ctx.m_settings.Radar_Enable)
			return;
		if (Input.load()->IsKeyDown(ctx.m_settings.Radar_Centre_Key))
		{
			Map::CentreRadar.store(true);
		}
	});
std::shared_ptr<CheatFunction> OverlayTransparentControls = std::make_shared<CheatFunction>(100, []()
{
	if (!ctx.m_settings.Overlay_Transparent)
		return;
	if (!Input.load())
		return;

	if (Input.load()->IsKeyDown(ctx.m_settings.Overlay_RevertKey))
	{
		ctx.m_settings.Toggle(&ctx.m_settings.Overlay_Transparent);
	}
});

std::shared_ptr<CheatFunction> ZoomControls = std::make_shared<CheatFunction>(60, []()
	{
		if (!Input.load())
			return;

		if (Input.load()->IsKeyDown(ctx.m_settings.Radar_Centre_ZoomIn))
		{
			Map::mapZoom.store(Map::mapZoom.load() + .2f);
		}
		if (Input.load()->IsKeyDown(ctx.m_settings.Radar_Centre_ZoomOut))
		{
			if(Map::mapZoom.load() - .2f > 0)
			Map::mapZoom.store(Map::mapZoom.load() - .2f);
		}
	});

std::shared_ptr<CheatFunction> DebugCameraController = std::make_shared<CheatFunction>(10, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (Input.load() == nullptr)
		return;
	//if (!ctx.m_settings.Visuals_Misc_DebugCam)
		//return;
/*
	if (Console.load() == nullptr || !Console.load()->CommandsBlocked())
	{
		Console.store(std::make_shared<ConsoleSystem>());
		Sleep(1200);
	}
	if (Console.load() == nullptr || !Console.load()->CommandsBlocked())
		return;
	if (Input.load()->IsKeyDown(ctx.m_settings.Visuals_Misc_DebugCamKey))
	{

		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->SetIntPlayerFlag(MiscScatterHandle);
		mem.ExecuteReadScatter(handle);
		mem.CloseScatterHandle(handle);
		handle = mem.CreateScatterHandle();
		std::shared_ptr<Client> client = std::make_shared<Client>();
		if ((BaseLocalPlayer.load()->GetPlayerFlagInt() & (int)4) != (int)4 && Console.load() != nullptr && Console.load()->CommandsBlocked())
		{
			BaseLocalPlayer.load()->WritePlayerFlag(handle, 4); // 4 is the admin flag value
			client->WriteCamFov(handle, 90.0f);	
			client->WriteCamLookSpeed(handle, 1.f);
			client->WriteCamSpeed(handle, 1.f);
			client->WriteCamZoomSpeed(handle, 1.f);
			mem.ExecuteWriteScatter(handle);
		}
		
		mem.CloseScatterHandle(handle);
	}*/
});

std::shared_ptr<CheatFunction> ForceAdminFlag = std::make_shared<CheatFunction>(100, []()
	{
		if (BaseLocalPlayer.load() == nullptr)
			return;
		if (!ctx.m_settings.Visuals_Misc_ForceAdminFlag)
			return;
		if (Console.load() == nullptr || !Console.load()->CommandsBlocked())
		{
			Console.store(std::make_shared<ConsoleSystem>());
			Sleep(1200);
		}
		if (Console.load() == nullptr || !Console.load()->CommandsBlocked())
			return;
		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->SetIntPlayerFlag(MiscScatterHandle);
		mem.ExecuteReadScatter(handle);
		mem.CloseScatterHandle(handle);
		handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->WritePlayerFlag(handle, 4); // 4 is the admin flag value
		mem.ExecuteWriteScatter(handle);
	});

float OriginalFOV = 0;
bool IsZooming;
bool FovWritten = false;
std::shared_ptr<CheatFunction> Zoom = std::make_shared<CheatFunction>(300, []()
{

	if (Input.load() == nullptr)
		return;

	if (!ctx.m_settings.Visuals_Misc_Zoom)
		return;
	bool down = Input.load()->IsKeyDown(ctx.m_settings.Visuals_Misc_ZoomKey);
	if (OriginalFOV == 0 && !ctx.m_settings.Visuals_Misc_ChangeFOV)
	{
		std::shared_ptr<ConvarGraphics> graphics = std::make_shared<ConvarGraphics>();
		OriginalFOV = graphics->ReadFOV();
	}
	if (ctx.m_settings.Visuals_Misc_ChangeFOV)
		OriginalFOV = ctx.m_settings.Visuals_Misc_FOV;
	if (!down && !FovWritten)
	{
		FovWritten = true;
		std::shared_ptr<ConvarGraphics> graphics = std::make_shared<ConvarGraphics>();
		graphics->WriteFOV(OriginalFOV);
	}
	if (!down)
	{
		IsZooming = false;
		return;
	}
	if (IsZooming)
		return;

	std::shared_ptr<ConvarGraphics> graphics = std::make_shared<ConvarGraphics>();
	if (!graphics)
		return;
	OriginalFOV = graphics->ReadFOV();
	graphics->WriteFOV(OriginalFOV - ctx.m_settings.Visuals_Misc_ZoomAmount);
	IsZooming = true;
	FovWritten = false;
});

std::shared_ptr<CheatFunction> ClothingADSBlocker = std::make_shared<CheatFunction>(900, []()
	{
		if (BaseLocalPlayer.load() == nullptr)
			return;

		if (!ctx.m_settings.Visuals_Misc_ADSBlocker)
			return;

		BaseLocalPlayer.load()->WriteClothingAimBlock(MiscWriteScatterHandle,false);
		MiscWriteScatterCount++;
	
	});

inline std::shared_ptr<BasePlayer> GetTargetPlayer()
{
	try
	{
		if (!BaseLocalPlayer.load())
			return nullptr;
		if (AimbotTarget.load() != nullptr)
		{
			if (!AimbotTarget.load()->GetIsNPC())
				return AimbotTarget;
		}
		std::vector<std::shared_ptr<BasePlayer>> templist;
		std::lock_guard<std::mutex> lock(PlayerCacheMutex);
		{
			templist = BaseLocalPlayer.load()->PlayerList;
		}
		if (templist.size() == 0)
		{
			return nullptr;
		}
		QuickSortPlayers(templist, 0, templist.size() - 1);
		Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		for (std::shared_ptr<BasePlayer> player : templist)
		{
			if (!player->IsPlayerValid())
				continue;
			if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
				continue;
			if (player->IsSleeper())
				continue;
			if (player->GetIsNPC())
				continue;
			if (player->IsFriendly())
				continue;
			if (player->IsDestroyed())
				continue;
			return player;
		}
	}
	catch (...)
	{
	}
	return nullptr;
}

std::vector<std::string> GetTargetHotbar()
{
	std::vector<std::string> ret = { };
	std::shared_ptr<BasePlayer> target = GetTargetPlayer();
	if (target == nullptr)
		return ret;
	std::string name = xorstr_("Player - ") + target->GetName() + xorstr_(":");
	ret.push_back(name);
	std::vector<std::shared_ptr<Item>> items = target->GetBeltContainerList();
	if (items.size() != 0)
	{
		for (std::shared_ptr<Item> item : items)
		{
			if (item == nullptr)
				continue;
			if (ItemNames.find(item->GetItemID()) == ItemNames.end())
				continue;
			std::string itemname = ItemNames[item->GetItemID()];
			std::string witemname = itemname;
			ret.push_back(witemname);
		}
	}
	return ret;
}		

std::vector<std::string> GetTargetTeam()
{
	/*std::vector<std::string> ret = {};
	std::shared_ptr<BasePlayer> target = GetTargetPlayer();
	if (target == nullptr)
		return ret;
	std::string name = xorstr_("Player - ") + target->GetName() + xorstr_(":");
	ret.push_back(name);
	if (target->GetTeamID() == 0)
	{
		ret.push_back(xorstr_("Player Has No Team"));
		return ret;
	}
	if (TeamMembers.find(target->GetTeamID()) == TeamMembers.end())
	{
		ret.push_back(xorstr_("Player Has No Team"));
		return ret;
	}
	std::vector<uint64_t> members = TeamMembers[target->GetTeamID()];
	for (uint64_t member : members)
	{
		std::string name = std::string(TeamName[member].begin(), TeamName[member].end());
		ret.push_back(name);
	}
	return ret;*/
	return {};
}

std::vector<std::string> GetTargetClothes()
{
	std::vector<std::string> ret = { };
	std::shared_ptr<BasePlayer> target = GetTargetPlayer();
	if (target == nullptr)
		return ret;
	std::string name = xorstr_("Player - ") + target->GetName() + xorstr_(":");
	ret.push_back(name);
	std::vector<std::shared_ptr<Item>> items = target->GetWearContainerList();
	if (items.size() != 0)
	{
		for (std::shared_ptr<Item> item : items)
		{
			if (item == nullptr)
				continue;
			if (ItemNames.find(item->GetItemID()) == ItemNames.end())
				continue;
			std::string itemname = ItemNames[item->GetItemID()];
			std::string witemname = itemname;
			ret.push_back(witemname);
		}
	}
	return ret;
}

std::vector<std::string> GetWorldHud()
{
	std::vector<std::string> ret = { };
	if (TodSky.load() != nullptr)
		ret.push_back(xorstr_("Time: ") + std::to_string(TodSky.load()->GetHour()) + xorstr_("/24"));
	return ret;
}

std::vector<std::string> TargetHotbar;
std::vector<std::string> TargetTeam;
std::vector<std::string> TargetClothes;
std::vector<std::string> WorldHudItems;

void VisualFunctions()
{

	if (ctx.m_settings.Visuals_Misc_ForceAdminFlag)
	{
		if (Console.load() == nullptr || !Console.load()->CommandsBlocked())
		{
			Console.store(std::make_shared<ConsoleSystem>());
		}
	}
	RadarKey->Execute();
	UpdateCaves();
	DebugCameraController->Execute();
	Zoom->Execute();
	OverlayTransparentControls->Execute();
	ZoomControls->Execute();
	ClothingADSBlocker->Execute();
	ForceAdminFlag->Execute();

}
void DrawBattleModeIndicator()
{
	if (!ctx.m_settings.Battlemode_Enable)
		return;
	if (!ctx.m_settings.Battlemode_Indicator)
		return;
	auto drawlist = ImGui::GetBackgroundDrawList();
	ImU32 colour = ImGui::GetColorU32(ImVec4(1, 0, 0, 1));
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		drawlist->AddText(ImVec2(centreofscreen.x, centreofscreen.y - 350), colour, xorstr_("Battlemode Active"));
}
void DrawCrosshair()
{
	if(!ctx.m_settings.Visuals_Misc_Crosshair)
				return;
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	auto drawlist = ImGui::GetBackgroundDrawList();
	ctx.m_draw->DrawCircle(ImVec2(centreofscreen.x, centreofscreen.y), ctx.m_settings.Visuals_Misc_Crosshair_Size, ctx.m_settings.Visuals_Misc_Crosshair_Colour, 0, 1);

}