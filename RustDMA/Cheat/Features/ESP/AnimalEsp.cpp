#include "Pch.h"
#include "Globals.h"
#include "AnimalEsp.h"

#include "Config.h"
#include "context.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "DMALibrary/Memory/Memory.h"
#include "Misc/CheatFunction.h"
#include "SDK/Networkable.h"

inline int HighestDistance = 0;

Animals GetConfig(std::shared_ptr<NetworkableObject> networkable)
{
	if (networkable->GetType() == ObjectType::Bear)
		return ctx.m_settings.Bear;
	if (networkable->GetType() == ObjectType::PolarBear)
		return ctx.m_settings.PolarBear;
	if (networkable->GetType() == ObjectType::Boar)
		return ctx.m_settings.Boar;
	if (networkable->GetType() == ObjectType::Stag)
		return ctx.m_settings.Stag;
	if (networkable->GetType() == ObjectType::Wolf)
		return ctx.m_settings.Wolf;
	if (networkable->GetType() == ObjectType::Horse)
		return ctx.m_settings.Horse;
	if (networkable->GetType() == ObjectType::Chicken)
		return ctx.m_settings.Chicken;
	if (networkable->GetType() == ObjectType::Shark)
		return ctx.m_settings.Shark;

	return ctx.m_settings.Chicken;
}

std::shared_ptr<CheatFunction> UpdateAnimals = std::make_shared<CheatFunction>(25, []()
{
	auto handle = mem.CreateScatterHandle();
	Networkable.load()->UpdateAnimalPositions(handle);

	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);
});

void AnimalEsp()
{
	if (ctx.m_settings.Misc_DisableESP)
		return;
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (!BaseLocalPlayer.load()->IsPlayerValid())
	{
		HighestDistance = 0;
		return;
	}
	Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
	if (localplayerpos == Vector3::Zero())
		return;
	if (!Networkable.load())
		return;

	std::vector<std::shared_ptr<NetworkableObject>> templist;
	std::lock_guard<std::mutex> lock(Networkable.load()->AnimalMutex);
	{
		templist = Networkable.load()->AnimalList;
	}

	if (templist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}

	int currenthighestdistance = 0;
	for (auto animal : templist)
	{
		if (!animal->IsValid())
			continue;
		if (animal->GetPosition() == Vector3::Zero())
			continue;

		Animals cfg = GetConfig(animal);
		int distance = (int)Vector3::Distance(localplayerpos, animal->GetPosition());
		if (distance <= 0)
			continue;
		if (!cfg.Enable)
			continue;
		if (!cfg.EnableInBattleMode && ctx.m_settings.Battlemode_Enable)
			continue;
		if (distance > cfg.MaxDistance+1)
			continue;
		if (distance > currenthighestdistance)
			currenthighestdistance = distance;
		Vector2 screenpos = WorldToScreen(animal->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;
		int textsize = cfg.FontSize;
		if(ctx.m_settings.Misc_Font_Override)
			textsize = ctx.m_settings.Misc_Font_Override_Size;
		ImColor textcolour = cfg.TextColour;
		if (cfg.OpacityCulling == 1)
		{
			textcolour.Value.w = ImLerp<float>(1.0f, 0.4f, (float)distance / (float)(HighestDistance + 1));
		}
		if (cfg.TextCulling == 1)
		{
			textsize = ImLerp<float>(cfg.FontSize, cfg.FontSize - 4, (float)distance / (float)(HighestDistance + 1));
		}
		std::string name = animal->GetName();
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		std::string wname = cfg.Name ? name : xorstr_("");
		ctx.m_draw->DrawString(wname + wdistance, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
	}
	HighestDistance = currenthighestdistance;
}
