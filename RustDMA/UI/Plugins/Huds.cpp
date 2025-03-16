#include "Huds.h"

#include "context.h"
#include "Globals.h"
#include "GUI.h"
#include "Features/Misc Features/VisualFeatures.h"

namespace HUDS
{
	void DrawHud(std::string name, ImVec2* inputpos, std::vector<std::string> data)
	{
		if (data.empty())
			return;

		// Calculate the size of the largest text element
		ImVec2 size = ImGui::CalcTextSize(data[0].c_str());
		for (size_t i = 1; i < data.size(); i++)
		{
			ImVec2 tmp = ImGui::CalcTextSize(data[i].c_str());
			if (tmp.x > size.x)
				size.x = tmp.x;
		}

		ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());
		if (nameSize.x > size.x)
			size.x = nameSize.x;

		float fontSize = ImGui::GetFontSize();
		size.x += 20.f;

		size.y = ((data.size() * fontSize) + (data.size() * 3)) + 48.f;

		FloatingSection::BeginSection(name, true, inputpos, size, false, ImVec2(1, 31), false, !ctx.m_settings.Visuals_Misc_HudBackground, true);

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 13.f));

		for (size_t i = 0; i < data.size(); i++)
		{
			ImColor color = ctx.m_settings.Visuals_Misc_Hud_TextColour;
			if (i == 0)
				color = ImColor(94, 255, 176, 255);
			FloatingSection::Text(data[i], color);
		}

		FloatingSection::EndSection();
	}

	void Run()
	{
		if (BaseLocalPlayer.load() != nullptr && Networkable.load() != nullptr && (BaseLocalPlayer.load()->GetListSize() != 0 || Networkable.load()->GetListSize() != 0))
		{
			if (ctx.m_settings.Visuals_Misc_TargetHotbar)
			{
				TargetHotbar = GetTargetHotbar();
				DrawHud(xorstr_("Target Hotbar"), &ctx.m_settings.Visuals_Misc_TargetHotbarPos, TargetHotbar);
			}
			if (ctx.m_settings.Visuals_Misc_TargetTeam)
			{
				TargetTeam = GetTargetTeam();
				DrawHud(xorstr_("Target Team"), &ctx.m_settings.Visuals_Misc_TargetTeamPos, TargetTeam);
			}
			if (ctx.m_settings.Visuals_Misc_TargetClothes)
			{
				TargetClothes = GetTargetClothes();
				DrawHud(xorstr_("Target Clothes"), &ctx.m_settings.Visuals_Misc_TargetClothesPos, TargetClothes);
			}
			if (ctx.m_settings.Visuals_Misc_WorldHud)
			{
				WorldHudItems = GetWorldHud();
				DrawHud(xorstr_("World Hud"), &ctx.m_settings.Visuals_Misc_WorldHudPos, WorldHudItems);
			}
		}
	}
}
