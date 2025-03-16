#include "Pch.h"
#include "Globals.h"
#include "VehicleEsp.h"
#include "visuals.h"
#include "Config.h"
#include "context.h"
#include "imgui_internal.h"
#include "DMALibrary/Memory/Memory.h"
#include "SDK/Networkable.h"
inline int HighestDistance = 0;

Vehicles GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetType() == ObjectType::Car)
		return ctx.m_settings.Car;
	if (networkable->GetType() == ObjectType::Train)
		return ctx.m_settings.Train;
	if (networkable->GetType() == ObjectType::AttackHelicopter)
		return ctx.m_settings.PatrolHelicopter;
	if (networkable->GetType() == ObjectType::Bradley)
		return ctx.m_settings.Bradley;
	if (networkable->GetType() == ObjectType::MiniCopter)
		return ctx.m_settings.MiniCopter;
	if (networkable->GetType() == ObjectType::AttackMiniCopter)
		return ctx.m_settings.AttackMiniCopter;
	if (networkable->GetType() == ObjectType::ScrapHeli)
		return ctx.m_settings.ScrapHeli;
	if (networkable->GetType() == ObjectType::TugBoat)
		return ctx.m_settings.TugBoat;
	if (networkable->GetType() == ObjectType::RowBoat)
		return ctx.m_settings.RowBoat;
	if (networkable->GetType() == ObjectType::RHIB)
		return ctx.m_settings.RHIB;
	if (networkable->GetType() == ObjectType::SnowMobile)
		return ctx.m_settings.SnowMobile;
	if (networkable->GetType() == ObjectType::SmallSubmarine)
		return ctx.m_settings.SmallSubmarine;
	if (networkable->GetType() == ObjectType::LargeSubmarine)
		return ctx.m_settings.LargeSubmarine;
	if (networkable->GetType() == ObjectType::PedalBike)
		return ctx.m_settings.Pedalbike;
	if (networkable->GetType() == ObjectType::Motorbike)
		return ctx.m_settings.Motorbike;
	if (networkable->GetType() == ObjectType::MotorbikeSidecar)
		return ctx.m_settings.MotorbikeSidecar;

	return ctx.m_settings.ScrapHeli;
}

std::shared_ptr<CheatFunction> UpdateVehicles = std::make_shared<CheatFunction>(15, []()
{
	auto handle = mem.CreateScatterHandle();
	Networkable.load()->UpdateVehiclePositions(handle);
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);

	/*	std::lock_guard<std::mutex> lock(Networkable.load()->VehicleMutex);
		{
			for (auto vehicle : Networkable.load()->VehicleList)
			{
				if (!vehicle->IsValid())
					continue;
				if (vehicle->GetPosition() == Vector3::Zero())
					continue;
				
				auto timediff = std::chrono::high_resolution_clock::now() - vehicle->GetLastPositionUpdate();
				float delta = std::chrono::duration<float>(timediff).count();
			//	printf("Delta: %f\n", delta);	
			//	Vector3 velocity = (vehicle->GetPosition() - vehicle->GetLastPosition()) /delta;
			//	if(!(velocity.x > 1000 || velocity.x < -1000 || velocity.y > 1000 || velocity.y < -1000 || velocity.z > 1000 || velocity.z < -1000))
				//vehicle->AddNewVelocity(velocity);
			}
		
		}*/
});

void VehicleEsp()
{
	if (ctx.m_settings.Misc_DisableESP)
		return;
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (!Networkable.load())
		return;
	if (!BaseLocalPlayer.load()->IsPlayerValid())
	{
		HighestDistance = 0;
		return;
	}
	Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
	if (localplayerpos == Vector3::Zero())
		return;
	int currenthighestdistance = 0;
	std::vector<std::shared_ptr<NetworkableObject>> templist;
	std::lock_guard<std::mutex> lock(Networkable.load()->VehicleMutex);
	{
		templist = Networkable.load()->VehicleList;
	}
	if (templist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}
	for (auto vehicle : templist)
	{
		if (!vehicle->IsValid())
			continue;
		if (vehicle->GetPosition() == Vector3::Zero())
			continue;

		Vehicles cfg = GetConfig(vehicle);
		int distance = (int)Vector3::Distance(localplayerpos, vehicle->GetPosition());
		if (distance <= 0)
			continue;
		if (!cfg.Enable)
			continue;
		if (distance > cfg.MaxDistance)
			continue;
		if (distance > currenthighestdistance)
			currenthighestdistance = distance;
		if (!cfg.EnableInBattleMode && ctx.m_settings.Battlemode_Enable)
			continue;
		ImColor textcolour = cfg.TextColour;
		if (cfg.OpacityCulling == 1)
		{
			textcolour.a = ImLerp<float>(1.0f, 0.4f, (float)distance / (float)(HighestDistance + 1));
		}
		int textsize = cfg.FontSize;
		if (ctx.m_settings.Misc_Font_Override)
			textsize = ctx.m_settings.Misc_Font_Override_Size;
		if (cfg.TextCulling == 1)
		{
			textsize = ImLerp<float>(cfg.FontSize, cfg.FontSize - 4, (float)distance / (float)(HighestDistance + 1));
		}
		Vector2 screenpos = WorldToScreen(vehicle->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;
		std::string name = vehicle->GetName();
		std::string wname = cfg.Name ? name : xorstr_("");
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
	}
	HighestDistance = currenthighestdistance;
}
