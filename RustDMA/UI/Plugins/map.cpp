#include "map.h"

#include "imgui_internal.h"
#include "GUI.h"
#include "Fuser/fuser.h"
#include "context.h"
#include "Globals.h"
#include <Font/newIcons.h>
#include "Cheat/Misc/Checksum.h"
namespace Map
{
	bool ReloadMap = true;
	bool ReloadCorruptedMap = false;
	ID3D11ShaderResourceView* mapTexture;
	ID3D11ShaderResourceView* mapPlayerMarker;
	int width, height, playerWidth, playerHeight;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> MapIcons;
	ImVec2 mapOffset(0, 0);
	std::atomic<float> mapZoom = 1.0f;
	//Woods map info temporarily here
	MapData mapInfo = { "", 1.0f, ImVec2(width / 2, height / 2) };
	std::atomic<bool> CentreRadar = true;

	void CreateMapDirectory()
	{
		try
		{

			std::filesystem::create_directory(std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents\\"))).string());
			std::filesystem::create_directory(std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents\\Moe\\"))).string());
				std::filesystem::create_directory(std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents\\Moe\\Maps"))).string());
				std::filesystem::create_directory(std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents\\Moe\\Assets"))).string());
				std::filesystem::create_directory(std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents\\Moe\\Maps\\Rust"))).string());

			
		}
		catch (std::exception ex)
		{
			printf("----------------------------------------\n");
						printf(xorstr_("Error creating map directory: %s\n"), ex.what());
		
		}
	}
	ImVec2 GetMapPosition(Vector3 worldPos)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 WorldCenterPosition = ImVec2(pos.x + mapInfo.Offset.x * mapZoom.load(), pos.y + mapInfo.Offset.y * mapZoom.load()) + mapOffset;
		// Adjusted to correctly map the X and Z (assuming Z is up in your 3D world) to your 2D view without rotation.
		return ImVec2(WorldCenterPosition.x + worldPos.x * mapInfo.Scale * mapZoom.load(), WorldCenterPosition.y - worldPos.z * mapInfo.Scale * mapZoom.load());
	}

	void DrawMap(ImTextureID mapTexture, float width, float height)
	{
		auto drawList = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = ImVec2(width, height) * mapZoom.load();
		drawList->AddImage(mapTexture, ImVec2((pos.x + 1.f) + mapOffset.x, pos.y + mapOffset.y), pos + size + mapOffset);
	}
	Vector3 Normalized(Vector3 vec)
	{
		float num = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		if (num > FLT_EPSILON)
		{
			return { vec.x / num, vec.y / num, vec.z / num };
		}
		return Vector3{ };
	}

	inline Vector2 CalcAngle(Vector3 source, Vector3 destination) // Works Good 02 11 22
	{
		Vector3 delta = destination - source;
		delta = Normalized(delta);

		float len = delta.Length();

		Vector2 ret;
		ret.y = asinf(delta.y / len) * (float)(180.f / 3.14159265358979323846f);
		ret.x = -atan2f(delta.x, -delta.z) * (float)(180.f / 3.14159265358979323846f);

		return ret;
	}

	bool IsLookingAt(std::shared_ptr<BasePlayer> player, std::shared_ptr<BasePlayer> enemy, float toleranceDegrees)
	{
		Vector2 TargetAngle = CalcAngle(player->GetPosition(), enemy->GetPosition());
		Vector2 CurrentAngle = TargetAngle - enemy->GetBodyRotation();

		// Normalize angles to the range [-180, 180]
		CurrentAngle.x = fmod(CurrentAngle.x + 540.0f, 360.0f) - 180.0f;
		CurrentAngle.y = fmod(CurrentAngle.y + 540.0f, 360.0f) - 180.0f;

		// Check if within tolerance
		if (abs(CurrentAngle.x) < toleranceDegrees && abs(CurrentAngle.y) < toleranceDegrees)
			return true;
		return false;
	}
	bool was_dragging = false;
	void DrawAnimatedRings(const ImVec2& center, int numRings, float initialRadius, float radiusIncrement, ImU32 color, float animationSpeed)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		float time = ImGui::GetTime();

		float baseRadius = initialRadius + sinf(time * animationSpeed) * radiusIncrement;

		for (int i = 0; i < numRings; ++i)
		{
			float currentRadius = baseRadius + (i * radiusIncrement);
			if (currentRadius < 0) currentRadius = -currentRadius;
			draw_list->AddCircle(center, currentRadius, color, 0, 2.0f); // Thickness added for 
		}
	}
	int rotation_start_index;

	void ImRotateStart()
	{
		rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
	}

	ImVec2 ImRotationCenter()
	{
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

		const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotation_start_index; i < buf.Size; i++)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
	}

	void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
	{
		float s = sin(rad), c = cos(rad);
		center = ImRotate(center, s, c) - center;

		auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
		for (int i = rotation_start_index; i < buf.Size; i++)
			buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
	}
	void DrawPlayerIcon(ImDrawList* DrawList, std::shared_ptr<BasePlayer>& player, ImVec2 Pos,RadarPlayers cfg)
	{
		// print player->GetBodyRotation()
		if(player == nullptr)
			return;
		if (BaseLocalPlayer.load() == nullptr)
			return;
		Vector4 rotation;
		if(player->GetClass() == BaseLocalPlayer.load()->GetClass())
			rotation = BaseLocalPlayer.load()->GetEyeAngles();
		else
			rotation = player->GetEyeAngles();
		float yaw = (atan2(-2 * (rotation.x * rotation.z - rotation.w * rotation.y), rotation.w * rotation.w - rotation.x * rotation.x - rotation.y * rotation.y + rotation.z * rotation.z)) * (180 / 3.14159265358979323846f);
		float deg = yaw - 90;
		ImColor PlayerColor = player->GetClass() == BaseLocalPlayer.load()->GetClass() ? cfg.LocalPlayerColour : player->IsSleeper() ? cfg.SleeperColour : player->IsPlayerVisible() ? cfg.VisibleDotColour : cfg.InvisibleDotColour;

	//	if (player.IsLocked || Game.Aimbot->LockedPlayer.Pointers.Address == player.Pointers.Address)
	//		DrawAnimatedRings(Pos, 3, iconScale * 10, 5, PlayerColor, 1.8f);
		const float iconScale = cfg.Scale;
		float innerCircleRadius = iconScale * 20 * Map::mapZoom.load();

	//	if (!player.TeamID.empty())
	//		DrawList->AddCircle(Pos, innerCircleRadius, GetOrCreateTeamColor(player.TeamID), innerCircleRadius + 4, 3.0f);

		DrawList->AddCircle(Pos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
		DrawList->AddCircleFilled(Pos, innerCircleRadius, PlayerColor);

		ImVec2 trianglePoints[3] = {
			ImVec2(Pos.x - iconScale * 12 * Map::mapZoom.load(), Pos.y - iconScale * 32 * Map::mapZoom.load()),
			ImVec2(Pos.x + iconScale * 12 * Map::mapZoom.load(), Pos.y - iconScale * 32 * Map::mapZoom.load()),
			ImVec2(Pos.x, Pos.y - iconScale * 52 * Map::mapZoom.load())
		};
		if (player->GetClass() != BaseLocalPlayer.load()->GetClass())
		{
		ImRotateStart();
		{
			ImVec2 arc_center_adjusted = (trianglePoints[0] + trianglePoints[1]) / 2;
			arc_center_adjusted.y += iconScale * 25.0f * Map::mapZoom.load();
			float start_angle = atan2(trianglePoints[0].y - arc_center_adjusted.y, trianglePoints[0].x - arc_center_adjusted.x);
			float end_angle = atan2(trianglePoints[1].y - arc_center_adjusted.y, trianglePoints[1].x - arc_center_adjusted.x);
			DrawList->PathLineTo(trianglePoints[2]);
			DrawList->PathArcTo(arc_center_adjusted, iconScale * 25.0f * Map::mapZoom.load(), start_angle, end_angle);
			DrawList->PathFillConvex(PlayerColor);
			DrawList->PathStroke(PlayerColor, false, 3.0f);
		}
		ImRotateEnd(-(deg * 3.14159265358979323846f / 180.0f), Pos);

		if (ImGui::IsMouseHoveringRect(ImVec2(Pos.x - innerCircleRadius, Pos.y - innerCircleRadius), ImVec2(Pos.x + innerCircleRadius, Pos.y + innerCircleRadius)))
		{
			
				DrawList->AddCircleFilled(Pos, innerCircleRadius, ImColor(255, 255, 255, 64));
			//	FloatingSection::BeginSection(xorstr_("Radar Player Info"), true, Pos, ImVec2(500, 500), false, ImVec2(1, 31), false);
				
			//	FloatingSection::EndSection();
			//if (ImGui::IsMouseClicked(0))
			//{
			//	if (Map::FollowLocal)
			//		Map::FollowLocal = false;
			//	SelectedFollowPlayer = player;
			//}
			//if (ImGui::IsMouseClicked(0)) {
			//	FollowingPlayer = !FollowingPlayer;
			//	FollowPlayer = player;
			//}
		}
		}
	

		//if (ctx.m_settings.map_viewangle_type == 0)
		//{

		if (player->GetClass() != BaseLocalPlayer.load()->GetClass())
		{
			float fovRadius = 10 * Map::mapZoom.load();
			DrawList->PathArcTo(Pos, fovRadius, (deg - 45) * 3.14159265358979323846f / 180.0f, (deg + 45) * 3.14159265358979323846f / 180.0f, 20);
			DrawList->PathStroke(ImColor(255, 255, 255, 64), false, 1.0f); // FOV
		}
		else
		{
			float directionLength = 25 * Map::mapZoom.load();
					ImVec2 directionEnd = ImVec2(Pos.x + cos(deg * 3.14159265358979323846f / 180.0f) * directionLength, Pos.y + sin(deg * 3.14159265358979323846f / 180.0f) * directionLength);
					DrawList->AddLine(Pos, directionEnd, ImColor(0, 0, 0, 255), 1.0f);
		}
			//	DrawList->PathFillConvex(ImColor(255, 255, 255, 64));
		//}
	//	else if (ctx.m_settings.map_viewangle_type == 1)
	//	{
	//		float directionLength = ctx.m_settings.map_viewdirection_length * Map::mapZoom.load();
	//		ImVec2 directionEnd = ImVec2(Pos.x + cos(deg * M_PI / 180.0f) * directionLength, Pos.y + sin(deg * M_PI / 180.0f) * directionLength);
	//		DrawList->AddLine(Pos, directionEnd, ImColor(255, 255, 255, 64), 1.0f);
	//	}

	/*	if (player.Local)
			for (auto& Enemy : Game.PlayerCache->GetPlayers())
			{
				if (Enemy.Position.Distance(player.Position) > 400) continue;
				if (IsLookingAt(Game.PlayerCache->LocalPlayer, Enemy, 14))
				{
					ImVec2 startPos = Pos;
					ImVec2 endPos = Map::GetMapPosition(Enemy.Position);

					// Draw a 50% transparent red line
					float lineThickness = 2.0f; // Line thickness
					DrawList->AddLine(startPos, endPos, ImColor(255, 0, 0, 128), lineThickness);

					static float timeOffset = 0.0f;
					float currentTime = ImGui::GetTime();
					float animationSpeed = 0.5f;

					float normalizedTime = fmod(currentTime + timeOffset, animationSpeed) / animationSpeed;

					ImVec2 lineVector = ImVec2(endPos.x - startPos.x, endPos.y - startPos.y);
					float lineLength = sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);

					ImVec2 normalizedDirection = ImVec2(lineVector.x / lineLength, lineVector.y / lineLength);

					float rectLength = 20.0f;

					float travelDistance = normalizedTime * (lineLength + rectLength) - rectLength;
					ImVec2 rectStart = ImVec2(startPos.x + normalizedDirection.x * travelDistance,
						startPos.y + normalizedDirection.y * travelDistance);

					ImVec2 rectEnd = ImVec2(rectStart.x + normalizedDirection.x * rectLength,
						rectStart.y + normalizedDirection.y * rectLength);

					DrawList->PathLineTo(rectStart);
					DrawList->PathLineTo(ImVec2(rectEnd.x, rectEnd.y));
					DrawList->PathStroke(ImColor(255, 255, 255, 128), false, lineThickness);
				}
			}*/
	}
	void HandleMapInput(ImTextureID mapTexture, float mapWidth, float mapHeight)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (ImGui::IsMouseHoveringRect(pos, pos + size) && ImGui::IsWindowFocused())
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			ImVec2 mouseOffset = ImVec2((mousePos.x - pos.x - mapOffset.x) / (mapWidth * mapZoom.load()), (mousePos.y - pos.y - mapOffset.y) / (mapHeight * mapZoom.load()));
			float newZoom = mapZoom.load() * (1.0f + io.MouseWheel * 0.1f);

			mapOffset.x -= (mapWidth * newZoom * mouseOffset.x) - (mapWidth * mapZoom.load() * mouseOffset.x);
			mapOffset.y -= (mapHeight * newZoom * mouseOffset.y) - (mapHeight * mapZoom.load() * mouseOffset.y);

			mapZoom.store(newZoom);

			// Dragging
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
				was_dragging = true;
				mapOffset.x += io.MouseDelta.x;
				mapOffset.y += io.MouseDelta.y;
			}
			else if (was_dragging)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
				was_dragging = false;
				CentreRadar.store(false);
			}
			
		}

