#include "Pch.h"
#include "Misc/kmboxNet.h"
#include "Aimbot.h"

#include "context.h"
#include "globals.h"
#include "Features/ESP/visuals.h"
#include "Features/Misc Features/MiscFeatures.h"
#include "Features/Misc Features/MiscWeapons.h"
#include "Misc/kmbox.h"

#include "Misc/PerlinNoise.hpp"
#include "SDK/Networkable.h"
#include <VMProtectSDK.h>
struct LastTargetInformation
{
	bool Active;
	bool NPC;
	std::wstring Username;
};
float generateNoise(float x, float y)
{
	std::random_device rd;
	siv::PerlinNoise::seed_type seed = rd();
	const siv::PerlinNoise perlin {seed};
	return perlin.octave2D_01(x, y, 4);
}

std::unordered_map<std::string, float> GetBulletSpeed =
{
	{xorstr_("Assault Rifle - ICEammo.rifle.explosive"), 225.0f},
	{xorstr_("Assault Rifle - ICEammo.rifle"), 375.0f},
	{xorstr_("Assault Rifle - ICEammo.rifle.hv"), 450.0f},
	{xorstr_("Assault Rifle - ICEammo.rifle.incendiary"), 225.0f},
	{xorstr_("Assault Rifleammo.rifle.hv"), 420.0f},
	{xorstr_("Assault Rifleammo.rifle"), 375.0f},
	{xorstr_("Assault Rifleammo.rifle.incendiary"), 225.0f},
	{xorstr_("Assault Rifleammo.rifle.explosive"), 225.0f},
	{xorstr_("Bolt Action Rifleammo.rifle"), 445.0f}, // tuned
	{xorstr_("Bolt Action Rifleammo.rifle.incendiary"), 220.0f},
	{xorstr_("Bolt Action Rifleammo.rifle.hv"), 500.0f},
	{xorstr_("Bolt Action Rifleammo.rifle.explosive"), 394.0f},
	{xorstr_("Abyss Assault Rifleammo.rifle"), 375.0f},
	{xorstr_("Abyss Assault Rifleammo.rifle.incendiary"), 225.0f},
	{xorstr_("Abyss Assault Rifleammo.rifle.hv"), 450.0f},
	{xorstr_("Abyss Assault Rifleammo.rifle.explosive"), 225.0f},
	{xorstr_("HMLMGammo.rifle"), 490.0f}, // tuned
	{xorstr_("HMLMGammo.rifle.incendiary"), 250.0f}, // tuned
	{xorstr_("HMLMGammo.rifle.hv"), 630.0f},
	{xorstr_("HMLMGammo.rifle.explosive"), 315.0f},
	{xorstr_("L96 Rifleammo.rifle"), 825.0f}, // tuned
	{xorstr_("L96 Rifleammo.rifle.incendiary"), 225.0f}, // tuned
	{xorstr_("L96 Rifleammo.rifle.hv"), 1100.0f}, // tuned
	{xorstr_("L96 Rifleammo.rifle.explosive"), 675.0f},
	{xorstr_("LR-300 Assault Rifleammo.rifle"),420.0f},
	{xorstr_("LR-300 Assault Rifleammo.rifle.incendiary"), 230.0f},
	{xorstr_("LR-300 Assault Rifleammo.rifle.hv"), 595.0f},
	{xorstr_("LR-300 Assault Rifleammo.rifle.explosive"), 255.0f},
	{xorstr_("M249ammo.rifle"), 488.0f},
	{xorstr_("M249ammo.rifle.incendiary"), 585.0f},
	{xorstr_("M249ammo.rifle.hv"), 293.0f},
	{xorstr_("M249ammo.rifle.explosive"), 293.0f},
	{xorstr_("M39 Rifleammo.rifle"), 390.0f}, // tuned
	{xorstr_("M39 Rifleammo.rifle.incendiary"), 281.0f},
	{xorstr_("M39 Rifleammo.rifle.hv"), 563.0f},
	{xorstr_("M39 Rifleammo.rifle.explosive"), 281.0f},
	{xorstr_("Semi-Automatic Rifleammo.rifle"), 375.0f},
	{xorstr_("Semi-Automatic Rifleammo.rifle.incendiary"), 225.0f},
	{xorstr_("Semi-Automatic Rifleammo.rifle.hv"), 450.0f},
	{xorstr_("Semi-Automatic Rifleammo.rifle.explosive"), 225.0f},
	{xorstr_("SKSammo.rifle"), 475.0f},
	{xorstr_("SKSammo.rifle.incendiary"), 280.0f},
	{xorstr_("SKSammo.rifle.hv"), 550.0f},
	{xorstr_("SKSammo.rifle.explosive"), 280.0f},
	{xorstr_("Custom SMGammo.pistol"), 300.0f}, // custom tuned to hit at sub 200m
	{xorstr_("Custom SMGammo.pistol.fire"), 235.0f}, // turned
	{xorstr_("Custom SMGammo.pistol.hv"), 380.0f}, // turned
	{xorstr_("M92 Pistolammo.pistol"), 300.0f},
	{xorstr_("M92 Pistolammo.pistol.fire"), 225.0f},
	{xorstr_("M92 Pistolammo.pistol.hv"), 400.0f},
	{xorstr_("MP5A4ammo.pistol"), 300.0f}, // tuned
	{xorstr_("MP5A4ammo.pistol.fire"), 228.0f}, // tuned
	{xorstr_("MP5A4ammo.pistol.hv"), 390.0f}, // tuned
	{xorstr_("Thompsonammo.pistol"), 300.0f},
	{xorstr_("Thompsonammo.pistol.fire"), 225.0f},
	{xorstr_("Thompsonammo.pistol.hv"), 400.0f},
	{xorstr_("Python Revolverammo.pistol"), 300.0f},
	{xorstr_("Python Revolverammo.pistol.fire"), 225.0f},
	{xorstr_("Python Revolverammo.pistol.hv"), 400.0f},
	{xorstr_("Revolverammo.pistol"), 300.0f},
	{xorstr_("Revolverammo.pistol.fire"), 225.0f},
	{xorstr_("Revolverammo.pistol.hv"), 400.0f},
	{xorstr_("Semi-Automatic Pistolammo.pistol"), 300.0f},
	{xorstr_("Semi-Automatic Pistolammo.pistol.fire"), 225.0f},
	{xorstr_("Semi-Automatic Pistolammo.pistol.hv"), 400.0f},
	{xorstr_("Double Barrel Shotgunammo.handmade.shell"), 100.0f},
	{xorstr_("Double Barrel Shotgunammo.shotgun"), .2250f},
	{xorstr_("Double Barrel Shotgunammo.shotgun.slug"), 225.0f},
	{xorstr_("Pump Shotgunammo.shotgun"), 225.0f},
	{xorstr_("Double Barrel Shotgunammo.shotgun.fire"), 100.0f},
	{xorstr_("Waterpipe Shotgunammo.handmade.shell"), 100.0f},
	{xorstr_("Waterpipe Shotgunammo.shotgun"), 225.0f},
	{xorstr_("Spas-12 Shotgunammo.shotgun"), 225.0f},
	{xorstr_("Waterpipe Shotgunammo.shotgun.slug"), 225.0f},
	{xorstr_("Waterpipe Shotgunammo.shotgun.fire"), 100.0f},
	{xorstr_("Spas-12 Shotgunammo.shotgun.fire"), 100.0f},
	{xorstr_("Spas-12 Shotgunammo.shotgun.slug"), 225.0f},
	{xorstr_("Spas-12 Shotgunammo.handmade.shell"), 100.0f},
	{xorstr_("M4 Shotgunammo.shotgun"), 293.0f},
	{xorstr_("M4 Shotgunammo.handmade.shell"), 130.0f},
	{xorstr_("M4 Shotgunammo.shotgun.fire"), 130.0f},
	{xorstr_("M4 Shotgunammo.shotgun.slug"), 225.0f},
	{xorstr_("Pump Shotgunammo.shotgun.fire"), 100.0f},
	{xorstr_("Pump Shotgunammo.shotgun.slug"), 225.0f},
	{xorstr_("Eoka Pistolammo.shotgun.slug"), 225.0f},
	{xorstr_("Pump Shotgunammo.handmade.shell"), 225.0f},
	{xorstr_("Prototype 17ammo.pistol"), 300.0f},
	{xorstr_("Prototype 17ammo.pistol.fire"), 225.0f},
	{xorstr_("Prototype 17ammo.pistol.hv"), 400.0f},
	{xorstr_("Multiple Grenade Launcherammo.grenadelauncher.he"), 27.0f}, // tuned
	{xorstr_("Multiple Grenade Launcherammo.grenadelauncher.buckshot"), 225.0f},
	{xorstr_("Multiple Grenade Launcherammo.grenadelauncher.smoke"), 30.0f}, // tuned
	{xorstr_("Speargunspeargun.spear"), .0f},
	{xorstr_("Legacy bowarrow.wooden"), 50.0f},
	{xorstr_("Legacy bowarrow.bone"), 45.0f},
	{xorstr_("Legacy bowarrow.hv"), 80.0f},
	{xorstr_("Legacy bowarrow.fire"), 40.0f},
	{xorstr_("Hunting Bowarrow.wooden"), 50.0f},
	{xorstr_("Hunting Bowarrow.bone"), 45.0f},
	{xorstr_("Hunting Bowarrow.hv"), 80.0f},
	{xorstr_("Hunting Bowarrow.fire"), 40.0f},
	{xorstr_("Crossbowarrow.wooden"), 50.0f}, // tuned
	{xorstr_("Crossbowarrow.bone"), 45.0f}, // tuned
	{xorstr_("Crossbowarrow.hv"), 105.0f}, // tuned
	{xorstr_("Compound Bowarrow.wooden"), 100.0f},
	{xorstr_("Crossbowarrow.fire"), 40.0f}, // tuned
	{xorstr_("Compound Bowarrow.bone"), 90.0f},
	{xorstr_("Compound Bowarrow.hv"), 160.0f},
	{xorstr_("Compound Bowarrow.fire"), 80.0f},
	{xorstr_("Nailgunammo.nailgun.nails"), 50.0f},
	{xorstr_("Minigunammo.rifle"), 455.0f},
};
std::unordered_map<std::string, float> GetBulletGravity =
{
	{xorstr_("ammo.rifle"), 1.f},
	{xorstr_("ammo.rifle.hv"), 1.f},
	{xorstr_("ammo.rifle.explosive"), 1.25f},
	{xorstr_("ammo.rifle.incendiary"), 1.f},
	{xorstr_("ammo.pistol"), 1.f},
	{xorstr_("ammo.pistol.hv"), 1.f},
	{xorstr_("ammo.pistol.fire"), 1.f},
	{xorstr_("arrow.hv"), 0.5f},
	{xorstr_("arrow.bone"), 0.75f},
	{xorstr_("arrow.wooden"), 0.75f},
	{xorstr_("arrow.fire"), 1.f},
	{xorstr_("ammo.handmade.shell"), 1.f},
	{xorstr_("ammo.shotgun"), 1.f},
	{xorstr_("ammo.shotgun.fire"), 1.f},
	{xorstr_("ammo.shotgun.slug"), 1.f},
	{xorstr_("ammo.nailgun.nails"), 0.75f},
	{xorstr_("ammo.grenadelauncher.smoke"), 0.75f},
	{xorstr_("ammo.grenadelauncher.buckshot"), 1.0f},
	{xorstr_("ammo.grenadelauncher.he"), 0.75f},
};
std::unordered_map<std::string, float> GetDrag =
{
	{xorstr_("ammo.rifle"), 0.6f},
	{xorstr_("ammo.rifle.hv"), 0.6f},
	{xorstr_("ammo.rifle.explosive"), 0.6f},
	{xorstr_("ammo.rifle.incendiary"), 0.6f},
	{xorstr_("ammo.pistol"), 0.7f},
	{xorstr_("ammo.pistol.hv"), 0.7f},
	{xorstr_("ammo.pistol.fire"), 0.7f},
	{xorstr_("arrow.hv"), 0.005f},
	{xorstr_("arrow.bone"), 0.01f},
	{xorstr_("arrow.wooden"), 0.005f},
	{xorstr_("arrow.fire"), 0.01f},
	{xorstr_("ammo.handmade.shell"), 1.f},
	{xorstr_("ammo.shotgun"), 1.f},
	{xorstr_("ammo.shotgun.fire"), 1.f},
	{xorstr_("ammo.shotgun.slug"), 0.6f},
	{xorstr_("ammo.nailgun.nails"), 0.005f},
	{xorstr_("ammo.grenadelauncher.smoke"), .005f},
	{xorstr_("ammo.grenadelauncher.buckshot"), 1.0f},
	{xorstr_("ammo.grenadelauncher.he"), .005f},
};

