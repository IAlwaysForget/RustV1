#include "Pch.h"
#include "Globals.h"
#include "CollectableEsp.h"

#include "Config.h"
#include "context.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "SDK/Networkable.h"
inline int HighestDistance = 0;

Collectables GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetType() == ObjectType::StoneCollectable)
		return ctx.m_settings.StoneCollectable;
	if (networkable->GetType() == ObjectType::MetalCollectable)
		return ctx.m_settings.MetalCollectable;
	if (networkable->GetType() == ObjectType::SulphurCollectable)
		return ctx.m_settings.SulphurCollectable;
	if (networkable->GetType() == ObjectType::Hemp)
		return ctx.m_settings.Hemp;
	if (networkable->GetType() == ObjectType::Pumpkin)
		return ctx.m_settings.Pumpkin;
	if (networkable->GetType() == ObjectType::Corn)
		return ctx.m_settings.Corn;
	if (networkable->GetType() == ObjectType::Mushroom)
		return ctx.m_settings.Mushroom;
	if (networkable->GetType() == ObjectType::Potato)
		return ctx.m_settings.Potato;
	if (networkable->GetType() == ObjectType::RedBerry)
		return ctx.m_settings.RedBerry;
	if (networkable->GetType() == ObjectType::WhiteBerry)
		return ctx.m_settings.WhiteBerry;
	if (networkable->GetType() == ObjectType::BlackBerry)
		return ctx.m_settings.BlackBerry;
	if (networkable->GetType() == ObjectType::YellowBerry)
		return ctx.m_settings.YellowBerry;
	if (networkable->GetType() == ObjectType::BlueBerry)
		return ctx.m_settings.BlueBerry;
	if (networkable->GetType() == ObjectType::GreenBerry)
		return ctx.m_settings.GreenBerry;
	if (networkable->GetType() == ObjectType::Diesel)
		return ctx.m_settings.Diesel;
	if (networkable->GetType() == ObjectType::DetectorSource)
		return ctx.m_settings.MetalDetector;

	return ctx.m_settings.Wood;
}

void CollectableEsp()
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
	std::lock_guard<std::mutex> lock(Networkable.load()->CollectableMutex);
	{
		templist = Networkable.load()->CollectableList;
	}
	if (templist.size() == 0)
		return;

	for (auto collectable : templist)
	{
		if (!collectable->IsValid())
			continue;
		if (collectable->GetPosition() == Vector3::Zero())
			continue;
		Collectables cfg = GetConfig(collectable);
		if (!cfg.EnableInBattleMode && ctx.m_settings.Battlemode_Enable)
			continue;
		int distance = (int)Vector3::Distance(localplayerpos, collectable->GetPosition()) ;
		if (distance <= 0)
			continue;
		if (!cfg.Enable)
			continue;
		if (distance > cfg.MaxDistance)
			continue;
		if (distance > currenthighestdistance)
			currenthighestdistance = distance;

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
		Vector2 screenpos = WorldToScreen(collectable->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;
		std::string name = collectable->GetName();
		std::string wname = cfg.Name ? name : xorstr_("");
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
		//DrawText(screenpos.x, screenpos.y, wname + wdistance, ConfigFonts[ctx.m_settings.Visuals_Misc_Font], textsize, textcolour, FontAlignment::Centre);
	}
	HighestDistance = currenthighestdistance;
}
