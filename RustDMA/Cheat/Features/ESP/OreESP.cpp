#include "Pch.h"
#include "Globals.h"
#include "OreESP.h"

#include "Config.h"
#include "context.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "SDK/Networkable.h"
inline int HighestDistance = 0;

Ores GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetType() == ObjectType::Stone)
		return ctx.m_settings.Stone;
	if (networkable->GetType() == ObjectType::Metal)
		return ctx.m_settings.Metal;
	return ctx.m_settings.Sulphur;
}

void OreEsp()
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
	std::lock_guard<std::mutex> lock(Networkable.load()->OreMutex);
	{
		templist = Networkable.load()->OresList;
	}
	if (templist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}
	if (localplayerpos == Vector3::Zero())
		return;
	for (auto ore : templist)
	{
		if (!ore->IsValid())
			continue;
		if (ore->GetPosition() == Vector3::Zero())
			continue;
		Ores cfg = GetConfig(ore);
		int distance = (int)Vector3::Distance(localplayerpos, ore->GetPosition());
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
		Vector2 screenpos = WorldToScreen(ore->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;

		std::string name = cfg.Name ? ore->GetName() : xorstr_("");
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		std::string wname = cfg.Name ? name : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
	}
	HighestDistance = currenthighestdistance;
}