inline bool Aimbotting = false;
inline std::shared_ptr<NetworkableObject> VehicleTarget = nullptr;
inline std::atomic<std::shared_ptr<BasePlayer>> AimbotTarget;
inline LastTargetInformation PreviousTargettingData;
inline ULONGLONG LastTargetTime = 0;

std::shared_ptr<CheatFunction> AimbotKeyManager = std::make_shared<CheatFunction>(100, []()
{
	if (Input.load() == nullptr)
		return;

	if (Input.load()->IsKeyDown(ctx.m_settings.Aimbot_AimKey))
	{
		Aimbotting = true;
	}
	else
	{
		Aimbotting = false;
		AimbotTarget.store(nullptr);
		VehicleTarget = nullptr;
		PreviousTargettingData = { false,false,L"" };
		LastTargetTime = 0;
	}
});

void DrawAimbot()
{
	if(!ctx.m_settings.Aimbot_Enable)
		return;
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	if (AimbotTarget.load() != nullptr && ctx.m_settings.Aimbot_DrawTarget)
	{
		auto draw_list = ImGui::GetBackgroundDrawList();
		const Vector2 w2s = WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false));
		const ImColor colour = ctx.m_settings.Aimbot_DrawTarget_Colour;
		draw_list->AddLine(ImVec2(centreofscreen.x, centreofscreen.y), ImVec2(w2s.x, w2s.y), colour, 2);
	}
	if (ctx.m_settings.Aimbot_DrawFOV)
	{
	
		ctx.m_draw->DrawCircle(ImVec2(centreofscreen.x, centreofscreen.y), ctx.m_settings.Aimbot_FOV, ctx.m_settings.Aimbot_FOVColour, 0, 2);
		
		
	}

}


