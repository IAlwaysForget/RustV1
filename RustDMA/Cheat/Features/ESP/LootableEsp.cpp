#include "Config.h"
#include "context.h"
#include "Pch.h"
#include "Globals.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "SDK/Networkable.h"
inline int HighestDistance = 0;

Lootables GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetType() == ObjectType::Barrel)
		return ctx.m_settings.Barrel;
	if (networkable->GetType() == ObjectType::FuelBarrel)
		return ctx.m_settings.FuelBarrel;
	if (networkable->GetType() == ObjectType::AmmoCrate)
		return ctx.m_settings.AmmoCrate;
	if (networkable->GetType() == ObjectType::BradleyCrate)
		return ctx.m_settings.BradleyCrate;
	if (networkable->GetType() == ObjectType::EliteCrate)
		return ctx.m_settings.EliteCrate;
	if (networkable->GetType() == ObjectType::MedicalCrate)
		return ctx.m_settings.MedicalCrate;
	if (networkable->GetType() == ObjectType::FoodCrate)
		return ctx.m_settings.FoodCrate;
	if (networkable->GetType() == ObjectType::MineCart)
		return ctx.m_settings.MineCart;
	if (networkable->GetType() == ObjectType::ToolBox)
		return ctx.m_settings.ToolBox;
	if (networkable->GetType() == ObjectType::FuelCrate)
		return ctx.m_settings.FuelCrate;
	if (networkable->GetType() == ObjectType::UnderWaterCrate)
		return ctx.m_settings.UnderWaterCrate;
	if (networkable->GetType() == ObjectType::HelicopterCrate)
		return ctx.m_settings.HelicopterCrate;
	if (networkable->GetType() == ObjectType::LockedCrate)
		return ctx.m_settings.LockedCrate;
	if (networkable->GetType() == ObjectType::MilitaryCrate)
		return ctx.m_settings.MilitaryCrate;
	if (networkable->GetType() == ObjectType::NormalCrate)
		return ctx.m_settings.NormalCrate;
	if (networkable->GetType() == ObjectType::Corpse)
		return ctx.m_settings.Corpse;
	if (networkable->GetType() == ObjectType::SupplyDrop)
		return ctx.m_settings.SupplyDrop;
	if (networkable->GetType() == ObjectType::TechCrate)
		return ctx.m_settings.TechCrate;

	return ctx.m_settings.NormalCrate;
}

void LootableEsp()
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
	std::lock_guard<std::mutex> lock(Networkable.load()->LootableMutex);
	{
		templist = Networkable.load()->LootableList;
	}
	if (templist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}
	for (auto lootable : templist)
	{
		if (!lootable->IsValid())
			continue;
		if (lootable->GetPosition() == Vector3::Zero())
			continue;

		Lootables cfg = GetConfig(lootable);
		int distance = (int)Vector3::Distance(localplayerpos, lootable->GetPosition());
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
		Vector2 screenpos = WorldToScreen(lootable->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;

		std::string name = lootable->GetName();
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		std::string wname = cfg.Name ? name : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
	}
	currenthighestdistance = HighestDistance;
}
