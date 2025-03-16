#include "Pch.h"
#include "PlayerEsp.h"

#include "context.h"
#include "Globals.h"
#include "imgui_internal.h"
#include "visuals.h"
#include "DMALibrary/Memory/Memory.h"
#include "SDK/BasePlayer.h"
std::mutex PlayerCacheMatrix;
int LastBoneCache = 0;
inline int HighestDistance = 0;
std::shared_ptr<CheatFunction> CachePlayers = std::make_shared<CheatFunction>(2000, []()
{
//	if (BaseLocalPlayer.load() == nullptr)
	//	return;
	//printf(xorstr_("[PlayerEsp] Caching Players\n"));
	BaseLocalPlayer.load()->CachePlayerList();
	//printf(xorstr_("[PlayerEsp] Cached Players\n"));
});

void PlayerUpdateLoop()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	auto handle = mem.CreateScatterHandle();

	BaseLocalPlayer.load()->UpdatePosition(handle);
	BaseLocalPlayer.load()->SetEyeAngles(handle);

	PlayerCacheMutex.lock();
	for (std::shared_ptr<BasePlayer> player : BaseLocalPlayer.load()->PlayerList)
	{
		if(player == nullptr)
			continue;
		if (!player->IsPlayerValid())
			continue;
		if (player->IsSleeper())
			continue;
		if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
			continue;
		player->SetLastPosition(player->GetPosition());

		player->UpdatePosition(handle);
		player->SetLastPositionUpdate();
		player->SetBonePosition(handle);
		player->UpdateVisibility(handle);
		player->SetDestroyed(handle);
		player->SetVelocity(handle);
		player->SetEyeAngles(handle);
	}
	//BaseLocalPlayer.load()->SetVelocity(handle);
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);
	for (std::shared_ptr<BasePlayer> player : BaseLocalPlayer.load()->PlayerList)
	{
		if (!player->IsPlayerValid())
			continue;
		if (player->IsSleeper())
			continue;
		if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
			continue;
		if (player == nullptr)
			continue;
		player->AddNewVelocity(player->GetVelocity());
		player->SetBonePosition1();
		// printf player position
		
	}
	//BaseLocalPlayer.load()->AddNewVelocity(BaseLocalPlayer.load()->GetVelocity());
	BaseLocalPlayer.load()->AddNewVelocity(BaseLocalPlayer.load()->GetVelocity());
	PlayerCacheMutex.unlock();
}
void UpdatePlayersFrame()
{
	//if (BaseLocalPlayer.load() == nullptr)
	//	return;
	/*PlayerCacheMutex.lock();
	for (std::shared_ptr<BasePlayer> player : BaseLocalPlayer.load()->PlayerList)
	{
		player->SetBonePosition1();
	}
	
	//BaseLocalPlayer.load()->AddNewVelocity(BaseLocalPlayer.load()->GetVelocity());
	PlayerCacheMutex.unlock();*/
}
std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(8, []()
{
	PlayerUpdateLoop();
});
std::shared_ptr<CheatFunction> UpdatePlayersPerFrame = std::make_shared<CheatFunction>(1, []()
	{
		UpdatePlayersFrame();
	});