int ConditionalSwapPlayer(std::vector<std::shared_ptr<BasePlayer>>& entities, int low, int high)
{
	std::shared_ptr<BasePlayer> pivot = entities[high];
	int i = low - 1;
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	for (int j = low; j < high; ++j)
	{
		if (ctx.m_settings.Aimbot_TargetPriority == 2)
		{
			if (Vector2::Distance(centreofscreen, WorldToScreen(entities[j]->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone,false))) < Vector2::Distance(centreofscreen, WorldToScreen(pivot->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false))))
			{
				++i;
				std::swap(entities[i], entities[j]);
				continue;
			}
			if (Vector3::Distance(Camera.load()->GetPosition(), entities[j]->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)) < Vector3::Distance(Camera.load()->GetPosition(), pivot->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (ctx.m_settings.Aimbot_TargetPriority == 0)
		{
			if (Vector3::Distance(Camera.load()->GetPosition(), entities[j]->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)) < Vector3::Distance(Camera.load()->GetPosition(), pivot->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (ctx.m_settings.Aimbot_TargetPriority == 1)
		{
			if (Vector2::Distance(centreofscreen, WorldToScreen(entities[j]->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false))) < Vector2::Distance(centreofscreen, WorldToScreen(pivot->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false))))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
	}
	std::swap(entities[i + 1], entities[high]);
	return i + 1;
}

void QuickSortPlayers(std::vector<std::shared_ptr<BasePlayer>>& entities, int low, int high)
{
	if (low < high)
	{
		int pi = ConditionalSwapPlayer(entities, low, high);
		QuickSortPlayers(entities, low, pi - 1);
		QuickSortPlayers(entities, pi + 1, high);
	}
}

int ConditionalSwapVehicle(std::vector<std::shared_ptr<NetworkableObject>>& entities, int low, int high)
{
	std::shared_ptr<NetworkableObject> pivot = entities[high];
	int i = low - 1;
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	for (int j = low; j < high; ++j)
	{
		if (Vector2::Distance(centreofscreen, WorldToScreen(entities[j]->GetPosition())) < Vector2::Distance(centreofscreen, WorldToScreen(pivot->GetPosition())))
		{
			++i;
			std::swap(entities[i], entities[j]);
			continue;
		}
		if (Vector3::Distance(Camera.load()->GetPosition(), entities[j]->GetPosition()) < Vector3::Distance(Camera.load()->GetPosition(), pivot->GetPosition()))
		{
			++i;
			std::swap(entities[i], entities[j]);
		}
	}
	std::swap(entities[i + 1], entities[high]);
	return i + 1;
}

void QuickSortVehicles(std::vector<std::shared_ptr<NetworkableObject>>& entities, int low, int high)
{
	if (low < high)
	{
		int pi = ConditionalSwapVehicle(entities, low, high);
		QuickSortVehicles(entities, low, pi - 1);
		QuickSortVehicles(entities, pi + 1, high);
	}
}

bool RefindDisposedTarget(std::vector<std::shared_ptr<BasePlayer>> templist)
{
	if (PreviousTargettingData.Active == false)
		return false;

	if (templist.size() == 0)
	{
		return false;
	}
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	for (std::shared_ptr<BasePlayer> player : templist)
	{
		if (!player->IsPlayerValid())
			continue;
		if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
			continue;
		if (player->IsSleeper())
			continue;
		if (Vector3::Distance(Camera.load()->GetPosition(), player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)) > ctx.m_settings.Aimbot_MaxDistance)
			continue;
		if (WorldToScreen(player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)) == Vector2::Zero())
			continue;
		if (player->GetIsNPC() && !ctx.m_settings.Aimbot_TargetScientist)
			continue;
		if (player->GetPosition() == player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false))
			continue;
		if (!player->GetIsNPC() && !ctx.m_settings.Aimbot_TargetPlayer)
			continue;
		if (!ctx.m_settings.Aimbot_StickyAimIgnoreVisibility)
		{
			if (!player->IsPlayerVisible() && ctx.m_settings.Aimbot_VisibleOnly)
				continue;
		}
		if (!ctx.m_settings.Aimbot_StickyAimIgnoreFOV)
		{
			if (Vector2::Distance(WorldToScreen(player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) > ctx.m_settings.Aimbot_FOV)
				continue;
		}
		if (player->IsFriendly())
			continue;
		if (player->IsDestroyed())
			continue;
		
		if (player->GetRawName() == PreviousTargettingData.Username)
		{
			AimbotTarget.store(player);
			return true;
		}
		LastTargetTime = GetTickCount64(); // target is dead or something, reset the timer
	}
	return false;
	
}

bool StickHeli()
{
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

	if (VehicleTarget == nullptr)
		return false;
	if (!Camera.load())
		return false;
	if (Vector3::Distance(Camera.load()->GetPosition(), VehicleTarget->GetPosition()) > ctx.m_settings.Aimbot_VehicleMaxDistance)
		return false;
	if (WorldToScreen(VehicleTarget->GetPosition()) == Vector2::Zero())
		return false;
	if (Vector2::Distance(WorldToScreen(VehicleTarget->GetPosition()), centreofscreen) > ctx.m_settings.Aimbot_FOV)
		return false;

	return true;
}

std::vector<std::shared_ptr<BasePlayer>> PreviousPlayerList;

