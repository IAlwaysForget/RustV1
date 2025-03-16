#include "Pch.h"
#include "Globals.h"
#include "DeployableEsp.h"

#include "Config.h"
#include "context.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "SDK/Networkable.h"
inline int HighestDistance = 0;

Deployables GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetType() == ObjectType::Stash)
		return ctx.m_settings.Stash;
	if (networkable->GetType() == ObjectType::SleepingBag)
		return ctx.m_settings.SleepingBag;
	if (networkable->GetType() == ObjectType::LandMine)
		return ctx.m_settings.LandMine;
	if (networkable->GetType() == ObjectType::BearTrap)
		return ctx.m_settings.BearTrap;
	if (networkable->GetType() == ObjectType::ShotgunTrap)
		return ctx.m_settings.ShotgunTrap;
	if (networkable->GetType() == ObjectType::AutoTurret)
		return ctx.m_settings.AutoTurret;
	if (networkable->GetType() == ObjectType::FlameTurret)
		return ctx.m_settings.FlameTurret;
	if (networkable->GetType() == ObjectType::SamSite)
		return ctx.m_settings.SamSite;
	if (networkable->GetType() == ObjectType::ToolCupboard)
		return ctx.m_settings.ToolCupboard;
	if (networkable->GetType() == ObjectType::Planter)
		return ctx.m_settings.Planter;
	if (networkable->GetType() == ObjectType::LargeBox)
		return ctx.m_settings.LargeBox;
	if (networkable->GetType() == ObjectType::Furnace)
		return ctx.m_settings.Furnace;
	if (networkable->GetType() == ObjectType::LargeFurnace)
		return ctx.m_settings.Furnace;
	if (networkable->GetType() == ObjectType::OilRefinery)
		return ctx.m_settings.OilRefinery;
	if (networkable->GetType() == ObjectType::Recycler)
		return ctx.m_settings.Recycler;
	if (networkable->GetType() == ObjectType::Battery)
		return ctx.m_settings.Battery;
	if (networkable->GetType() == ObjectType::Workbench1)
		return ctx.m_settings.Workbench;
	if (networkable->GetType() == ObjectType::Workbench2)
		return ctx.m_settings.Workbench;
	if (networkable->GetType() == ObjectType::Workbench1)
		return ctx.m_settings.Workbench;

	return ctx.m_settings.Workbench;
}

void DeployableEsp()
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
	std::vector<std::shared_ptr<NetworkableObject>> templist;
	std::lock_guard<std::mutex> lock(Networkable.load()->DeployableMutex);
	{
		templist = Networkable.load()->DeployableList;
	}
	if (templist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}

	int currenthighestdistance = 0;

	for (auto deployable : templist)
	{
		if (!deployable->IsValid())
			continue;
		if (deployable->GetPosition() == Vector3::Zero())
			continue;

		Deployables cfg = GetConfig(deployable);
		int distance = (int)Vector3::Distance(localplayerpos, deployable->GetPosition());
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
		Vector2 screenpos = WorldToScreen(deployable->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;

		std::string name = deployable->GetName();
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		std::string wname = cfg.Name ? name : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
	}
}