void PlayerEsp()
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
	std::vector<std::shared_ptr<BasePlayer>> templayerlist;
	std::lock_guard<std::mutex> lock(PlayerCacheMutex);
	{
		templayerlist = BaseLocalPlayer.load()->PlayerList;
	}
	if (templayerlist.size() == 0)
	{
		HighestDistance = 0;
		return;
	}

	int currenthighestdistance = 0;
	for (int i = 0; i < templayerlist.size(); i++)
	{
		std::shared_ptr<BasePlayer> player = templayerlist[i];

		Players cfg = player->GetPlayerConfig();
		if (!cfg.Enable)
			continue;
		if (!cfg.EnableInBattleMode && ctx.m_settings.Battlemode_Enable)
			continue;
		if (player == nullptr || player->IsLocalPlayer() || !player->IsPlayerValid() || player->GetClass() == BaseLocalPlayer.load()->GetClass())
			continue;
		if (player->IsSleeper() && !cfg.DrawSleepers)
			continue;
		if (player->IsDestroyed())
			continue;
		Vector3 position = player->GetPosition();
		if (position == Vector3(0, 0, 0))
			continue;
		Vector2 screenpos = WorldToScreen(position);
		if (screenpos.x == 0 && screenpos.y == 0)
			continue;
		int distance = (int)Vector3::Distance(position, localplayerpos);
		if (distance <= 0)
			continue;
		if (distance > cfg.MaxDistance)
			continue;
		
		Vector2 headpos = WorldToScreen(player->GetBonePosition(Bones::Head));
		if (distance > currenthighestdistance)
			currenthighestdistance = distance;
		headpos.y += 0.4f;
		ImColor textcolour = player->IsPlayerVisible() && !player->IsSleeper() ? cfg.TextVisibleColour : cfg.TextInvisibleColour;
		textcolour = player->IsSleeper() ? cfg.SleeperColour : textcolour;
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

		std::string weaponame = player->GetActiveItemName();
		std::string wweapon = cfg.Weapon ? weaponame : xorstr_("");
		std::string wdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
		std::string wname = cfg.Name ? player->GetName() : xorstr_("");
		if (cfg.Team && (!player->IsFriendly() && !player->GetIsNPC()))
		{
		
		}
		if (cfg.Box && !player->IsSleeper())
		{
			if (headpos != Vector2::Zero() && headpos.x != 0 && headpos.y != 0)
			{
				// print headpos
				//printf(xorstr_("HeadPos: %f %f\n"), headpos.x, headpos.y);
				float height = (headpos.y - 5) - screenpos.y;
				if (!(distance > 10 && height > 150)) // prevent a fewfuck ups
				{
					ctx.m_draw->DrawRect(ImVec2(screenpos.x - (height / 4), screenpos.y), (height / 2), height + 1, 4, ImColor(0, 0, 0, 255));
				ctx.m_draw->DrawRect(ImVec2(screenpos.x - (height / 4), screenpos.y), (height / 2), height, 2, player->IsPlayerVisible() ? cfg.BoxVisibleColour : cfg.BoxInvisibleColour);
					if (cfg.FillBox)
						ctx.m_draw->DrawFilledRect(ImVec2(screenpos.x - (height / 4), screenpos.y), (height / 2) - 1, height + 4, ImColor(30, 30, 30, 150));
				}
			}

		}
		if (cfg.Skeleton && distance >= 2)
		{
			Vector2 neckpos = WorldToScreen(player->GetBonePosition(Bones::Neck));
			Vector2 spine4pos = WorldToScreen(player->GetBonePosition(Bones::Spine4));
			Vector2 spine3pos = WorldToScreen(player->GetBonePosition(Bones::Spine3));
			Vector2 spine2pos = WorldToScreen(player->GetBonePosition(Bones::Spine2));
			Vector2 spine1pos = WorldToScreen(player->GetBonePosition(Bones::Spine1));
			Vector2 righthippos = WorldToScreen(player->GetBonePosition(Bones::RightHip));
			Vector2 lefthippos = WorldToScreen(player->GetBonePosition(Bones::LeftHip));
			Vector2 rightkneepos = WorldToScreen(player->GetBonePosition(Bones::RightKnee));
			Vector2 leftkneepos = WorldToScreen(player->GetBonePosition(Bones::LeftLnee));
			Vector2 rightfootpos = WorldToScreen(player->GetBonePosition(Bones::RightFoot));
			Vector2 leftfootpos = WorldToScreen(player->GetBonePosition(Bones::LeftFoot));
			Vector2 rightupperarmpos = WorldToScreen(player->GetBonePosition(Bones::RightUpperarm));
			Vector2 leftupperarmpos = WorldToScreen(player->GetBonePosition(Bones::LeftUpperarm));
			Vector2 rightforearmpos = WorldToScreen(player->GetBonePosition(Bones::RightForearm));
			Vector2 leftforearmpos = WorldToScreen(player->GetBonePosition(Bones::LeftForearm));
			Vector2 righthandpos = WorldToScreen(player->GetBonePosition(Bones::RightHand));
			Vector2 lefthandpos = WorldToScreen(player->GetBonePosition(Bones::LeftHand));

			if(headpos.x != 0 && headpos.y != 0 && neckpos.x != 0 && neckpos.y != 0)
				ctx.m_draw->DrawCircle(ImVec2(headpos.x, headpos.y), Vector2::Distance(headpos,neckpos), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 13, 1);
			if (neckpos.x != 0 && neckpos.y != 0 && spine4pos.x != 0 && spine4pos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(neckpos.x, neckpos.y), ImVec2(spine4pos.x, spine4pos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (spine4pos.x != 0 && spine4pos.y != 0 && spine3pos.x != 0 && spine3pos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(spine4pos.x, spine4pos.y), ImVec2(spine3pos.x, spine3pos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (spine2pos.x != 0 && spine2pos.y != 0 && spine3pos.x != 0 && spine3pos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(spine2pos.x, spine2pos.y), ImVec2(spine3pos.x, spine3pos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (spine1pos.x != 0 && spine1pos.y != 0 && spine2pos.x != 0 && spine2pos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(spine2pos.x, spine2pos.y), ImVec2(spine1pos.x, lefthippos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (lefthippos.x != 0 && lefthippos.y != 0 && righthippos.x != 0 && righthippos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(righthippos.x, righthippos.y), ImVec2(lefthippos.x, lefthippos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (righthippos.x != 0 && righthippos.y != 0 && rightkneepos.x != 0 && rightkneepos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(righthippos.x, righthippos.y), ImVec2(rightkneepos.x, rightkneepos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (lefthippos.x != 0 && lefthippos.y != 0 && leftkneepos.x != 0 && leftkneepos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(lefthippos.x, lefthippos.y), ImVec2(leftkneepos.x, leftkneepos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (rightkneepos.x != 0 && rightkneepos.y != 0 && rightfootpos.x != 0 && rightfootpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(rightkneepos.x, rightkneepos.y), ImVec2(rightfootpos.x, rightfootpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (leftkneepos.x != 0 && leftkneepos.y != 0 && leftfootpos.x != 0 && leftfootpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(leftkneepos.x, leftkneepos.y), ImVec2(leftfootpos.x, leftfootpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (spine4pos.x != 0 && spine4pos.y != 0 && rightupperarmpos.x != 0 && rightupperarmpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(spine4pos.x, spine4pos.y), ImVec2(rightupperarmpos.x, rightupperarmpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (spine4pos.x != 0 && spine4pos.y != 0 && leftupperarmpos.x != 0 && leftupperarmpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(spine4pos.x, spine4pos.y), ImVec2(leftupperarmpos.x, leftupperarmpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (rightupperarmpos.x != 0 && rightupperarmpos.y != 0 && rightforearmpos.x != 0 && rightforearmpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(rightupperarmpos.x, rightupperarmpos.y), ImVec2(rightforearmpos.x, rightforearmpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (leftupperarmpos.x != 0 && leftupperarmpos.y != 0 && leftforearmpos.x != 0 && leftforearmpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(leftupperarmpos.x, leftupperarmpos.y), ImVec2(leftforearmpos.x, leftforearmpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (rightforearmpos.x != 0 && rightforearmpos.y != 0 && righthandpos.x != 0 && righthandpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(rightforearmpos.x, rightforearmpos.y), ImVec2(righthandpos.x, righthandpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);
			if (leftforearmpos.x != 0 && leftforearmpos.y != 0 && lefthandpos.x != 0 && lefthandpos.y != 0)
				ctx.m_draw->DrawLine(ImVec2(leftforearmpos.x, leftforearmpos.y), ImVec2(lefthandpos.x, lefthandpos.y), player->IsPlayerVisible() ? cfg.SkeletonVisibleColour : cfg.SkeletonInvisibleColour, 1);

		}
		cfg.DistanceSeperateLine == false ? wname += wdistance : wname;
		ctx.m_draw->DrawString(wname, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
		ctx.m_draw->DrawString(wweapon, ImVec2(screenpos.x, screenpos.y + (textsize)), textcolour, textsize, FontAlignment::Centre);
		if (cfg.DistanceSeperateLine)
		{
			if (wweapon.size() == 0)
				wweapon = wdistance;
			else
				ctx.m_draw->DrawString(wdistance, ImVec2(screenpos.x, screenpos.y + (textsize * 2)), textcolour, textsize, FontAlignment::Centre); // this is drawn below all of them

			ctx.m_draw->DrawString(wname, ImVec2(screenpos.x, screenpos.y), textcolour, textsize, FontAlignment::Centre);
			ctx.m_draw->DrawString(wweapon, ImVec2(screenpos.x, screenpos.y + (textsize)), textcolour, textsize, FontAlignment::Centre);
			
		}

	}
	HighestDistance = currenthighestdistance;
}