void GetAimbotTarget()
{
	if (!Networkable.load())
		return;
	if (!Aimbotting)
		return;
	if (!Camera.load())
		return;
	if (BaseLocalPlayer.load() == nullptr)
		return;

	std::vector<std::shared_ptr<NetworkableObject>> vehiclelist;
	std::lock_guard<std::mutex> vehiclelock(Networkable.load()->VehicleMutex);
	{
		vehiclelist = Networkable.load()->VehicleList;
	}
	if (StickHeli())
	{
		for (std::shared_ptr<NetworkableObject> vehicle : vehiclelist)
		{
			if (VehicleTarget)
			{
				if (vehicle->GetClass() == VehicleTarget->GetClass())
				{
					VehicleTarget = vehicle;
					return;
				}
			}
		}
	}
	VehicleTarget = nullptr;
	if (vehiclelist.size() != 0)
	{

		Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		QuickSortVehicles(vehiclelist, 0, vehiclelist.size() - 1);
		for (std::shared_ptr<NetworkableObject> vehicle : vehiclelist)
		{
		
			if (!ctx.m_settings.Aimbot_TargetHeli && !ctx.m_settings.Aimbot_TargetBradley)
				continue;
			if (vehicle == nullptr)
				continue;
			if (vehicle->GetPosition() == Vector3::Zero())
				continue;
			if(vehicle->GetType() != ObjectType::Bradley && vehicle->GetType() != ObjectType::AttackHelicopter)
				continue;
			if (vehicle->GetType() == ObjectType::Bradley && !ctx.m_settings.Aimbot_TargetBradley)
				continue;
			if (vehicle->GetType() == ObjectType::AttackHelicopter && !ctx.m_settings.Aimbot_TargetHeli)
				continue;
			if (WorldToScreen(vehicle->GetPosition()) == Vector2::Zero())
				continue;
			if (Vector3::Distance(vehicle->GetPosition(), Camera.load()->GetPosition()) > ctx.m_settings.Aimbot_VehicleMaxDistance)
				continue;
			if (Vector2::Distance(WorldToScreen(vehicle->GetPosition()), centreofscreen) > ctx.m_settings.Aimbot_FOV)
				continue;
			VehicleTarget = vehicle;
			break;
		}
	}

	std::vector<std::shared_ptr<BasePlayer>> templist;
	Vector3 localpos = Camera.load()->GetPosition();
	std::lock_guard<std::mutex> lock(PlayerCacheMutex);
	{
		templist = BaseLocalPlayer.load()->PlayerList;
	}
	if (templist.size() == 0)
	{
		return;
	}
	QuickSortPlayers(templist, 0, templist.size() - 1);
	if (ctx.m_settings.Aimbot_StickyAim)
	{

		if (RefindDisposedTarget(templist))
			return;
	}
	AimbotTarget.store(nullptr);


	PreviousPlayerList = templist;

	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	if ((GetTickCount64() > LastTargetTime + ctx.m_settings.Aimbot_RetargetTime && ctx.m_settings.Aimbot_StickyAim) || !ctx.m_settings.Aimbot_StickyAim)
	{
		for (std::shared_ptr<BasePlayer> player : templist)
		{
			if (!player->IsPlayerValid())
				continue;
			if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
				continue;
			if (player->IsSleeper())
				continue;
			if (Vector3::Distance(Camera.load()->GetPosition(), player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)) > ctx.m_settings.Aimbot_MaxDistance)
				continue;
			if (WorldToScreen(player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)) == Vector2::Zero())
				continue;
			if (!ctx.m_settings.Aimbot_TargetInjured && player->GetPoseType() == StanceFlags::Crawling)
				continue;
			if (player->GetIsNPC() && !ctx.m_settings.Aimbot_TargetScientist)
				continue;
			if (player->GetPosition() == player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false))
				continue;
			if (!player->GetIsNPC() && !ctx.m_settings.Aimbot_TargetPlayer)
				continue;
			if (!player->IsPlayerVisible() && ctx.m_settings.Aimbot_VisibleOnly)
				continue;
			if (Vector2::Distance(WorldToScreen(player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) > ctx.m_settings.Aimbot_FOV)
				continue;
			if (player->IsFriendly())
				continue;
			if (player->IsDestroyed())
				continue;
			if (VehicleTarget != nullptr && Vector2::Distance(WorldToScreen(VehicleTarget->GetPosition()), centreofscreen) < Vector2::Distance(WorldToScreen(player->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen))
				continue;
			AimbotTarget.store(player);
			PreviousTargettingData = { true,AimbotTarget.load()->GetIsNPC(),AimbotTarget.load()->GetRawName() };
			LastTargetTime = GetTickCount64();
			return;
		}
	}
	PreviousTargettingData = { false,false,L""};
}


static float Clamp(float value, float min, float max)
{
	bool flag = value < min;
	if (flag)
	{
		value = min;
	}
	else
	{
		bool flag2 = value > max;
		if (flag2)
		{
			value = max;
		}
	}
	return value;
}

inline Vector2 CalcAngle(Vector3 dst)
{
	Vector3 direction = Camera.load()->GetPosition() - dst;
	return Vector2(((float)((asinf(direction.y / direction.Length()))) * (float)(180.f / 3.14159265358979323846f)), ((float)(-atan2f(direction.x, -direction.z)) * (float)(180.f / 3.14159265358979323846f)));
}

Vector3 PredictedPos;
Vector3 HybridPredictedPos;

Vector3 Normalized(Vector3 vec)
{
	float num = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (num > FLT_EPSILON)
	{
		return {vec.x / num, vec.y / num, vec.z / num};
	}
	return Vector3 { };
}

