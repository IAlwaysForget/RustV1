#include "Pch.h"
#include "Globals.h"
#include "DroppedItemEsp.h"

#include "Config.h"
#include "context.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "SDK/Networkable.h"
inline int HighestDistance = 0;

DroppedItems GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetItemType() == ItemType::Other)
		return ctx.m_settings.OtherItems;
	if (networkable->GetItemType() == ItemType::Food)
		return ctx.m_settings.FoodItems;
	if (networkable->GetItemType() == ItemType::Medical)
		return ctx.m_settings.MedicalItems;
	if (networkable->GetItemType() == ItemType::Ammo)
		return ctx.m_settings.AmmoItems;
	if (networkable->GetItemType() == ItemType::Explosives)
		return ctx.m_settings.ExplosiveItems;
	if (networkable->GetItemType() == ItemType::Resources)
		return ctx.m_settings.ResourceItems;
	if (networkable->GetItemType() == ItemType::Armour)
		return ctx.m_settings.ArmourItems;
	if (networkable->GetItemType() == ItemType::Tool)
		return ctx.m_settings.ToolItems;
	if (networkable->GetItemType() == ItemType::Gun)
		return ctx.m_settings.GunItems;
	if (networkable->GetItemType() == ItemType::Components)
		return ctx.m_settings.ComponentsItems;
	return ctx.m_settings.OtherItems;
}

void DroppedItemEsp()
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
	std::lock_guard<std::mutex> lock(Networkable.load()->DroppedItemsMutex);
	{
		templist = Networkable.load()->DroppedItems;
	}

	if (templist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}
	int currenthighestdistance = 0;

	if (localplayerpos == Vector3::Zero())
		return;
	for (auto itm : templist)
	{
		if (!itm->IsValid())
			continue;
		if (itm->GetPosition() == Vector3::Zero())
			continue;
		if (itm->GetItemType() == ItemType::None)
			continue;
		DroppedItems cfg = GetConfig(itm);
		int distance = (int)Vector3::Distance(localplayerpos, itm->GetPosition()) ;
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
		Vector2 screenpos = WorldToScreen(itm->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;
		std::string name = itm->GetItemName();
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		std::string wname = cfg.Name ? name : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
	}
	HighestDistance = currenthighestdistance;
}