		DrawMap(mapTexture, mapWidth, mapHeight);
	}

	//goto points, are a vector of world positions to go to. this is added because then we can call the function 'Goto'
	//everywhere we want, without having the issue of ImGetCursorScreenPos() not being relative from the map, messing up the calculations on where to go to.
	std::vector<Vector3> gotoPoints;

	void Goto(Vector3 WorldPos)
	{
		gotoPoints.push_back(WorldPos);
	}

	void HandleGoTo()
	{
		if (gotoPoints.empty())
			return;
		Vector3 WorldPos = gotoPoints[0];
		ImVec2 pos = ImGui::GetCursorScreenPos();

		ImVec2 targetScreenPos = ImVec2(
			pos.x + (WorldPos.x * mapInfo.Scale * mapZoom.load()) + (mapInfo.Offset.x * mapZoom.load()) + mapOffset.x,
			pos.y - (WorldPos.z * mapInfo.Scale * mapZoom.load()) + (mapInfo.Offset.y * mapZoom.load()) + mapOffset.y
		);

		ImVec2 screenCenter = ImVec2(pos.x + ImGui::GetContentRegionAvail().x / 2, pos.y + ImGui::GetContentRegionAvail().y / 2);
		ImVec2 diff = screenCenter - targetScreenPos;
		mapOffset = ImLerp(mapOffset, mapOffset + diff, 0.1);
		// printf ImLerp(mapOffset, mapOffset + diff, 0.1);
		//printf("MapOffset: %f %f\n", mapOffset.x, mapOffset.y);
		if (std::abs(diff.x) < 1 && std::abs(diff.y) < 1)
			gotoPoints.erase(gotoPoints.begin());
	}
	void DrawString(std::string text, ImVec2 pos, ImColor color, int fontSize, FontAlignment alignment)
	{
		auto draw_list = ImGui::GetWindowDrawList();
		if (fontSize == 0)
			fontSize = ImGui::GetFontSize();
		ImFont* font = Font::overlayFont;
		if (ctx.m_settings.Overlay_BoldFont)
			font = Font::overlayFontBold;

		ImVec2 txtSize = ImGui::CustomCalcTextSize(text.c_str(), 0, false, -1, fontSize, font);
		// set the position
		switch (alignment)
		{
		case FontAlignment::Centre:
			pos.x -= (txtSize.x / 2);
			break;
		case FontAlignment::Right:
			pos.x += txtSize.x;
			break;
		case FontAlignment::Left:
			pos.x -= txtSize.x;
			break;
		case FontAlignment::None:
			break;
		case FontAlignment::CentreCentre:
			pos.x -= (txtSize.x / 2);
			pos.y -= (txtSize.y / 2);
			break;
		case FontAlignment::CentreLeft:
			pos.x -= txtSize.x;
			pos.y += (txtSize.y / 2);
			break;
		case FontAlignment::CentreRight:
			pos.x += txtSize.x;
			pos.y -= (txtSize.y / 2);
			break;
		}

		draw_list->AddText(font, fontSize, pos, color, text.c_str());
	}
	void DrawPlayers()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<BasePlayer>> playerlist;
		PlayerCacheMutex.lock();
		playerlist = BaseLocalPlayer.load()->PlayerList;
		PlayerCacheMutex.unlock();
		for (std::shared_ptr<BasePlayer> player : playerlist)
		{


			if (player == nullptr)
				continue;
			if (player->IsLocalPlayer())
				continue;
			if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
				continue;
			if (player->IsDestroyed())
				continue;
			if (player->GetPosition() == Vector3::Zero())
				continue;
			if (!player->GetPlayerRadarConfig().Enable)
				continue;
			bool above = false;
			ImVec2 playerPos = GetMapPosition(player->GetPosition());
			RadarPlayers cfg = player->GetPlayerRadarConfig();
			if (!cfg.Enable)
				continue;
			int distance = (int)Vector3::Distance(player->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			if (!player->GetIsNPC())
				continue;
			if(player->GetPosition().y > localplayerpos.y)
				above = true;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 20 * Map::mapZoom.load();
			int difference = above ? (int)(player->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - player->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (cfg.IconType == 2)
			{
				
					if (LoadItemIconFromZip(xorstr_("ScientistAvatar")).texture != nullptr)
					{
						drawList->AddImage(LoadItemIconFromZip(xorstr_("ScientistAvatar")).texture, ImVec2(playerPos.x - innerCircleRadius, playerPos.y - innerCircleRadius), ImVec2(playerPos.x + innerCircleRadius, playerPos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));

					}
				
			}
			std::string weaponame = player->GetActiveItemName();
			std::string sweapon = cfg.Weapon ? weaponame : xorstr_("");
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? player->GetName() : xorstr_("");

			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			if (cfg.IconType == 1)
				DrawPlayerIcon(drawList, player, playerPos, cfg);

			DrawString(sname + sdistance + ydistance, ImVec2(playerPos.x, playerPos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
			DrawString(sweapon, ImVec2(playerPos.x, playerPos.y + innerCircleRadius + (cfg.FontSize) + (iconScale * 8 * Map::mapZoom.load())), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);


		}
		// lazy way to ensure that players are always drawn at the topmost layer
		for (std::shared_ptr<BasePlayer> player : playerlist)
		{


			if (player == nullptr)
				continue;
			if (player->IsLocalPlayer())
				continue;
			if (player->GetClass() == BaseLocalPlayer.load()->GetClass())
				continue;
			if (player->IsDestroyed())
				continue;
			if (player->GetPosition() == Vector3::Zero())
				continue;
			if (player->GetIsNPC())
				continue;

			ImVec2 playerPos = GetMapPosition(player->GetPosition());
			RadarPlayers cfg = player->GetPlayerRadarConfig();
			if(!cfg.DrawSleepers && player->IsSleeper())
				continue;
				if (!cfg.Enable)
				continue;
			int distance = (int)Vector3::Distance(player->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			bool above = false;
			if (player->GetPosition().y > localplayerpos.y)
				above = true;
			std::string weaponame = player->GetActiveItemName();
			std::string sweapon = cfg.Weapon ? weaponame : xorstr_("");
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? player->GetName() : xorstr_("");
			int difference = above ? (int)(player->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - player->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : ""; 
			DrawPlayerIcon(drawList, player, playerPos, cfg);
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 20 * Map::mapZoom.load();
			DrawString(sname + sdistance + ydistance, ImVec2(playerPos.x, playerPos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
			DrawString(sweapon, ImVec2(playerPos.x, playerPos.y + innerCircleRadius + (cfg.FontSize) + (iconScale * 8 * Map::mapZoom.load())), cfg.TextColour,  cfg.FontSize , FontAlignment::CentreCentre);
		}
		ImVec2 localplayermappos = GetMapPosition(BaseLocalPlayer.load()->GetPosition());
		std::shared_ptr<BasePlayer> localplayer = BaseLocalPlayer.load();
		DrawPlayerIcon(drawList, localplayer, localplayermappos, ctx.m_settings.PlayerRadar);


	}
	void DrawAnimals()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> animallist;
		Networkable.load()->AnimalMutex.lock();
		animallist = Networkable.load()->AnimalList;
		Networkable.load()->AnimalMutex.unlock();
	
		for (std::shared_ptr<NetworkableObject> animal : animallist)
		{
			if (animal == nullptr)
				continue;
			if (animal->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 animalpos = GetMapPosition(animal->GetPosition());
			int distance = (int)Vector3::Distance(animal->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			RadarAnimals cfg;
			bool above = false;
			if (animal->GetPosition().y > localplayerpos.y)
				above = true;
			if (animal->GetType() == ObjectType::Bear)
			{
				cfg =  ctx.m_settings.RadarBear;
				if (!cfg.Enable)
					continue;
				
			}
			if (animal->GetType() == ObjectType::PolarBear)
			{
				cfg =  ctx.m_settings.RadarPolarBear;
				if (!cfg.Enable)
					continue;
			
			}
			if (animal->GetType() == ObjectType::Boar)
			{
				cfg = ctx.m_settings.RadarBoar;
				if (!cfg.Enable)
					continue;
				
			}
			if (animal->GetType() == ObjectType::Stag)
			{
				cfg = ctx.m_settings.RadarStag;
				if (!cfg.Enable)
					continue;
				
			}
			if (animal->GetType() == ObjectType::Wolf)
			{
				cfg = ctx.m_settings.RadarWolf;
				if (!cfg.Enable)
					continue;
				
			}
			if (animal->GetType() == ObjectType::Horse)
			{
				cfg = ctx.m_settings.RadarHorse;
				if (!cfg.Enable)
					continue;
			
			}
			if (animal->GetType() == ObjectType::Chicken)
			{
				cfg = ctx.m_settings.RadarChicken;
				if (!cfg.Enable)
					continue;
				
			}
			if (animal->GetType() == ObjectType::Shark)
			{
				cfg = ctx.m_settings.RadarShark;
				if (!cfg.Enable)
					continue;
				
			}
			int difference = above ? (int)(animal->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - animal->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{
				if (animal->GetPrefabName() != "")
				{
						if (LoadItemIconFromZip(animal->GetPrefabName()).texture != nullptr)
						{
							drawList->AddImage(LoadItemIconFromZip(animal->GetPrefabName()).texture, ImVec2(animalpos.x - innerCircleRadius, animalpos.y - innerCircleRadius), ImVec2(animalpos.x + innerCircleRadius, animalpos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));
						}
				
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(animalpos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(animalpos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? animal->GetName() : xorstr_("");

			std::string ydistance =  difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";

			if (cfg.IconType == 1)
				drawList->AddCircleFilled(animalpos, innerCircleRadius, cfg.DotColour);
		
			DrawString(sname + sdistance + ydistance, ImVec2(animalpos.x, animalpos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}
		
	}
	void DrawVehicles()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> vehiclelist;
		Networkable.load()->VehicleMutex.lock();
		vehiclelist = Networkable.load()->VehicleList;
		Networkable.load()->VehicleMutex.unlock();

		for (std::shared_ptr<NetworkableObject> vehicle : vehiclelist)
		{
			if (vehicle == nullptr)
				continue;
			if (vehicle->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 vehiclepos = GetMapPosition(vehicle->GetPosition());
			int distance = (int)Vector3::Distance(vehicle->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			RadarVehicles cfg;
			bool above = false;
			if (vehicle->GetPosition().y > localplayerpos.y)
				above = true;
			if (vehicle->GetType() == ObjectType::Car)
			{
				cfg = ctx.m_settings.RadarCar;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::MiniCopter)
			{
				cfg = ctx.m_settings.RadarMiniCopter;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::AttackHelicopter)
			{
				cfg = ctx.m_settings.RadarPatrolHelicopter;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::AttackMiniCopter)
			{
				cfg = ctx.m_settings.RadarAttackMiniCopter;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::ScrapHeli)
			{
				cfg = ctx.m_settings.RadarScrapHeli;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::Bradley)
			{
				cfg = ctx.m_settings.RadarBradley;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::RHIB)
			{
				cfg = ctx.m_settings.RadarRHIB;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::PedalBike)
			{
				cfg = ctx.m_settings.RadarPedalbike;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::Motorbike)
			{
				cfg = ctx.m_settings.RadarMotorbike;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::MotorbikeSidecar)
			{
				cfg = ctx.m_settings.RadarMotorbikeSidecar;
				if (!cfg.Enable)
					continue;

			}

			if (vehicle->GetType() == ObjectType::RowBoat)
			{
				cfg = ctx.m_settings.RadarRowBoat;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::Train)
			{
				cfg = ctx.m_settings.RadarTrain;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::SnowMobile)
			{
				cfg = ctx.m_settings.RadarSnowMobile;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::SmallSubmarine)
			{
				cfg = ctx.m_settings.RadarSmallSubmarine;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::LargeSubmarine)
			{
				cfg = ctx.m_settings.RadarLargeSubmarine;
				if (!cfg.Enable)
					continue;

			}
			if (vehicle->GetType() == ObjectType::TugBoat)
			{
				cfg = ctx.m_settings.RadarTugBoat;
				if (!cfg.Enable)
					continue;

			}
			int difference = above ? (int)(vehicle->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - vehicle->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{
				if (vehicle->GetPrefabName() != "")
				{
					if (LoadItemIconFromZip(vehicle->GetPrefabName()).texture != nullptr)
					{
						drawList->AddImage(LoadItemIconFromZip(vehicle->GetPrefabName()).texture, ImVec2(vehiclepos.x - innerCircleRadius, vehiclepos.y - innerCircleRadius), ImVec2(vehiclepos.x + innerCircleRadius, vehiclepos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));

					}
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(vehiclepos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(vehiclepos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? vehicle->GetName() : xorstr_("");


			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			DrawString(sname + sdistance + ydistance, ImVec2(vehiclepos.x, vehiclepos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}

	}

	void DrawDeployable()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> deployablelist;
		Networkable.load()->DeployableMutex.lock();
		deployablelist = Networkable.load()->DeployableList;
		Networkable.load()->DeployableMutex.unlock();

		for (std::shared_ptr<NetworkableObject> deployable : deployablelist)
		{
			if (deployable == nullptr)
				continue;
			if (deployable->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 deployablepos = GetMapPosition(deployable->GetPosition());
			int distance = (int)Vector3::Distance(deployable->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			RadarDeployables cfg;
			bool above = false;
			if (deployable->GetPosition().y > localplayerpos.y)
				above = true;
			if (deployable->GetType() == ObjectType::ToolCupboard)
			{
				cfg = ctx.m_settings.RadarToolCupboard;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Stash)
			{
				cfg = ctx.m_settings.RadarStash;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Furnace)
			{
				cfg = ctx.m_settings.RadarFurnace;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::LargeFurnace)
			{
				cfg = ctx.m_settings.RadarFurnace;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::LargeBox)
			{
				cfg = ctx.m_settings.RadarLargeBox;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::FlameTurret)
			{
				cfg = ctx.m_settings.RadarFlameTurret;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::SamSite)
			{
				cfg = ctx.m_settings.RadarSamSite;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::AutoTurret)
			{
				cfg = ctx.m_settings.RadarAutoTurret;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::ShotgunTrap)
			{
				cfg = ctx.m_settings.RadarShotgunTrap;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::LandMine)
			{
				cfg = ctx.m_settings.RadarLandMine;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::BearTrap)
			{
				cfg = ctx.m_settings.RadarBearTrap;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Battery)
			{
				cfg = ctx.m_settings.RadarBattery;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::SleepingBag)
			{
				cfg = ctx.m_settings.RadarSleepingBag;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Workbench1)
			{
				cfg = ctx.m_settings.RadarWorkbench;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Workbench2)
			{
				cfg = ctx.m_settings.RadarWorkbench;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Workbench3)
			{
				cfg = ctx.m_settings.RadarWorkbench;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::OilRefinery)
			{
				cfg = ctx.m_settings.RadarOilRefinery;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Planter)
			{
				cfg = ctx.m_settings.RadarPlanter;
				if (!cfg.Enable)
					continue;

			}
			if (deployable->GetType() == ObjectType::Recycler)
			{
				cfg = ctx.m_settings.RadarRecycler;
				if (!cfg.Enable)
					continue;
			}
			int difference = above ? (int)(deployable->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - deployable->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{
				if (deployable->GetPrefabName() != "")
				{
					if (LoadItemIconFromZip(deployable->GetPrefabName()).texture != nullptr)
					{
							drawList->AddImage(LoadItemIconFromZip(deployable->GetPrefabName()).texture, ImVec2(deployablepos.x - innerCircleRadius, deployablepos.y - innerCircleRadius), ImVec2(deployablepos.x + innerCircleRadius, deployablepos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));
						}
					
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(deployablepos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(deployablepos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? deployable->GetName() : xorstr_("");
			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			DrawString(sname + sdistance + ydistance, ImVec2(deployablepos.x, deployablepos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}

	}
	void DrawOres()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> orelist;
		Networkable.load()->OreMutex.lock();
		orelist = Networkable.load()->OresList;
		Networkable.load()->OreMutex.unlock();

		for (std::shared_ptr<NetworkableObject> ore : orelist)
		{
			if (ore == nullptr)
				continue;
			if (ore->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 orepos = GetMapPosition(ore->GetPosition());
			int distance = (int)Vector3::Distance(ore->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			RadarOres cfg;
			bool above = false;
			if (ore->GetPosition().y > localplayerpos.y)
				above = true;
			if (ore->GetType() == ObjectType::Stone)
			{
				cfg = ctx.m_settings.RadarStone;
				if (!cfg.Enable)
					continue;

			}
			if (ore->GetType() == ObjectType::Metal)
			{
				cfg = ctx.m_settings.RadarMetal;
				if (!cfg.Enable)
					continue;

			}
			if (ore->GetType() == ObjectType::Sulphur)
			{
				cfg = ctx.m_settings.RadarSulphur;
				if (!cfg.Enable)
					continue;

			}
			int difference = above ? (int)(ore->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - ore->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{

				if (ore->GetPrefabName() != "")
				{
					if (LoadItemIconFromZip(ore->GetPrefabName()).texture != nullptr)
					{
							drawList->AddImage(LoadItemIconFromZip(ore->GetPrefabName()).texture, ImVec2(orepos.x - innerCircleRadius, orepos.y - innerCircleRadius), ImVec2(orepos.x + innerCircleRadius, orepos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));
						}
					
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(orepos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(orepos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? ore->GetName() : xorstr_("");
			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			DrawString(sname + sdistance + ydistance, ImVec2(orepos.x, orepos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}
	}
	void DrawCollectables()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> collectablelist;
		Networkable.load()->CollectableMutex.lock();
		collectablelist = Networkable.load()->CollectableList;
		Networkable.load()->CollectableMutex.unlock();

		for (std::shared_ptr<NetworkableObject> collectable : collectablelist)
		{
			if (collectable == nullptr)
				continue;
			if (collectable->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 collectablepos = GetMapPosition(collectable->GetPosition());
			int distance = (int)Vector3::Distance(collectable->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;

			RadarCollectables cfg;
			bool above = false;
			if (collectable->GetPosition().y > localplayerpos.y)
				above = true;

			if (collectable->GetType() == ObjectType::StoneCollectable)
			{
				cfg = ctx.m_settings.RadarStoneCollectable;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::MetalCollectable)
			{
				cfg = ctx.m_settings.RadarMetalCollectable;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::SulphurCollectable)
			{
				cfg = ctx.m_settings.RadarSulphurCollectable;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Wood)
			{
				cfg = ctx.m_settings.RadarWood;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Hemp)
			{
				cfg = ctx.m_settings.RadarHemp;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Pumpkin)
			{
				cfg = ctx.m_settings.RadarPumpkin;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Corn)
			{
				cfg = ctx.m_settings.RadarCorn;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Mushroom)
			{
				cfg = ctx.m_settings.RadarMushroom;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Potato)
			{
				cfg = ctx.m_settings.RadarPotato;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::BlackBerry)
			{
				cfg = ctx.m_settings.RadarBlackBerry;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::BlueBerry)
			{
				cfg = ctx.m_settings.RadarBlueBerry;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::RedBerry)
			{
				cfg = ctx.m_settings.RadarRedBerry;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::YellowBerry)
			{
				cfg = ctx.m_settings.RadarYellowBerry;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::GreenBerry)
			{
				cfg = ctx.m_settings.RadarGreenBerry;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::WhiteBerry)
			{
				cfg = ctx.m_settings.RadarWhiteBerry;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::DetectorSource)
			{
				cfg = ctx.m_settings.RadarMetalDetector;
				if (!cfg.Enable)
					continue;
			}
			if (collectable->GetType() == ObjectType::Diesel)
			{
				cfg = ctx.m_settings.RadarDiesel;
				if (!cfg.Enable)
					continue;
			}
			if(distance > cfg.MaxDistance)
				continue;
			int difference = above ? (int)(collectable->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - collectable->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{
				if (collectable->GetPrefabName() != "")
				{
					if (LoadItemIconFromZip(collectable->GetPrefabName()).texture != nullptr)
					{
							drawList->AddImage(LoadItemIconFromZip(collectable->GetPrefabName()).texture, ImVec2(collectablepos.x - innerCircleRadius, collectablepos.y - innerCircleRadius), ImVec2(collectablepos.x + innerCircleRadius, collectablepos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));
						}
					
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(collectablepos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(collectablepos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? collectable->GetName() : xorstr_("");

			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			DrawString(sname + sdistance + ydistance, ImVec2(collectablepos.x, collectablepos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}
	}
	void DrawLootable()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> lootablelist;
		Networkable.load()->LootableMutex.lock();
		lootablelist = Networkable.load()->LootableList;
		Networkable.load()->LootableMutex.unlock();

		for (std::shared_ptr<NetworkableObject> lootable : lootablelist)
		{
			if (lootable == nullptr)
				continue;
			if (lootable->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 lootablepos = GetMapPosition(lootable->GetPosition());
			int distance = (int)Vector3::Distance(lootable->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			RadarLootables cfg;
			bool above = false;
			if (lootable->GetPosition().y > localplayerpos.y)
				above = true;
			if (lootable->GetType() == ObjectType::Barrel)
			{
				cfg = ctx.m_settings.RadarBarrel;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::FuelBarrel)
			{
				cfg = ctx.m_settings.RadarFuelBarrel;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::AmmoCrate)
			{
				cfg = ctx.m_settings.RadarAmmoCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::BradleyCrate)
			{
				cfg = ctx.m_settings.RadarBradleyCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::EliteCrate)
			{
				cfg = ctx.m_settings.RadarEliteCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::MedicalCrate)
			{
				cfg = ctx.m_settings.RadarMedicalCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::FoodCrate)
			{
				cfg = ctx.m_settings.RadarFoodCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::MineCart)
			{
				cfg = ctx.m_settings.RadarMineCart;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::ToolBox)
			{
				cfg = ctx.m_settings.RadarToolBox;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::FuelCrate)
			{
				cfg = ctx.m_settings.RadarFuelCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::UnderWaterCrate)
			{
				cfg = ctx.m_settings.RadarUnderWaterCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::HelicopterCrate)
			{
				cfg = ctx.m_settings.RadarHelicopterCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::LockedCrate)
			{
				cfg = ctx.m_settings.RadarLockedCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::MilitaryCrate)
			{
				cfg = ctx.m_settings.RadarMilitaryCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::NormalCrate)
			{
				cfg = ctx.m_settings.RadarNormalCrate;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::Corpse)
			{
				cfg = ctx.m_settings.RadarCorpse;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::SupplyDrop)
			{
				cfg = ctx.m_settings.RadarSupplyDrop;
				if (!cfg.Enable)
					continue;
			}
			if (lootable->GetType() == ObjectType::TechCrate)
			{
				cfg = ctx.m_settings.RadarTechCrate;
				if (!cfg.Enable)
					continue;
			}
			
			int difference = above ? (int)(lootable->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - lootable->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{
				if (lootable->GetPrefabName() != "")
				{
					if (LoadItemIconFromZip(lootable->GetPrefabName()).texture != nullptr)
					{
							drawList->AddImage(LoadItemIconFromZip(lootable->GetPrefabName()).texture, ImVec2(lootablepos.x - innerCircleRadius, lootablepos.y - innerCircleRadius), ImVec2(lootablepos.x + innerCircleRadius, lootablepos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));
						
					}
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(lootablepos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(lootablepos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? lootable->GetName() : xorstr_("");

			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			DrawString(sname + sdistance + ydistance, ImVec2(lootablepos.x, lootablepos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}
	}
	void DrawDroppedItems()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		if (Networkable.load() == nullptr)
			return;
		Vector3 localplayerpos = BaseLocalPlayer.load() == nullptr ? Vector3::Zero() : BaseLocalPlayer.load()->GetPosition();
		std::vector<std::shared_ptr<NetworkableObject>> droppeditemlist;
		Networkable.load()->DroppedItemsMutex.lock();
		droppeditemlist = Networkable.load()->DroppedItems;
		Networkable.load()->DroppedItemsMutex.unlock();

		for (std::shared_ptr<NetworkableObject> droppeditem : droppeditemlist)
		{
			if (droppeditem == nullptr)
				continue;
			if (droppeditem->GetPosition() == Vector3::Zero())
				continue;
			ImVec2 itempos = GetMapPosition(droppeditem->GetPosition());
			int distance = (int)Vector3::Distance(droppeditem->GetPosition(), localplayerpos);
			if (distance <= 0)
				continue;
			RadarDroppedItems cfg;
			bool above = false;
			if (droppeditem->GetPosition().y > localplayerpos.y)
				above = true;

			if (droppeditem->GetItemType() == ItemType::Other)
			{
				cfg = ctx.m_settings.RadarOtherItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Food)
			{
				cfg = ctx.m_settings.RadarFoodItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Medical)
			{
				cfg = ctx.m_settings.RadarMedicalItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Ammo)
			{
				cfg = ctx.m_settings.RadarAmmoItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Explosives)
			{
				cfg = ctx.m_settings.RadarExplosiveItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Resources)
			{
				cfg = ctx.m_settings.RadarResourceItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Armour)
			{
				cfg = ctx.m_settings.RadarArmourItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Tool)
			{
				cfg = ctx.m_settings.RadarToolItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Gun)
			{
				cfg = ctx.m_settings.RadarGunItems;
				if (!cfg.Enable)
					continue;
			}
			if (droppeditem->GetItemType() == ItemType::Components)
			{
				cfg = ctx.m_settings.RadarComponentsItems;
				if (!cfg.Enable)
					continue;
			}
			int difference = above ? (int)(droppeditem->GetPosition().y - localplayerpos.y) : (int)(localplayerpos.y - droppeditem->GetPosition().y);
			if (difference > cfg.MaxYDistance)
				continue;
			if (distance > cfg.MaxDistance)
				continue;
			const float iconScale = cfg.Scale;
			float innerCircleRadius = iconScale * 15 * Map::mapZoom.load();
			if (cfg.IconType == 2)
			{
				if (droppeditem->GetPrefabName() != "")
				{
					if (LoadItemIconFromZip(droppeditem->GetPrefabName()).texture != nullptr)
					{
							drawList->AddImage(LoadItemIconFromZip(droppeditem->GetPrefabName()).texture, ImVec2(itempos.x - innerCircleRadius, itempos.y - innerCircleRadius), ImVec2(itempos.x + innerCircleRadius, itempos.y + innerCircleRadius), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 180));
						
					}
				}
			}
			if (cfg.IconType == 1)
			{
				drawList->AddCircle(itempos, innerCircleRadius + 1, ImColor(0, 0, 0, 255), 12, 1.0f);
				drawList->AddCircleFilled(itempos, innerCircleRadius, cfg.DotColour);
			}
			std::string sdistance = cfg.Distance ? xorstr_("[") + std::to_string(distance) + xorstr_("m]") : xorstr_("");
			std::string sname = cfg.Name ? droppeditem->GetItemName() : xorstr_("");
			std::string ydistance = difference > 10 ? (cfg.YDistance ? (above ? xorstr_("(+") + std::to_string(difference) + xorstr_(")") : "(-" + std::to_string(difference) + xorstr_(")")) : "") : "";
			DrawString(sname + sdistance + ydistance, ImVec2(itempos.x, itempos.y + (innerCircleRadius + (iconScale * 8 * Map::mapZoom.load()))), cfg.TextColour, cfg.FontSize, FontAlignment::CentreCentre);
		}
	}
	void Map()
	{
			/*std::string MapNameCur = Game.CurrentGameStatus.Location;
			if (CurrentLevel != -1)
				MapNameCur += std::to_string(CurrentLevel + 1);

			ImageData CMapTexture = TarkovAPI::LoadMapImageFromZip(MapNameCur);
			mapTexture = CMapTexture.texture;
			width = CMapTexture.width;
			height = CMapTexture.height;

			std::vector<MapData> MapData = Maps[Game.CurrentGameStatus.Location];
			for (auto& map : MapData)
			{
				if (map.Name == MapNameCur)
				{
					mapInfo = map;

					break;
				}
			}*/
		if (MapWorld.load() == nullptr || !MapWorld.load()->IsValid())
			return;
		if (ReloadMap)
		{
			if (mapTexture)
				mapTexture->Release();
			std::string filename = xorstr_("map_") + std::to_string(MapWorld.load()->GetSize()) + xorstr_("_") + std::to_string(MapWorld.load()->GetSeed()) + xorstr_(".png");
			std::string map = std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents/Moe/Maps/Rust/") + filename)).string();

			if (fuser.LoadTextureFromFile(map.c_str(), &mapTexture, &width, &height))
			{
				
				mapInfo = { "", 1.0f, ImVec2(width / 2, height / 2) };
			}
			ReloadMap = false;
		}
		if (ReloadCorruptedMap)
		{
			if (mapTexture)
				mapTexture->Release();
			printf(xorstr_("Reloading Corrupted Map\n"));
			std::string filename = xorstr_("c:/map.png");

			if (fuser.LoadTextureFromFile(filename.c_str(), &mapTexture, &width, &height))
			{

				mapInfo = { "", 1.0f, ImVec2(width / 2, height / 2) };
			}
			ReloadCorruptedMap = false;
		}
		if (BaseLocalPlayer.load() == nullptr || (Networkable.load()->GetListSize() == 0))
			return;

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				int mapsize = mapTexture ? width :(int)(((int)MapWorld.load()->GetSize()+1000 ));
				HandleMapInput(mapTexture, mapsize, mapsize);
				HandleGoTo();
				DrawOres();
				DrawCollectables();
				DrawLootable();
				DrawDroppedItems();
				DrawAnimals();
				DrawVehicles();
				DrawDeployable();

				if (BaseLocalPlayer.load() != nullptr && BaseLocalPlayer.load()->GetPosition() != Vector3::Zero())
				{
					if (CentreRadar.load() == true)
					{
						gotoPoints.clear();
						gotoPoints.push_back(BaseLocalPlayer.load()->GetPosition());
					}
					DrawPlayers();

				}
			

			

		

	
	}

	void RenderMap()
	{
		if (ctx.m_settings.Radar_Enable)
		{
			FloatingSection::BeginSection(xorstr_("map"), true, &ctx.m_settings.Visuals_Misc_MapPos, ImVec2(500, 500), true, ImVec2(1, 31), true);
			Map();
			FloatingSection::EndSection();
		}
	}
} // namespace Map