int LastItemId = 0;
WeaponID LastWeaponID;
float LastBulletSpeed = 0;
float LastBulletGravity = 0;
float LastDrag = 0;
std::string LastAmmoName;
std::string LastItemName;
float LastScalar;
float LastVelocityScale = 0;
bool LastHasDrag = false;
float ShortBulletDrop(float height, float distance, float velocity, float gravity)
{
	const float pitch = std::atan2(height, distance);
	const float vel = velocity * std::cos(pitch);
	const float time = distance / vel;

	return (0.5f * gravity * time * time) * 10;
}
void Prediction()
{
	//DECLARE_TIME(process);
	//START_TIME(process);
	if (!Camera.load())
		return;

	if (AimbotTarget.load() == nullptr && VehicleTarget == nullptr)
	{
		PredictedPos = Vector3::Zero();
		return;
	}
	if (HeldItem.load() != nullptr)
	{
		if (HeldItem.load()->GetItemID() != LastItemId || LastAmmoName != HeldItem.load()->GetBaseProjectile()->GetAmmoName())
		{
			std::shared_ptr<BaseProjectile> baseprojectile = HeldItem.load()->GetBaseProjectile();
			if (baseprojectile != nullptr)
			{
				// maps are intensive so we cache here to save 10ms
				LastItemId = HeldItem.load()->GetItemID();
				LastWeaponID = WeaponIDS[LocalItemNames[HeldItem.load()->GetItemID()]];
				LastBulletSpeed = GetBulletSpeed[LocalItemNames[HeldItem.load()->GetItemID()] + HeldItem.load()->GetBaseProjectile()->GetAmmoName()];
				LastBulletGravity = GetBulletGravity[HeldItem.load()->GetBaseProjectile()->GetAmmoName()];
				LastDrag = GetDrag[HeldItem.load()->GetBaseProjectile()->GetAmmoName()];
				LastAmmoName = HeldItem.load()->GetBaseProjectile()->GetAmmoName();
				//	LastScalar = baseprojectile->GetProjectileWeaponMod().Scalar;
				LastVelocityScale = baseprojectile->GetVelocityScale();
				LastHasDrag = GetDrag.contains(HeldItem.load()->GetBaseProjectile()->GetAmmoName());
			}
		}
	}
	Vector3 targetpos = Vector3(0, 0, 0);
	bool targetheli = false;
	if (VehicleTarget != nullptr)
	{
		targetpos = VehicleTarget->GetPosition();
		if (VehicleTarget->GetType() == ObjectType::AttackHelicopter)
		{
			targetpos.y += 2.5f;
			targetpos.x += 1.5f;
		}
		if (VehicleTarget->GetType() == ObjectType::Bradley)
		{
			targetpos.y += 1.2f;
			targetpos.x += 0.5f;
		}
		targetheli = true;
	
	}

	if (AimbotTarget.load() != nullptr)
	{
		targetpos = AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone,true);
	
		if (LastWeaponID == WeaponID::Bow || LastWeaponID == WeaponID::Crossbow
			|| LastWeaponID != WeaponID::CompoundBow || LastWeaponID == WeaponID::NailGun)
		{
			if (Vector3::Distance(Camera.load()->GetPosition(), targetpos) < 9)
			{
				//	PredictedPos = targetpos;
				//	HybridPredictedPos = AimbotTarget.load()->GetBonePosition(BoneMap[ctx.m_settings.Aimbot_Hybrid_AimBone]);
				//return;
			}
		}
		else
		{
			if (Vector3::Distance(Camera.load()->GetPosition(), targetpos) < 20)
			{
				//PredictedPos = targetpos;
				//HybridPredictedPos = AimbotTarget.load()->GetBonePosition(BoneMap[ctx.m_settings.Aimbot_Hybrid_AimBone]);
				//return;
			}
		}
	}

	if (HeldItem.load() == nullptr)
	{
		//printf("No held item\n");
		PredictedPos = targetpos;
		return;
	}
	std::shared_ptr<BaseProjectile> baseprojectile = HeldItem.load()->GetBaseProjectile();
	if (baseprojectile == nullptr)
	{
		//	 printf("No base projectile\n");
		PredictedPos = targetpos;
		return;
	}

	float bulletspeed = LastBulletSpeed;
	float bulletgravity = LastBulletGravity;

	float speedscalar = 1.0f;
	auto mod = baseprojectile->GetProjectileWeaponMod();
	if (mod.Enabled)
	{
		speedscalar = baseprojectile->GetProjectileWeaponMod().Scalar;
	}
	float velocityscale = LastVelocityScale;

	Vector3 localpos = Camera.load()->GetPosition();
	float velocity = ((bulletspeed * velocityscale) * speedscalar);
	float drag = 1.0f;

	if (LastHasDrag)
	{
		drag = LastDrag;
	}

	float distance = Vector3::Distance(targetpos, localpos);
	const Vector3 direction = targetpos - localpos;
	const float radius = 0.05f;
	const float diameter = radius * 2.f;
	const float stepsize = 0.03125f;
	int i = 0;

	Vector3 _targetpos = targetpos;
	Vector3 aimpos = _targetpos;
	
	Vector3 closestaimpos = targetpos;
	float closestaimposdist = FLT_MAX;
	float closesttraveltime = -1.f;
	if (!ctx.m_settings.Aimbot_ShortAlgo)
	{
		while (i++ < 128)
		{
			Vector3 startdir = (aimpos - localpos);
			startdir = Normalized(startdir);
			Vector3 startvelocity = startdir * velocity;

			Vector3 currentvelocity = startvelocity;
			Vector3 currentPos = localpos;
			Vector3 prevpos = currentPos;
			Vector3 prevprevpos = prevpos;
			float prevDist = FLT_MAX;

			bool reached = false;

			for (float currentTime = 0.f; currentTime < 8.f; currentTime += stepsize)
			{
				currentPos = currentPos + (currentvelocity * stepsize);
				currentvelocity.y -= 9.81f * bulletgravity * stepsize;
				currentvelocity = currentvelocity - (currentvelocity * drag * stepsize);

				float dist = Vector3::Distance(targetpos, currentPos);

				if (dist > prevDist)
				{
				
					
						Vector3 targetdiff = targetpos - prevprevpos;
						Vector3 posdiff = prevpos - prevprevpos;
						float targetdiffdot = Vector3::Dot(targetdiff, posdiff);
						float posdiffdot = Vector3::Dot(posdiff, posdiff);
						Vector3 closestpos =  prevprevpos + posdiff * (targetdiffdot / posdiffdot);
					
					currentPos = closestpos;

					float closestdist = Vector3::Distance(closestpos, targetpos);

					if (closestdist < closestaimposdist)
					{
						float temptime = currentTime;

						if (Vector3::Distance(prevpos, targetpos) > radius)
						{
							bool negate =
								((localpos.x - prevpos.x) * (localpos.x - prevpos.x) +
									(localpos.y - prevpos.y) * (localpos.y - prevpos.y) +
									(localpos.z - prevpos.z) * (localpos.z - prevpos.z))
								<
								((localpos.x - targetpos.x) * (localpos.x - targetpos.x) +
									(localpos.y - targetpos.y) * (localpos.y - targetpos.y) +
									(localpos.z - targetpos.z) * (localpos.z - targetpos.z));

							float disttotarget = Vector3::Distance(prevpos, closestpos);
							float tickspeed = currentvelocity.Length() * stepsize;

							float finaltime = (disttotarget / tickspeed) * stepsize;
							if (negate)
								finaltime = -finaltime;

							temptime -= finaltime;
						}

						closestaimpos = aimpos;
						closestaimposdist = closestdist;
						closesttraveltime = temptime;
					}
					else
						reached = true;

					if (closestdist <= radius)
						reached = true;

					break;
				}

				prevDist = dist;
				prevprevpos = prevpos;
				prevpos = currentPos;
			}
			if (reached)
				break;

			// if (!higher && currentPos.y > targetpos.y)
			//     break;

			float addDist = (targetpos.y - currentPos.y) * 0.65f;
			if (addDist < diameter)
				addDist = diameter;

			aimpos.y += addDist;
		}
	}
	else
	{
		
		closesttraveltime = 0.f;
		const float target_distance = distance;
		const float bullet_drop = ShortBulletDrop(direction.y, target_distance, velocity, bulletgravity);
		const float time_step = 0.015625f;
		const float bullettime = velocity / distance;
		closestaimpos.y += bullet_drop;
		if (!targetheli)
		{
			if (ctx.m_settings.Aimbot_Movement_Prediction)
			{
				Vector3 movementvelocity = Vector3::Zero();
				Vector3 localvelocity = Vector3::Zero();

				PlayerCacheMutex.lock();
				auto handle = mem.CreateScatterHandle();
				AimbotTarget.load()->SetVelocity(handle);
				mem.ExecuteReadScatter(handle);
				mem.CloseScatterHandle(handle);

				movementvelocity = AimbotTarget.load()->GetAverageVelocity();
				localvelocity = BaseLocalPlayer.load()->GetVelocity();
			
				PlayerCacheMutex.unlock();
				if (movementvelocity.y > 0.f)
					movementvelocity.y /= 3.25f;
				if (!(std::isnan(movementvelocity.x) || std::isnan(movementvelocity.y) || std::isnan(movementvelocity.z) || movementvelocity == Vector3::Zero()
					|| movementvelocity.x > 1000 || movementvelocity.x < -1000 || movementvelocity.y > 1000 || movementvelocity.y < -1000 || movementvelocity.z > 1000 || movementvelocity.z < -1000))
				{
					if (movementvelocity.y > 0.f)
						movementvelocity.y /= 3.25f;
					closestaimpos.x + closestaimpos.x + ((movementvelocity.x ) * bullettime);
					closestaimpos.y + closestaimpos.y + ((movementvelocity.y ) * bullettime);
					closestaimpos.z + closestaimpos.z + ((movementvelocity.z) * bullettime);
					if (ctx.m_settings.Aimbot_Movement_LocalPlayerPrediction)
					{
						closestaimpos.x = closestaimpos.x + (((localvelocity.x) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
						closestaimpos.y = closestaimpos.y + (((localvelocity.y) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
						closestaimpos.z = closestaimpos.z + (((localvelocity.z) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
					}
				}
				else
				{

				}
			}
		}
	}

	if (!targetheli)
	{
		

		if (ctx.m_settings.Aimbot_Movement_Prediction && !ctx.m_settings.Aimbot_ShortAlgo)
		{
			Vector3 movementvelocity = Vector3::Zero();
			Vector3 localvelocity = Vector3::Zero();
			PlayerCacheMutex.lock();
			auto handle = mem.CreateScatterHandle();
			AimbotTarget.load()->SetVelocity(handle);
			mem.ExecuteReadScatter(handle);
			mem.CloseScatterHandle(handle);

			movementvelocity = AimbotTarget.load()->GetAverageVelocity();
			localvelocity = BaseLocalPlayer.load()->GetVelocity();
			PlayerCacheMutex.unlock();
			if (movementvelocity.y > 0.f)
				movementvelocity.y /= 3.25f;
			if (!(std::isnan(movementvelocity.x) || std::isnan(movementvelocity.y) || std::isnan(movementvelocity.z) || movementvelocity == Vector3::Zero()
				|| movementvelocity.x > 1000 || movementvelocity.x < -1000 || movementvelocity.y > 1000 || movementvelocity.y < -1000 || movementvelocity.z > 1000 || movementvelocity.z < -1000))
			{
				//printf("AimbotTarget velocity: %f %f %f\n", std::abs(movementvelocity.x), std::abs(movementvelocity.y), movementvelocity.z);
				//	if (distance > 40)
				//	{

				closestaimpos.x = closestaimpos.x + (((movementvelocity.x) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				closestaimpos.y = closestaimpos.y + (((movementvelocity.y) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				closestaimpos.z = closestaimpos.z + (((movementvelocity.z) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				if (ctx.m_settings.Aimbot_Movement_LocalPlayerPrediction)
				{
					closestaimpos.x = closestaimpos.x + (((localvelocity.x) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
					closestaimpos.y = closestaimpos.y + (((localvelocity.y) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
					closestaimpos.z = closestaimpos.z + (((localvelocity.z) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				}
				//}
				//	else
				//	{
				//		closestaimpos.x = closestaimpos.x + (((movementvelocity.x * 1.3f) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				//		closestaimpos.y = closestaimpos.y + (((movementvelocity.y * 1.f) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				//		closestaimpos.z = closestaimpos.z + (((movementvelocity.z * 1.3f) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				//	}
				//	if (distance > 40)
				//		closestaimpos = closestaimpos + (((movementvelocity * 1.2f) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
				//	else
				//		closestaimpos = closestaimpos + (((movementvelocity * 1.3f) * ctx.m_settings.Aimbot_Movement_Prediction_Modifier) * closesttraveltime);
			}
			else
			{
				//	printf("AimbotTarget velocity is zero\n");
			}
			//	printf("PredictedPos: %f %f %f\n", closestaimpos.x, closestaimpos.y, closestaimpos.z);
		}
		//	if (!(std::isnan(localvelocity.x) || std::isnan(localvelocity.y) || std::isnan(localvelocity.z) || localvelocity == Vector3::Zero()
		//		|| localvelocity.x > 1000 || localvelocity.x < -1000 || localvelocity.y > 1000 || localvelocity.y < -1000 || localvelocity.z > 1000 || localvelocity.z < -1000))
		//	{
		//		closestaimpos = closestaimpos + ((localvelocity*0.5f)*closesttraveltime);
		//	}
	}
	if (closestaimpos == Vector3::Zero())
	{
		PredictedPos = Vector3::Zero();
		return;
	}

	PredictedPos = closestaimpos;

	if (ctx.m_settings.Aimbot_Hybrid && !targetheli)
	{
		targetpos = AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_Hybrid_AimBone,true);
		if (targetpos == Vector3::Zero())
		{
			HybridPredictedPos = Vector3::Zero();
			return;
		}
		aimpos = targetpos;
		closestaimpos = targetpos;
		closestaimposdist = FLT_MAX;
		closesttraveltime = -1.f;

		while (i++ < 256)
		{
			Vector3 startdir = (aimpos - localpos);
			startdir = Normalized(startdir);
			Vector3 startvelocity = startdir * velocity;

			Vector3 currentvelocity = startvelocity;
			Vector3 currentPos = localpos;
			Vector3 prevpos = currentPos;
			Vector3 prevprevpos = prevpos;
			float prevDist = FLT_MAX;

			bool reached = false;

			for (float currentTime = 0.f; currentTime < 8.f; currentTime += stepsize)
			{
				currentPos = currentPos + (currentvelocity * stepsize);
				currentvelocity.y -= 9.81f * bulletgravity * stepsize;
				currentvelocity = currentvelocity - (currentvelocity * drag * stepsize);

				float dist = Vector3::Distance(targetpos, currentPos);

				if (dist > prevDist)
				{
					Vector3 targetdiff = targetpos - prevprevpos;
					Vector3 posdiff = prevpos - prevprevpos;
					float targetdiffdot = Vector3::Dot(targetdiff, posdiff);
					float posdiffdot = Vector3::Dot(posdiff, posdiff);
					Vector3 closestpos = prevprevpos + posdiff * (targetdiffdot / posdiffdot);
					currentPos = closestpos;

					float closestdist = Vector3::Distance(closestpos, targetpos);

					if (closestdist < closestaimposdist)
					{
						float temptime = currentTime;

						if (Vector3::Distance(prevpos, targetpos) > radius)
						{
							bool negate =
								((localpos.x - prevpos.x) * (localpos.x - prevpos.x) +
									(localpos.y - prevpos.y) * (localpos.y - prevpos.y) +
									(localpos.z - prevpos.z) * (localpos.z - prevpos.z))
								<
								((localpos.x - targetpos.x) * (localpos.x - targetpos.x) +
									(localpos.y - targetpos.y) * (localpos.y - targetpos.y) +
									(localpos.z - targetpos.z) * (localpos.z - targetpos.z));

							float disttotarget = Vector3::Distance(prevpos, closestpos);
							float tickspeed = currentvelocity.Length() * stepsize;

							float finaltime = (disttotarget / tickspeed) * stepsize;
							if (negate)
								finaltime = -finaltime;

							temptime -= finaltime;
						}

						closestaimpos = aimpos;
						closestaimposdist = closestdist;
						closesttraveltime = temptime;
					}
					else
						reached = true;

					if (closestdist <= radius)
						reached = true;

					break;
				}

				prevDist = dist;
				prevprevpos = prevpos;
				prevpos = currentPos;
			}
			if (reached)
				break;

			// if (!higher && currentPos.y > targetpos.y)
			//     break;

			float addDist = (targetpos.y - currentPos.y) * 0.65f;
			if (addDist < diameter)
				addDist = diameter;

			aimpos.y += addDist;
		}
	}
	HybridPredictedPos = closestaimpos;
}

std::chrono::system_clock::time_point KmboxStart;

void ClampAngle(float& yaw, float& pitch)
{
	if (yaw > 89.0f)
	{
		yaw = 89.0f;
	}
	if (yaw < -89.0f)
	{
		yaw = -89.0f;
	}
	while (pitch > 180.0f)
	{
		pitch -= 360.0f;
	}
	while (pitch < -180.0f)
	{
		pitch += 360.0f;
	}
}

bool IsAngleValid(Vector2 angles)
{
	if ((!std::isnan(angles.x) && !std::isnan(angles.y)) && (!(angles.x >= (88.f) || angles.x <= (-88.f) || angles.x <= (-88.f) || angles.x >= (88.f))))
	{
		return true;
	}
	return false;
}

void NormalizeAngle(float& pitch, float& yaw)
{
	if (pitch < -89.f) pitch = -89.f;
	if (pitch > 89.f) pitch = 89.f;

	while (yaw < -180.f) yaw += 360.f;
	while (yaw > 180.f) yaw -= 360.f;
}

VMMDLL_SCATTER_HANDLE AimbotHandle;

void ViewAngleAimbot()
{
	if ((!AimbotTarget.load() || AimbotTarget.load() == nullptr) && (!VehicleTarget || VehicleTarget == nullptr))
		return;
	//	DECLARE_TIME(process);
	//START_TIME(process);

	Vector2 aimpos = CalcAngle(PredictedPos);
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	if (ctx.m_settings.Aimbot_SmoothX > 0 && ctx.m_settings.Aimbot_SmoothY > 0)
	{
		if (BaseLocalPlayer.load() == nullptr)
			return;

		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->SetBodyRotation(handle);
		mem.ExecuteReadScatter(handle);
		mem.CloseScatterHandle(handle);

		Vector2 baseangles = BaseLocalPlayer.load()->GetBodyRotation();
		if (baseangles == Vector2::Zero())
			return;
		srand(__rdtsc());
		Vector2 diff = baseangles - aimpos;
		ClampAngle(diff.x, diff.y);
		float randomnumber = (float)(rand() % 100001) / 100000;
		srand(__rdtsc());
		if (ctx.m_settings.Aimbot_SlowMovingProjectile_Smooth)
		{
			if (LastWeaponID == WeaponID::Bow || LastWeaponID == WeaponID::Crossbow
				|| LastWeaponID == WeaponID::CompoundBow || LastWeaponID == WeaponID::NailGun)
			{
				diff.x = baseangles.x - diff.x / (ctx.m_settings.Aimbot_SlowMovingProjectile_SmoothX + (float(rand() % ((int)ctx.m_settings.Aimbot_SlowMovingProjectile_SmoothX) + (float)(rand() % 5) + randomnumber)));
				diff.y = baseangles.y - diff.y / (ctx.m_settings.Aimbot_SlowMovingProjectile_SmoothY + (float(rand() % ((int)ctx.m_settings.Aimbot_SlowMovingProjectile_SmoothY) + (float)(rand() % 5) + randomnumber)));
			}
			else
			{
				diff.x = baseangles.x - diff.x / (ctx.m_settings.Aimbot_SmoothX + (float(rand() % ((int)ctx.m_settings.Aimbot_SmoothX) + (float)(rand() % 5) + randomnumber)));
				diff.y = baseangles.y - diff.y / (ctx.m_settings.Aimbot_SmoothY + (float(rand() % ((int)ctx.m_settings.Aimbot_SmoothY) + (float)(rand() % 5) + randomnumber)));
			}
		}
		else
		{
			diff.x = baseangles.x - diff.x / (ctx.m_settings.Aimbot_SmoothX + (float(rand() % ((int)ctx.m_settings.Aimbot_SmoothX) + (float)(rand() % 5) + randomnumber)));
			diff.y = baseangles.y - diff.y / (ctx.m_settings.Aimbot_SmoothY + (float(rand() % ((int)ctx.m_settings.Aimbot_SmoothY) + (float)(rand() % 5) + randomnumber)));
		}
		aimpos = diff;

		NormalizeAngle(aimpos.x, aimpos.y);
		ClampAngle(aimpos.x, aimpos.y);
		if (IsAngleValid(aimpos))
		{
			if (baseangles.x != aimpos.x && baseangles.y != aimpos.y && Vector2::Distance(Vector2(0, 0), baseangles) != 0 && Vector2::Distance(baseangles, Vector2(0, 0)) != 0)
			{
				BaseLocalPlayer.load()->WriteViewAngles(AimbotHandle, aimpos);
			
				KmboxStart = std::chrono::system_clock::now();
			}
		}

		if (ctx.m_settings.Aimbot_Hybrid)
		{
			if (BaseLocalPlayer.load() == nullptr)
				return;
			if (Vector2::Distance(WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) > ctx.m_settings.Aimbot_Hybrid_FOV)
				return;
			if (Vector2::Distance(WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) < ctx.m_settings.Aimbot_Hybrid_CutOut_FOV)
				return;
			if (HybridPredictedPos == Vector3::Zero())
				return;
			Vector2 aimpos1 = CalcAngle(HybridPredictedPos);
			aimpos1.Clamp();
			Vector4 quaternion = Vector4::ToQuaternion(aimpos1);
			BaseLocalPlayer.load()->WriteEyeAngles(AimbotHandle, quaternion);
		}
	}
	else
	{
	}
	//STOP_TIME(process);
	//std::cout << "Elapsed time in milliseconds: " << GET_TIME_MS(process) << std::endl;
}

void SilentAimbot()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if ((!AimbotTarget.load() || AimbotTarget.load() == nullptr) && (!VehicleTarget || VehicleTarget == nullptr))
		return;

	Vector2 aimpos = CalcAngle(PredictedPos);
	aimpos.Clamp();
	Vector4 quaternion = Vector4::ToQuaternion(aimpos);
	BaseLocalPlayer.load()->WriteEyeAngles(AimbotHandle, quaternion);
}

void KMBoxAimbot()
{
	if ((!AimbotTarget.load() || AimbotTarget.load() == nullptr) && (!VehicleTarget || VehicleTarget == nullptr))
		return;
	Vector2 screen_pos = WorldToScreen(PredictedPos);
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	int smoothx = ctx.m_settings.Aimbot_SmoothX;
	int smoothy = ctx.m_settings.Aimbot_SmoothY;

	int center_y = centreofscreen.y;
	int center_x = centreofscreen.x;
	float angle = std::atan2(screen_pos.y - center_y, screen_pos.x - center_x);
	float distance = std::sqrt(std::pow(screen_pos.x - center_x, 2) + std::pow(screen_pos.y - center_y, 2));
	float x = screen_pos.x - center_x;
	float y = screen_pos.y - center_y;
	float perlin = generateNoise(distance * std::cos(angle), distance * std::sin(angle));
	float aim_strength = (smoothx * smoothy) + perlin;
	float noise = 0;
	if (ctx.m_settings.Aimbot_SmoothingType == 0)
		noise = generateNoise(distance * std::cos(angle), distance * std::sin(angle));
	if (ctx.m_settings.Aimbot_SmoothingType == 1)
		noise = (logf(aim_strength + (distance * std::cos(angle)) / ((ctx.m_settings.Aimbot_FOV) * (ctx.m_settings.Aimbot_FOV)) * aim_strength) * (perlin * perlin)) / 2;
	float scaling_factor_x = (smoothx + std::abs(noise)) * 2.0f;
	float scaling_factor_y = (smoothy + std::abs(noise)) * 2.0f;

	int amount_to_move_x = static_cast<int>(std::round(x / scaling_factor_x));
	int amount_to_move_y = static_cast<int>(std::round(y / scaling_factor_y));
	if (smoothx == 0 || (amount_to_move_x == 0 && Vector2::Distance(screen_pos, centreofscreen) > 2.0f))
		amount_to_move_x = x;
	if (smoothx == 0 || (amount_to_move_y == 0 && Vector2::Distance(screen_pos, centreofscreen) > 2.0f))
		amount_to_move_y = y;
	if (KmboxStart + std::chrono::milliseconds(ctx.m_settings.Aimbot_KmboxDelay) < std::chrono::system_clock::now())
	{
		kmbox::move(amount_to_move_x, amount_to_move_y);
		KmboxStart = std::chrono::system_clock::now();
	}

	if (ctx.m_settings.Aimbot_Hybrid)
	{
		if (BaseLocalPlayer.load() == nullptr)
			return;
		if (Vector2::Distance(WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) > ctx.m_settings.Aimbot_Hybrid_FOV)
			return;
		if (Vector2::Distance(WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) < ctx.m_settings.Aimbot_Hybrid_CutOut_FOV)
			return;
		if (HybridPredictedPos == Vector3::Zero())
			return;
		Vector2 aimpos1 = CalcAngle(HybridPredictedPos);
		aimpos1.Clamp();
		Vector4 quaternion = Vector4::ToQuaternion(aimpos1);
		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->WriteEyeAngles(handle, quaternion);
		mem.ExecuteReadScatter(handle);
		mem.CloseScatterHandle(handle);
	}
}



void KMBoxNetAimbot()
{
		if ((!AimbotTarget.load() || AimbotTarget.load() == nullptr) && (!VehicleTarget || VehicleTarget == nullptr))
		return;

	Vector2 screen_pos = WorldToScreen(PredictedPos);
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	int smoothx = ctx.m_settings.Aimbot_SmoothX;
	int smoothy = ctx.m_settings.Aimbot_SmoothY;

	int center_y = centreofscreen.y;
	int center_x = centreofscreen.x;
	float angle = std::atan2(screen_pos.y - center_y, screen_pos.x - center_x);
	float distance = std::sqrt(std::pow(screen_pos.x - center_x, 2) + std::pow(screen_pos.y - center_y, 2));
	float x = screen_pos.x - center_x;
	float y = screen_pos.y - center_y;
	float perlin = generateNoise(distance * std::cos(angle), distance * std::sin(angle));
	float aim_strength = (smoothx * smoothy) + perlin;
	float noise = 0;
	if (ctx.m_settings.Aimbot_SmoothingType == 0)
		noise = generateNoise(distance * std::cos(angle), distance * std::sin(angle));
	if (ctx.m_settings.Aimbot_SmoothingType == 1)
		noise = (logf(aim_strength + (distance * std::cos(angle)) / ((ctx.m_settings.Aimbot_FOV) * (ctx.m_settings.Aimbot_FOV)) * aim_strength) * (perlin * perlin)) / 2;
	float scaling_factor_x = (smoothx + std::abs(noise)) * 2.0f;
	float scaling_factor_y = (smoothy + std::abs(noise)) * 2.0f;

	int amount_to_move_x = static_cast<int>(std::round(x / scaling_factor_x));
	int amount_to_move_y = static_cast<int>(std::round(y / scaling_factor_y));
	if (smoothx == 0 || (amount_to_move_x == 0 && Vector2::Distance(screen_pos, centreofscreen) > 2.0f))
		amount_to_move_x = x;
	if (smoothx == 0 || (amount_to_move_y == 0 && Vector2::Distance(screen_pos, centreofscreen) > 2.0f))
		amount_to_move_y = y;
	if (KmboxStart + std::chrono::milliseconds(ctx.m_settings.Aimbot_KmboxDelay) < std::chrono::system_clock::now())
	{
		KmBoxMgr.Mouse.Move(amount_to_move_x, amount_to_move_y);
		KmboxStart = std::chrono::system_clock::now();
	}

	if (ctx.m_settings.Aimbot_Hybrid)
	{
		if (BaseLocalPlayer.load() == nullptr)
			return;
		if (Vector2::Distance(WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) > ctx.m_settings.Aimbot_Hybrid_FOV)
			return;
		if (Vector2::Distance(WorldToScreen(AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false)), centreofscreen) < ctx.m_settings.Aimbot_Hybrid_CutOut_FOV)
			return;
		if (HybridPredictedPos == Vector3::Zero())
			return;
		Vector2 aimpos1 = CalcAngle(HybridPredictedPos);
		aimpos1.Clamp();
		Vector4 quaternion = Vector4::ToQuaternion(aimpos1);
		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->WriteEyeAngles(handle, quaternion);
		mem.ExecuteReadScatter(handle);
		mem.CloseScatterHandle(handle);
	}
}

int test123 = 0;

void Aimbot()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (!BaseLocalPlayer.load())
		return;
	if (BaseLocalPlayer.load()->GetListSize() == 0)
		return;

	GetAimbotTarget();
	//STOP_TIME(process);
	//std::cout << "Elapsed time in milliseconds: " << GET_TIME_MS(process) << std::endl;

	if (!ctx.m_settings.Aimbot_Enable)
		return;

	if (AimbotTarget.load() == nullptr && VehicleTarget == nullptr)
		return;
	if (!Aimbotting)
		return;

	if (AimbotTarget.load() != nullptr)
	{
		//	DECLARE_TIME(process);
		//	START_TIME(process);
		//AimbotTarget.load()->SetBonePositionSingleBone(BoneMap[ctx.m_settings.Aimbot_AimBone]);
		//STOP_TIME(process);
		//std::cout << "Elapsed time in milliseconds: " << GET_TIME_MS(process) << std::endl;
		if (AimbotTarget.load()->GetBonePositionByAimbone(ctx.m_settings.Aimbot_AimBone, false) == Vector3::Zero())
		{
			return;
		}
	}
	if (VehicleTarget != nullptr)
	{
		if (VehicleTarget->GetPosition() == Vector3::Zero())
			return;
	}

	//	DECLARE_TIME(process);
	//	START_TIME(process);
	Prediction();

	if (PredictedPos == Vector3::Zero())
		return;
	AimbotHandle = mem.CreateScatterHandle();

	// print aimbot target velocity
	if (ctx.m_settings.Aimbot_AimType == 0)
		ViewAngleAimbot();
	else if (ctx.m_settings.Aimbot_AimType == 1)
		SilentAimbot();
	else if (ctx.m_settings.Aimbot_AimType == 3)
		KMBoxAimbot();
	else if (ctx.m_settings.Aimbot_AimType == 4)
		KMBoxNetAimbot();
	mem.ExecuteWriteScatter(AimbotHandle);
	mem.CloseScatterHandle(AimbotHandle);
	// UpdateVelocity->Execute();
}
