#pragma once
#include "hash.h"
#include "fileIO.h"
#include <vector>
#include <string>

#include "Draw.h"
#include "imgui.h"
#include "xorstr.hpp"

struct string_setting_t
{
	char text[128];
};

class node_t
{
public:
	virtual ~node_t() = default;

	virtual void add(node_t* node_ptr) = 0;

	virtual void load_setting(std::string& config_str) = 0;
	virtual void save_setting(std::string& config_str) = 0;
	//virtual bool set_value(std::string& feature) = 0;
};

class holder_t : public node_t
{
public:
	holder_t(const char* name) : m_name(name)
	{
	}

	holder_t(holder_t* holder_ptr, const char* name) : m_name(name) { holder_ptr->add(this); }

	void load_setting(std::string& node) override
	{
	}

	void save_setting(std::string& node) override
	{
	}

	//bool set_value(std::string& feature) override { return false; }

	void add(node_t* node_ptr) override;

	void load(const char* path, const char* file);

	void load(std::string& config);

	void save(std::string& config);

	void save(const char* path, const char* file);

	const char* m_name;
	std::vector<node_t*> m_nodes;
	std::string tempConfig;
};

class ISetting : public node_t
{
	void add(node_t* node) override
	{
	}
};

template <typename T>
class setting_t : ISetting
{
public:
	__forceinline setting_t(holder_t* holder_ptr, uint64_t name) : m_name(name) { holder_ptr->add(this); }

	__forceinline setting_t(holder_t* holder_ptr, uint64_t name, const T& rhs) : m_old_value(rhs), m_value(rhs), m_default(rhs), m_name(name) { holder_ptr->add(this); }

	bool changed()
	{
		uint8_t* data1 = reinterpret_cast<uint8_t*>(&m_value);
		uint8_t* data2 = reinterpret_cast<uint8_t*>(&m_old_value);

		for (size_t i = 0; i < sizeof(m_value); i++)
		{
			if (data1[i] != data2[i])
			{
				m_old_value = m_value;
				return true; // Objects are not equal
			}
		}
		return false;
	}

	void load_setting(std::string& config_str) override
	{
		m_value = m_default;

		uint8_t* data = (uint8_t*)&m_value;

		auto pos = config_str.find(std::to_string(m_name));
		if (pos == std::string::npos)
			return;

		pos += std::to_string(m_name).length() + 1;
		const char* buffer = config_str.data() + pos;
		for (size_t i = 0; i < sizeof(m_value); i++)
		{
			unsigned temp;
			sscanf(&buffer[2 * i], "%02X", &temp);
			data[i] = temp; //Set Value
		}
	}

	void save_setting(std::string& config_str) override
	{
		config_str.append(std::to_string(m_name) + xorstr_(":"));

		uint8_t* data = (uint8_t*)&m_value;

		char* buffer = new char[sizeof(m_value) * 2 + 1];
		for (size_t i = 0; i < sizeof(m_value); i++)
			sprintf(&buffer[2 * i], "%02X", data[i]);

		config_str.append(std::string(buffer) + xorstr_(";"));
		delete[] buffer;
	}

	setting_t& operator=(const T& rhs)
	{
		return *this;
	}

	__forceinline operator T&()
	{
		return m_value;
	}

	__forceinline operator const T&() const
	{
		return m_value;
	}

	__forceinline T* operator &()
	{
		return &m_value;
	}

	__forceinline const T* operator &() const
	{
		return &m_value;
	}

	__forceinline T& operator ()()
	{
		return m_value;
	}

	__forceinline const T& operator ()() const
	{
		return m_value;
	}

	bool operator!=(const T& rhs) const
	{
		return m_value != rhs;
	}

private:
	T m_value;
	T m_old_value;
	T m_default;
	uint64_t m_name;
};

template <typename t>
class settingtype_t
{
public:
	settingtype_t(const t& value = { })
	{
		for (size_t i = 0; i < 1; ++i)
			m_data[i] = value;
	}

	t m_data[1];

	t& operator( )()
	{
		return m_data[0];
	}

	t& operator[](const size_t& iter)
	{
		return m_data[iter];
	}
};

// fuck it!
struct settingstring_t
{
	char buffer[128];

	settingstring_t()
	{
		clear();
	}

	settingstring_t(const char* text)
	{
		set(text);
	}

	__forceinline void set(std::string& str)
	{
		set(str.c_str());
	}

	__forceinline void set(const char* text)
	{
		clear();
		strncpy_s(buffer, 128, text, 128);
	}

	__forceinline std::string get() const
	{
		return buffer;
	}

	__forceinline bool empty() const
	{
		return buffer[0] == 0;
	}

	__forceinline void clear()
	{
		memset(buffer, 0, 128);
	}

	__forceinline const char* c_str()
	{
		return buffer;
	}
};

struct Deployables
{
	Deployables() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 200;
	int FontSize = 13;
	bool EnableInBattleMode = false;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(81, 255, 0, 255);
};
struct RadarDeployables
{
	RadarDeployables() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(81, 255, 0, 255);
	ImColor DotColour = ImColor(81, 255, 0, 255);
};
struct Animals
{
	Animals() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 200;
	int FontSize = 13;
	bool EnableInBattleMode = false;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(180, 0, 255, 255);
};
struct RadarAnimals
{
	RadarAnimals() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(180, 0, 255, 255);
	ImColor DotColour = ImColor(180, 0, 255, 255);
};
struct Lootables
{
	Lootables() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 200;
	int FontSize = 13;
	bool EnableInBattleMode = false;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(255, 0, 150, 255);
};
struct RadarLootables
{
	RadarLootables() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(255, 0, 150, 255);
	ImColor DotColour = ImColor(255, 0, 150, 255);
};
struct Vehicles
{
	Vehicles() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 200;
	int FontSize = 13;
	bool EnableInBattleMode = true;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(200, 255, 0, 255);
};
struct RadarVehicles
{
	RadarVehicles() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(200, 255, 0, 255);
	ImColor DotColour = ImColor(200, 255, 0, 255);
};
struct DroppedItems
{
	DroppedItems() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 200;
	int FontSize = 13;
	bool EnableInBattleMode = false;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(0, 0, 255, 255);
};
struct RadarDroppedItems
{
	RadarDroppedItems() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(0, 0, 255, 255);
	ImColor DotColour = ImColor(0, 0, 255, 255);
};
struct Collectables
{
	Collectables() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 100;
	int FontSize = 13;
	bool EnableInBattleMode = false;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(255, 230, 0, 255);
};
struct RadarCollectables
{
	RadarCollectables() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(255, 230, 0, 255);
	ImColor DotColour = ImColor(255, 230, 0, 255);
};
struct Ores
{
	Ores() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	int MaxDistance = 200;
	int FontSize = 13;
	bool EnableInBattleMode = false;
	int OpacityCulling = 0;
	int TextCulling = 0;
	ImColor TextColour = ImColor(255, 255, 255, 255);
};
struct RadarOres
{
	RadarOres() = default;
	bool Enable = false;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	ImColor TextColour = ImColor(255, 255, 255, 255);
	ImColor DotColour = ImColor(255, 255, 255, 255);
};
struct Players
{
	Players() = default;
	bool Enable = true;
	bool Distance = true;
	bool Name = true;
	bool Weapon = true;
	int MaxDistance = 2000;
	int FontSize = 13;
	bool EnableInBattleMode = true;
	int OpacityCulling = 0;
	bool Box = true;
	bool FillBox = false;
	int TextCulling = 0;
	bool DrawSleepers = true;
	bool Skeleton = false;
	ImColor TextVisibleColour = ImColor(255, 255, 255, 255);
	ImColor TextInvisibleColour = ImColor(255, 0, 0, 255);
	ImColor BoxVisibleColour = ImColor(0, 150, 255, 255);
	ImColor BoxInvisibleColour = ImColor(0, 150, 255, 120);
	ImColor SleeperColour = ImColor(150, 255, 0, 255);
	ImColor SkeletonVisibleColour = ImColor(255, 255, 255, 255);
	ImColor SkeletonInvisibleColour = ImColor(255, 0, 0, 255);
	bool DistanceSeperateLine = false; // hubba wants this
	bool ShowTools = false;
	bool Team = true;
};

struct RadarPlayers
{
	RadarPlayers() = default;
	bool Enable = true;
	bool Distance = true;
	bool Name = true;
	bool YDistance = true;
	bool Weapon = true;
	int MaxDistance = 2000;
	int MaxYDistance = 100;
	int FontSize = 11;
	int IconType = 1; // none, dot, icon
	float Scale = 0.35f;
	bool DrawSleepers = true;
	ImColor TextColour = ImColor(255, 255, 255, 255);
	ImColor VisibleDotColour = ImColor(255, 0, 0, 255);
	ImColor InvisibleDotColour = ImColor(255, 0, 0, 255);
	ImColor SleeperColour = ImColor(150, 255, 0, 255);
	ImColor LocalPlayerColour = ImColor(0, 255, 0, 255);

};

class c_settings
{
public:
	bool tooltips = false;
	holder_t m_holder {""};

	setting_t<bool> menu_menu_key {&m_holder, fnvc("menu_menu_key"), true};
	setting_t<int> menu_menu_key_hotkey {&m_holder, fnvc("menu_menu_key_hotkey"), VK_INSERT};
	setting_t<ImVec2> menu_size {&m_holder, fnvc("menu_size"), ImVec2(500, 500)};

	// RADAR
	setting_t<bool> Radar_Enable{ &m_holder, fnvc("Radar_Enable"), false };
	setting_t<int> Radar_Centre_Key{ &m_holder, fnvc("Radar_Centre_Key"), 0x4E };
	setting_t<int> Radar_Centre_ZoomIn{ &m_holder, fnvc("Radar_Centre_ZoomIn"), VK_ADD };
	setting_t<int> Radar_Centre_ZoomOut{ &m_holder, fnvc("Radar_Centre_ZoomOut"), VK_SUBTRACT };
	//VISUALS MISC
	setting_t<bool> Visuals_Misc_AdminEsp {&m_holder, fnvc("Visuals_Misc_AdminEsp"), false};
	setting_t<bool> Visuals_Misc_ChangeTime {&m_holder, fnvc("Visuals_Misc_ChangeTime"), false};
	setting_t<int> Visuals_Misc_Time {&m_holder, fnvc("Visuals_Misc_Time"), 12};
	setting_t<bool> Visuals_Misc_ChangeFOV {&m_holder, fnvc("Visuals_Misc_ChangeFOV"), false};
	setting_t<int> Visuals_Misc_FOV {&m_holder, fnvc("Visuals_Misc_FOV"), 90};
	setting_t<bool> Visuals_Misc_ClearWaterVision {&m_holder, fnvc("Visuals_Misc_ClearWaterVision"), false};
	setting_t<bool> Visuals_Misc_BrightNight {&m_holder, fnvc("Visuals_Misc_BrightNight"), false};
	setting_t<bool> Visuals_Misc_BrightCave {&m_holder, fnvc("Visuals_Misc_BrightCave"), false};
	setting_t<bool> Visuals_Misc_ForceAdminFlag{ &m_holder, fnvc("Visuals_Misc_ForceAdminFlag"), false };
	setting_t<bool> Visuals_Misc_ADSBlocker{ &m_holder, fnvc("Visuals_Misc_ADSBlocker"), false };
	setting_t<bool> Visuals_Misc_Crosshair{ &m_holder, fnvc("Visuals_Misc_Crosshair"), false };
	setting_t<int> Visuals_Misc_Crosshair_Size{ &m_holder, fnvc("Visuals_Misc_Crosshair_Size"), 2 };
	setting_t<ImColor> Visuals_Misc_Crosshair_Colour{ &m_holder, fnvc("Visuals_Misc_Crosshair_Colour"),  ImColor(255, 255, 255, 255) };
	setting_t<bool> Visuals_Misc_Zoom {&m_holder, fnvc("Visuals_Misc_Zoom"), false};
	setting_t<int> Visuals_Misc_ZoomKey {&m_holder, fnvc("Visuals_Misc_ZoomKey"), VK_RBUTTON};
	setting_t<int> Visuals_Misc_ZoomAmount {&m_holder, fnvc("Visuals_Misc_ZoomAmount"), 40};
	setting_t<bool> Visuals_Misc_TargetHotbar {&m_holder, fnvc("Visuals_Misc_TargetHotbar"), true};
	setting_t<ImVec2> Visuals_Misc_TargetHotbarPos {&m_holder, fnvc("Visuals_Misc_TargetHotbar_Pos"), ImVec2(150, 50)};
	setting_t<bool> Visuals_Misc_TargetTeam {&m_holder, fnvc("Visuals_Misc_TargetTeam"), true};
	setting_t<ImVec2> Visuals_Misc_TargetTeamPos {&m_holder, fnvc("Visuals_Misc_TargetTeam_Pos"), ImVec2(100, 50)};
	setting_t<bool> Visuals_Misc_TargetClothes {&m_holder, fnvc("Visuals_Misc_TargetClothes"), true};
	setting_t<ImVec2> Visuals_Misc_TargetClothesPos {&m_holder, fnvc("Visuals_Misc_TargetClothes_Pos"), ImVec2(50, 50)};
	setting_t<int> Visuals_Misc_Font {&m_holder, fnvc("Visuals_Misc_Font"), 2};
	setting_t<bool> Visuals_Misc_WorldHud {&m_holder, fnvc("Visuals_Misc_WorldHud"), true};
	setting_t<ImVec2> Visuals_Misc_WorldHudPos {&m_holder, fnvc("Visuals_Misc_WorldHud_Pos"), ImVec2(10, 50)};
	setting_t<int> Visuals_Misc_CapsuleKey {&m_holder, fnvc("Visuals_Misc_CapsuleKey"), 0x46};
	setting_t<bool> Visuals_Misc_HudBackground{ &m_holder, fnvc("Visuals_Misc_HudBackground"), false };
	setting_t<ImColor> Visuals_Misc_Hud_TextColour{ &m_holder, fnvc("Visuals_Misc_Hud_TextColour"),  ImColor(255, 255, 255, 255) };
	setting_t<ImColor> Visuals_Misc_Hud_BackgroundColour{ &m_holder, fnvc("Visuals_Misc_Hud_BackgroundColour"),  ImColor(10, 10, 10, 255) };

	setting_t<ImVec2> Visuals_Misc_MapPos{ &m_holder, fnvc("Visuals_Misc_MapPos"), ImVec2(500, 500) };
	setting_t<ImVec2> Visuals_Misc_GUIPos{ &m_holder, fnvc("Visuals_Misc_GUIPos"), ImVec2(0, 0)};

	//WEAPON MISC
	setting_t<bool> Weapon_Misc_NoRecoil {&m_holder, fnvc("Weapon_Misc_NoRecoil"), false};
	setting_t<int> Weapon_Misc_RecoilAmountx {&m_holder, fnvc("Weapon_Misc_RecoilAmountx"), 30};
	setting_t<int> Weapon_Misc_RecoilAmounty {&m_holder, fnvc("Weapon_Misc_RecoilAmounty"), 60};
	setting_t<bool> Weapon_Misc_NoRecoilDisabler{ &m_holder, fnvc("Weapon_Misc_NoRecoilDisabler"), false };
	setting_t<int> Weapon_Misc_NoRecoilDisablerKey{ &m_holder, fnvc("Weapon_Misc_NoRecoilDisablerKey"), 0 };
	setting_t<bool> Weapon_Misc_NoSway {&m_holder, fnvc("Weapon_Misc_NoSway"), false};
	setting_t<int> Weapon_Misc_SwayAmount {&m_holder, fnvc("Weapon_Misc_SwayAmount"), 50};
	setting_t<bool> Weapon_Misc_NoAimCone {&m_holder, fnvc("Weapon_Misc_NoAimCone"), false};
	setting_t<int> Weapon_Misc_AimConeAmount {&m_holder, fnvc("Weapon_Misc_AimConeAmount"), 100};
	setting_t<bool> Weapon_Misc_ForceAutomatic {&m_holder, fnvc("Weapon_Misc_ForceAutomatic"), false};
	setting_t<bool> Weapon_Misc_InstantEoka {&m_holder, fnvc("Weapon_Misc_InstantEoka"), false};
	setting_t<bool> Weapon_Misc_InstantCompound {&m_holder, fnvc("Weapon_Misc_InstantCompound"), false};
	setting_t<bool> Weapon_Misc_AttackDistance {&m_holder, fnvc("Weapon_Misc_AttackDistance"), false};
	setting_t<bool> Weapon_Misc_AttackableSprinting {&m_holder, fnvc("Weapon_Misc_AttackableSprinting"), false};
	setting_t<bool> Weapon_Misc_InstantHammer{ &m_holder, fnvc("Weapon_Misc_InstantHammer"), false };
	setting_t<bool> Weapon_Misc_FreeRotation{ &m_holder, fnvc("Weapon_Misc_FreeRotation"), false };
	setting_t<float> Weapon_Misc_FreeRotation_Degrees{ &m_holder, fnvc("Weapon_Misc_FreeRotation_Degrees"), 0.01f };
	//MOVEMENT MISC
	setting_t<bool> Movement_Misc_Spiderman {&m_holder, fnvc("Movement_Misc_Spiderman"), false};
	setting_t<bool> Movement_Misc_NoFallDamage{ &m_holder, fnvc("Movement_Misc_NoFallDamage"), false };
	setting_t<bool> Movement_Misc_AlwaysShoot {&m_holder, fnvc("Movement_Misc_AlwaysShoot"), false};
	setting_t<bool> Movement_Misc_CapsuleBypass {&m_holder, fnvc("Movement_Misc_CapsuleBypass"), false};
	setting_t<bool> Movement_Misc_FlyHack{ &m_holder, fnvc("Movement_Misc_FlyHack"), false };
	setting_t<bool> Movement_Misc_FlyHack_NoLadder{ &m_holder, fnvc("Movement_Misc_FlyHack_NoLadder"), false };
	setting_t<int> Movement_Misc_FlyHack_Key{ &m_holder, fnvc("Movement_Misc_FlyHack_Key"), 70 };
	setting_t<bool> Movement_Misc_SpeedHack{ &m_holder, fnvc("Movement_Misc_SpeedHack"), false };
	setting_t<bool> Movement_Misc_SpeedHack_UseKey{ &m_holder, fnvc("Movement_Misc_SpeedHack_UseKey"), false };
	setting_t<int> Movement_Misc_SpeedHack_Speed{ &m_holder, fnvc("Movement_Misc_SpeedHack_Speed"), 5 };
	setting_t<int> Movement_Misc_SpeedHack_Key{ &m_holder, fnvc("Movement_Misc_SpeedHack_Key"), VK_LSHIFT };
	setting_t<bool> Movement_Misc_NoSprintDelays {&m_holder, fnvc("Movement_Misc_NoSprintDelays"), false};
	setting_t<bool> Movement_Misc_InfiniteJump {&m_holder, fnvc("Movement_Misc_InfiniteJump"), false};
	setting_t<bool> Movement_Misc_GravityModifier {&m_holder, fnvc("Movement_Misc_GravityModifier"), false};
	setting_t<float> Movement_Misc_GravityModifierAmount {&m_holder, fnvc("Movement_Misc_GravityModifierAmount"), 2.5f};
	setting_t<bool> Movement_Misc_FakeLag {&m_holder, fnvc("Movement_Misc_FakeLag"), false};
	setting_t<int> Movement_Misc_FakeLagDelay {&m_holder, fnvc("Movement_Misc_FakeLagDelay"), 300};
	setting_t<bool> Movement_Misc_InteractiveDebug {&m_holder, fnvc("Movement_Misc_InteractiveDebug"), false};
	setting_t<int> Movement_Misc_DebugRight {&m_holder, fnvc("Movement_Misc_DebugRight"), VK_RIGHT};
	setting_t<int> Movement_Misc_DebugLeft {&m_holder, fnvc("Movement_Misc_DebugLeft"), VK_LEFT};
	setting_t<int> Movement_Misc_DebugForward {&m_holder, fnvc("Movement_Misc_DebugForward"), VK_UP};
	setting_t<int> Movement_Misc_DebugBackwards {&m_holder, fnvc("Movement_Misc_DebugBackwards"), VK_DOWN};
	setting_t<int> Movement_Misc_DebugUp {&m_holder, fnvc("Movement_Misc_DebugUp"), VK_RSHIFT};
	setting_t<int> Movement_Misc_DebugDown {&m_holder, fnvc("Movement_Misc_DebugDown"), VK_RCONTROL};
	setting_t<int> Movement_Misc_DebugCamKey {&m_holder, fnvc("Movement_Misc_DebugCamKey"), 90};
	setting_t<bool> Movement_Misc_OmniSprint {&m_holder, fnvc("Movement_Misc_OmniSprint"), false};
	setting_t<bool> Movement_Misc_Manipulation {&m_holder, fnvc("Movement_Misc_Manipulation"), false};
	setting_t<int> Movement_Misc_ManipulationKey {&m_holder, fnvc("Movement_Misc_ManipulationKey"), 67};
	setting_t<int> Movement_Misc_IncreaseManipulation {&m_holder, fnvc("Movement_Misc_IncreaseManipulation"), VK_RIGHT};
	setting_t<int> Movement_Misc_DecreaseManipulation {&m_holder, fnvc("Movement_Misc_DecreaseManipulation"), VK_LEFT};
	setting_t<bool> Movement_Misc_AntiAim{ &m_holder, fnvc("Movement_Misc_AntiAim"), false };
	setting_t<int> Movement_Misc_AntiAim_Hotkey{ &m_holder, fnvc("Movement_Misc_AntiAim_Hotkey"), 70 };
	setting_t<int> Movement_Misc_AntiAim_Mode{ &m_holder, fnvc("Movement_Misc_AntiAim_Mode"), 0 };
	setting_t<int> Movement_Misc_AntiAim_Right{ &m_holder, fnvc("Movement_Misc_AntiAim_Right"), VK_RIGHT };
	setting_t<int> Movement_Misc_AntiAim_Left{ &m_holder, fnvc("Movement_Misc_AntiAim_Left"), VK_LEFT };
	setting_t<int> Movement_Misc_AntiAim_Back{ &m_holder, fnvc("Movement_Misc_AntiAim_Back"), VK_DOWN };
	setting_t<ImColor> Movement_Misc_AntiAim_Indicator_Colour{ &m_holder, fnvc("Movement_Misc_AntiAim_Indicator_Colour"), ImColor(255, 0, 0, 255) };
	//SKINS
	setting_t<bool> Skins_Enable {&m_holder, fnvc("Skins_Enable"), false};
	setting_t<int> Skins_AkSkin {&m_holder, fnvc("Skins_AkSkin"), 0};
	setting_t<int> Skins_BoltSkin {&m_holder, fnvc("Skins_BoltSkin"), 0};
	setting_t<int> Skins_M249Skin {&m_holder, fnvc("Skins_M249Skin"), 0};
	setting_t<int> Skins_LR300Skin {&m_holder, fnvc("Skins_LR300Skin"), 0};
	setting_t<int> Skins_MP5Skin {&m_holder, fnvc("Skins_MP5Skin"), 0};
	setting_t<int> Skins_CustomSMGSkin {&m_holder, fnvc("Skins_CustomSMGSkin"), 0};
	setting_t<int> Skins_ThompsonSkin {&m_holder, fnvc("Skins_ThompsonSkin"), 0};
	setting_t<int> Skins_SemiAutoRifleSkin {&m_holder, fnvc("Skins_SemiAutoRifleSkin"), 0};
	setting_t<int> Skins_SemiAutoPistolSkin {&m_holder, fnvc("Skins_SemiAutoPistolSkin"), 0};
	setting_t<int> Skins_PythonRevolverSkin {&m_holder, fnvc("Skins_PythonRevolverSkin"), 0};
	setting_t<int> Skins_M39Skin {&m_holder, fnvc("Skins_M39Skin"), 0};
	setting_t<int> Skins_L96Skin {&m_holder, fnvc("Skins_L96Skin"), 0};
	setting_t<int> Skins_BowSkin {&m_holder, fnvc("Skins_BowSkin"), 0};
	setting_t<int> Skins_CrossbowSkin {&m_holder, fnvc("Skins_CrossbowSkin"), 0};
	setting_t<int> Skins_EokaSkin {&m_holder, fnvc("Skins_EokaSkin"), 0};
	setting_t<int> Skins_JackHammerSkin {&m_holder, fnvc("Skins_JackHammerSkin"), 0};
	setting_t<int> Skins_SalvagedIcepickSkin {&m_holder, fnvc("Skins_SalvagedIcepickSkin"), 0};
	setting_t<int> Skins_PickaxeSkin {&m_holder, fnvc("Skins_PickaxeSkin"), 0};
	setting_t<int> Skins_HatchetSkin {&m_holder, fnvc("Skins_HatchetSkin"), 0};
	setting_t<int> Skins_StonePickaxeSkin {&m_holder, fnvc("Skins_StonePickaxeSkin"), 0};
	setting_t<int> Skins_StoneHatchetSkin {&m_holder, fnvc("Skins_StoneHatchetSkin"), 0};
	setting_t<int> Skins_CombatKnifeSkin {&m_holder, fnvc("Skins_CombatKnifeSkin"), 0};
	setting_t<int> Skins_RockSkin {&m_holder, fnvc("Skins_RockSkin"), 0};
	setting_t<int> Skins_RocketLauncherSkin {&m_holder, fnvc("Skins_RocketLauncherSkin"), 0};
	setting_t<int> Skins_RevolverSkin {&m_holder, fnvc("Skins_RevolverSkin"), 0};

	// Random Misc
	setting_t<bool> Misc_InstantInteract{ &m_holder, fnvc("Misc_InstantInteract"), false };

	//OVERLAY
	setting_t<bool> Overlay_OverRideResolution {&m_holder, fnvc("Overlay_OverRideResolution"), false};
	setting_t<bool> Overlay_Transparent {&m_holder, fnvc("Overlay_Transparent"), false};
	setting_t<bool> Overlay_BoldFont {&m_holder, fnvc("Overlay_BoldFont"), false};
	setting_t<int> Overlay_ScreenWidth {&m_holder, fnvc("Overlay_ScreenWidth"), 0};
	setting_t<int> Overlay_ScreenHeight {&m_holder, fnvc("Overlay_ScreenHeight"), 0};
	setting_t<bool> Overlay_Vsync {&m_holder, fnvc("Overlay_Vsync"), false};
	setting_t<int> Overlay_RevertKey {&m_holder, fnvc("Overlay_RevertKey"), 123};

	//Aimbot Config
	setting_t<bool> Aimbot_Enable {&m_holder, fnvc("Aimbot_Enable"), true};
	setting_t<bool> Aimbot_TargetScientist {&m_holder, fnvc("Aimbot_TargetScientist"), true};
	setting_t<bool> Aimbot_TargetPlayer {&m_holder, fnvc("Aimbot_TargetPlayer"), true};
	setting_t<bool> Aimbot_StickyAim{ &m_holder, fnvc("Aimbot_StickyAim"), true };
	setting_t<bool> Aimbot_StickyAimIgnoreFOV{ &m_holder, fnvc("Aimbot_StickyAimIgnoreFOV"), true };
	setting_t<bool> Aimbot_StickyAimIgnoreVisibility{ &m_holder, fnvc("Aimbot_StickyAimIgnoreVisibility"), false };
	setting_t<int> Aimbot_RetargetTime{ &m_holder, fnvc("Aimbot_RetargetTime"), 100 };
	setting_t<bool> Aimbot_TargetInjured {&m_holder, fnvc("Aimbot_TargetInjured"), false};
	setting_t<bool> Aimbot_VisibleOnly {&m_holder, fnvc("Aimbot_VisibleOnly"), true};
	setting_t<bool> Aimbot_DrawTarget{ &m_holder, fnvc("Aimbot_DrawTarget"), false };
	setting_t<ImColor> Aimbot_DrawTarget_Colour{ &m_holder, fnvc("Aimbot_DrawTarget_Colour"), ImColor(0, 150, 255, 255) };
	setting_t<int> Aimbot_FOV {&m_holder, fnvc("Aimbot_FOV"), 200};
	setting_t<int> Aimbot_SmoothX {&m_holder, fnvc("Aimbot_SmoothX"), 25};
	setting_t<int> Aimbot_SmoothY {&m_holder, fnvc("Aimbot_SmoothY"), 25};
	setting_t<bool> Aimbot_SlowMovingProjectile_Smooth{ &m_holder, fnvc("Aimbot_SlowMovingProjectile_Smooth"), false };
	setting_t<int> Aimbot_SlowMovingProjectile_SmoothX{ &m_holder, fnvc("Aimbot_SlowMovingProjectile_SmoothX"), 10 };
	setting_t<int> Aimbot_SlowMovingProjectile_SmoothY{ &m_holder, fnvc("Aimbot_SlowMovingProjectile_SmoothY"), 10 };
	setting_t<int> Aimbot_MaxDistance {&m_holder, fnvc("Aimbot_MaxDistance"), 300};
	setting_t<bool> Aimbot_DrawFOV {&m_holder, fnvc("Aimbot_DrawFOV"), true};
	setting_t<int> Aimbot_AimBone {&m_holder, fnvc("Aimbot_AimBone"), 0};
	setting_t<int> Aimbot_AimKey {&m_holder, fnvc("Aimbot_AimKey"), VK_XBUTTON1};
	setting_t<int> Aimbot_AimType {&m_holder, fnvc("Aimbot_AimType"), 0};
	setting_t<int> Aimbot_SmoothingType {&m_holder, fnvc("Aimbot_SmoothingType"), 0};
	setting_t<int> Aimbot_TargetPriority {&m_holder, fnvc("Aimbot_TargetPriority"), 1};
	setting_t<ImColor> Aimbot_FOVColour {&m_holder, fnvc("Aimbot_FOVColour"), ImColor(255, 255, 255, 255)};
	setting_t<int> Aimbot_KmboxBaudRate {&m_holder, fnvc("Aimbot_KmboxBaudRate"), 115200};
	setting_t<int> Aimbot_KmboxDelay {&m_holder, fnvc("Aimbot_KmboxDelay"), 10};
	setting_t<const char*> Aimbot_NetworkIP {&m_holder, fnvc("Aimbot_NetworkIP "), "192.168.178.102"};
	setting_t<const char*> Aimbot_NetworkPort {&m_holder, fnvc("Aimbot_NetworkPort "), "512"};
	setting_t<const char*> Aimbot_NetworkMac {&m_holder, fnvc("Aimbot_NetworkMac "), "C362383B"};
	setting_t<const char*> Aimbot_KmboxComPort {&m_holder, fnvc("Aimbot_KmboxComPort "), "Com0"};
	setting_t<bool> Aimbot_TargetHeli {&m_holder, fnvc("Aimbot_TargetHeli"), true};
	setting_t<bool> Aimbot_TargetBradley{ &m_holder, fnvc("Aimbot_TargetBradley"), true };
	setting_t<int> Aimbot_VehicleMaxDistance {&m_holder, fnvc("Aimbot_VehicleMaxDistance"), 450};
	setting_t<bool> Aimbot_Hybrid{ &m_holder, fnvc("Aimbot_Hybrid"), false };
	setting_t<int> Aimbot_Hybrid_AimBone{ &m_holder, fnvc("Aimbot_Hybrid_AimBone"), 0 };
	setting_t<int> Aimbot_Hybrid_FOV{ &m_holder, fnvc("Aimbot_Hybrid_FOV"), 200 };
	setting_t<int> Aimbot_Hybrid_CutOut_FOV{ &m_holder, fnvc("Aimbot_Hybrid_CutOut_FOV"), 10 };
	setting_t<bool> Aimbot_Movement_Prediction{ &m_holder, fnvc("Aimbot_Movement_Prediction"), true };
	setting_t<bool> Aimbot_ShortAlgo{ &m_holder, fnvc("Aimbot_ShortAlgo"), false };
	setting_t<int> Aimbot_Prediction_SampleSize{ &m_holder, fnvc("Aimbot_Prediction_SampleSize"), 12 };
	setting_t<float> Aimbot_Movement_Prediction_Modifier{ &m_holder, fnvc("Aimbot_Movement_Prediction_Modifier"), 1.0f };
	setting_t<bool> Aimbot_Movement_LocalPlayerPrediction{ &m_holder, fnvc("Aimbot_Movement_LocalPlayerPrediction"), true };
	//BattleMode
	setting_t<bool> Battlemode_Enable {&m_holder, fnvc("Battlemode_Enable"), false};
	setting_t<bool> Battlemode_Indicator{ &m_holder, fnvc("Battlemode_Indicator"), true };
	setting_t<bool> Battlemode_ResourceSaver{ &m_holder, fnvc("Battlemode_ResourceSaver"), false };
	setting_t<int> Battlemode_key {&m_holder, fnvc("Battlemode_key"), 0x58};

	//ESP Stuff
	//Entities start here
	setting_t<Players> Player {&m_holder, fnvc("Player"), Players()};
	setting_t<RadarPlayers> PlayerRadar{ &m_holder, fnvc("PlayerRadar"), RadarPlayers() };
	setting_t<Players> Friend {&m_holder, fnvc("Friend"), Players()};
	setting_t<RadarPlayers> FriendRadar{ &m_holder, fnvc("FriendRadar"), RadarPlayers() };
	setting_t<Players> Scientist {&m_holder, fnvc("Scientist"), Players()};
	setting_t<RadarPlayers> ScientistRadar{ &m_holder, fnvc("ScientistRadar"), RadarPlayers() };

	setting_t<Animals> Bear {&m_holder, fnvc("Bear"), Animals()};
	setting_t<RadarAnimals> RadarBear{ &m_holder, fnvc("RadarBear"), RadarAnimals() };
	setting_t<Animals> Wolf {&m_holder, fnvc("Wolf"), Animals()};
	setting_t<RadarAnimals> RadarWolf{ &m_holder, fnvc("RadarWolf"), RadarAnimals() };
	setting_t<Animals> Boar {&m_holder, fnvc("Boar"), Animals()};
	setting_t<RadarAnimals> RadarBoar{ &m_holder, fnvc("RadarBoar"), RadarAnimals() };
	setting_t<Animals> Chicken {&m_holder, fnvc("Chicken"), Animals()};
	setting_t<RadarAnimals> RadarChicken{ &m_holder, fnvc("RadarChicken"), RadarAnimals() };
	setting_t<Animals> Shark{ &m_holder, fnvc("Shark"), Animals() };
	setting_t<RadarAnimals> RadarShark{ &m_holder, fnvc("RadarShark"), RadarAnimals() };
	setting_t<Animals> Horse {&m_holder, fnvc("Horse"), Animals()};
	setting_t<RadarAnimals> RadarHorse{ &m_holder, fnvc("RadarHorse"), RadarAnimals() };
	setting_t<Animals> Stag {&m_holder, fnvc("Stag"), Animals()};
	setting_t<RadarAnimals> RadarStag{ &m_holder, fnvc("RadarStag"), RadarAnimals() };
	setting_t<Animals> PolarBear {&m_holder, fnvc("PolarBear"), Animals()};
	setting_t<RadarAnimals> RadarPolarBear{ &m_holder, fnvc("RadarPolarBear"), RadarAnimals() };

	setting_t<Vehicles> PatrolHelicopter {&m_holder, fnvc("PatrolHelicopter"), Vehicles()};
	setting_t<RadarVehicles> RadarPatrolHelicopter{ &m_holder, fnvc("RadarPatrolHelicopter"), RadarVehicles() };
	setting_t<Vehicles> Bradley {&m_holder, fnvc("Bradley"), Vehicles()};
	setting_t<RadarVehicles> RadarBradley{ &m_holder, fnvc("RadarBradley"), RadarVehicles() };
	setting_t<Vehicles> ScrapHeli {&m_holder, fnvc("ScrapHeli"), Vehicles()};
	setting_t<RadarVehicles> RadarScrapHeli{ &m_holder, fnvc("RadarScrapHeli"), RadarVehicles() };
	setting_t<Vehicles> MiniCopter {&m_holder, fnvc("MiniCopter"), Vehicles()};
	setting_t<RadarVehicles> RadarMiniCopter{ &m_holder, fnvc("RadarMiniCopter"), RadarVehicles() };
	setting_t<Vehicles> AttackMiniCopter {&m_holder, fnvc("AttackMiniCopter"), Vehicles()};
	setting_t<RadarVehicles> RadarAttackMiniCopter{ &m_holder, fnvc("RadarAttackMiniCopter"), RadarVehicles() };
	setting_t<Vehicles> Train {&m_holder, fnvc("Train"), Vehicles()};
	setting_t<RadarVehicles> RadarTrain{ &m_holder, fnvc("RadarTrain"), RadarVehicles() };
	setting_t<Vehicles> Car {&m_holder, fnvc("Car"), Vehicles()};
	setting_t<RadarVehicles> RadarCar{ &m_holder, fnvc("RadarCar"), RadarVehicles() };
	setting_t<Vehicles> TugBoat {&m_holder, fnvc("TugBoat"), Vehicles()};
	setting_t<RadarVehicles> RadarTugBoat{ &m_holder, fnvc("RadarTugBoat"), RadarVehicles() };
	setting_t<Vehicles> RowBoat {&m_holder, fnvc("RowBoat"), Vehicles()};
	setting_t<RadarVehicles> RadarRowBoat{ &m_holder, fnvc("RadarRowBoat"), RadarVehicles() };
	setting_t<Vehicles> RHIB {&m_holder, fnvc("RHIB"), Vehicles()};
	setting_t<RadarVehicles> RadarRHIB{ &m_holder, fnvc("RadarRHIB"), RadarVehicles() };
	setting_t<Vehicles> SnowMobile{ &m_holder, fnvc("SnowMobile"), Vehicles() };
	setting_t<RadarVehicles> RadarSnowMobile{ &m_holder, fnvc("RadarSnowMobile"), RadarVehicles() };
	setting_t<Vehicles> SmallSubmarine{ &m_holder, fnvc("SmallSubmarine"), Vehicles() };
	setting_t<RadarVehicles> RadarSmallSubmarine{ &m_holder, fnvc("RadarSmallSubmarine"), RadarVehicles() };
	setting_t<Vehicles> LargeSubmarine{ &m_holder, fnvc("LargeSubmarine"), Vehicles() };
	setting_t<RadarVehicles> RadarLargeSubmarine{ &m_holder, fnvc("RadarLargeSubmarine"), RadarVehicles() };
	setting_t<Vehicles> Pedalbike{ &m_holder, fnvc("Pedalbike"), Vehicles() };
	setting_t<RadarVehicles> RadarPedalbike{ &m_holder, fnvc("RadarPedalbike"), RadarVehicles() };
	setting_t<Vehicles> Motorbike{ &m_holder, fnvc("Motorbike"), Vehicles() };
	setting_t<RadarVehicles> RadarMotorbike{ &m_holder, fnvc("RadarMotorbike"), RadarVehicles() };
	setting_t<Vehicles> MotorbikeSidecar{ &m_holder, fnvc("MotorbikeSidecar"), Vehicles() };
	setting_t<RadarVehicles> RadarMotorbikeSidecar{ &m_holder, fnvc("RadarMotorbikeSidecar"), RadarVehicles() };
	//Entities end here

	//Resource ESP starts here
	setting_t<Ores> Stone {&m_holder, fnvc("Stone"), Ores()};
	setting_t<RadarOres> RadarStone{ &m_holder, fnvc("RadarStone"), RadarOres() };
	setting_t<Ores> Metal {&m_holder, fnvc("Metal"), Ores()};
	setting_t<RadarOres> RadarMetal{ &m_holder, fnvc("RadarMetal"), RadarOres() };
	setting_t<Ores> Sulphur {&m_holder, fnvc("Sulphur"), Ores()};
	setting_t<RadarOres> RadarSulphur{ &m_holder, fnvc("RadarSulphur"), RadarOres() };

	setting_t<Collectables> Wood {&m_holder, fnvc("Wood"), Collectables()};
	setting_t<RadarCollectables> RadarWood{ &m_holder, fnvc("RadarWood"), RadarCollectables() };
	setting_t<Collectables> Hemp {&m_holder, fnvc("Hemp"), Collectables()};
	setting_t<RadarCollectables> RadarHemp{ &m_holder, fnvc("RadarHemp"), RadarCollectables() };
	setting_t<Collectables> Pumpkin {&m_holder, fnvc("Pumpkin"), Collectables()};
	setting_t<RadarCollectables> RadarPumpkin{ &m_holder, fnvc("RadarPumpkin"), RadarCollectables() };
	setting_t<Collectables> Corn {&m_holder, fnvc("Corn"), Collectables()};
	setting_t<RadarCollectables> RadarCorn{ &m_holder, fnvc("RadarCorn"), RadarCollectables() };
	setting_t<Collectables> Mushroom {&m_holder, fnvc("Mushroom"), Collectables()};
	setting_t<RadarCollectables> RadarMushroom{ &m_holder, fnvc("RadarMushroom"), RadarCollectables() };
	setting_t<Collectables> Potato {&m_holder, fnvc("Potato"), Collectables()};
		setting_t<RadarCollectables> RadarPotato{ &m_holder, fnvc("RadarPotato"), RadarCollectables() };
	setting_t<Collectables> RedBerry {&m_holder, fnvc("RedBerry"), Collectables()};
		setting_t<RadarCollectables> RadarRedBerry{ &m_holder, fnvc("RadarRedBerry"), RadarCollectables() };
	setting_t<Collectables> WhiteBerry {&m_holder, fnvc("WhiteBerry"), Collectables()};
	setting_t<RadarCollectables> RadarWhiteBerry{ &m_holder, fnvc("RadarWhiteBerry"), RadarCollectables() };
	setting_t<Collectables> BlackBerry {&m_holder, fnvc("BlackBerry"), Collectables()};
	setting_t<RadarCollectables> RadarBlackBerry{ &m_holder, fnvc("RadarBlackBerry"), RadarCollectables() };
	setting_t<Collectables> YellowBerry {&m_holder, fnvc("YellowBerry"), Collectables()};
	setting_t<RadarCollectables> RadarYellowBerry{ &m_holder, fnvc("RadarYellowBerry"), RadarCollectables() };
	setting_t<Collectables> BlueBerry {&m_holder, fnvc("BlueBerry"), Collectables()};
	setting_t<RadarCollectables> RadarBlueBerry{ &m_holder, fnvc("RadarBlueBerry"), RadarCollectables() };
	setting_t<Collectables> GreenBerry {&m_holder, fnvc("GreenBerry"), Collectables()};
	setting_t<RadarCollectables> RadarGreenBerry{ &m_holder, fnvc("RadarGreenBerry"), RadarCollectables() };
	setting_t<Collectables> StoneCollectable {&m_holder, fnvc("StoneCollectable"), Collectables()};
	setting_t<RadarCollectables> RadarStoneCollectable{ &m_holder, fnvc("RadarStoneCollectable"), RadarCollectables() };
	setting_t<Collectables> MetalCollectable {&m_holder, fnvc("MetalCollectable"), Collectables()};
	setting_t<RadarCollectables> RadarMetalCollectable{ &m_holder, fnvc("RadarMetalCollectable"), RadarCollectables() };
	setting_t<Collectables> SulphurCollectable {&m_holder, fnvc("SulphurCollectable"), Collectables()};
	setting_t<RadarCollectables> RadarSulphurCollectable{ &m_holder, fnvc("RadarSulphurCollectable"), RadarCollectables() };
	setting_t<Collectables> Diesel {&m_holder, fnvc("Diesel"), Collectables()};
	setting_t<RadarCollectables> RadarDiesel{ &m_holder, fnvc("RadarDiesel"), RadarCollectables() };
	setting_t<Collectables> MetalDetector {&m_holder, fnvc("MetalDetector"), Collectables()};
	setting_t<RadarCollectables> RadarMetalDetector{ &m_holder, fnvc("RadarMetalDetector"), RadarCollectables() };
	//Resource ESP Ends here

	//Lootable ESP Starts here
	setting_t<DroppedItems> OtherItems {&m_holder, fnvc("OtherItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarOtherItems{ &m_holder, fnvc("RadarOtherItems"), RadarDroppedItems() };
	setting_t<DroppedItems> GunItems {&m_holder, fnvc("GunItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarGunItems{ &m_holder, fnvc("RadarGunItems"), RadarDroppedItems() };
	setting_t<DroppedItems> ToolItems {&m_holder, fnvc("ToolItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarToolItems{ &m_holder, fnvc("RadarToolItems"), RadarDroppedItems() };
	setting_t<DroppedItems> ArmourItems {&m_holder, fnvc("ArmourItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarArmourItems{ &m_holder, fnvc("RadarArmourItems"), RadarDroppedItems() };
	setting_t<DroppedItems> MedicalItems {&m_holder, fnvc("MedicalItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarMedicalItems{ &m_holder, fnvc("RadarMedicalItems"), RadarDroppedItems() };
	setting_t<DroppedItems> ResourceItems {&m_holder, fnvc("ResourceItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarResourceItems{ &m_holder, fnvc("RadarResourceItems"), RadarDroppedItems() };
	setting_t<DroppedItems> FoodItems {&m_holder, fnvc("FoodItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarFoodItems{ &m_holder, fnvc("RadarFoodItems"), RadarDroppedItems() };
	setting_t<DroppedItems> ExplosiveItems {&m_holder, fnvc("ExplosiveItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarExplosiveItems{ &m_holder, fnvc("RadarExplosiveItems"), RadarDroppedItems() };
	setting_t<DroppedItems> AmmoItems {&m_holder, fnvc("AmmoItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarAmmoItems{ &m_holder, fnvc("RadarAmmoItems"), RadarDroppedItems() };
	setting_t<DroppedItems> ComponentsItems {&m_holder, fnvc("ComponentsItems"), DroppedItems()};
	setting_t<RadarDroppedItems> RadarComponentsItems{ &m_holder, fnvc("RadarComponentsItems"), RadarDroppedItems() };

	setting_t<Lootables> Barrel {&m_holder, fnvc("Barrel"), Lootables()};
	setting_t<RadarLootables> RadarBarrel{ &m_holder, fnvc("RadarBarrel"), RadarLootables() };
	setting_t<Lootables> FuelBarrel {&m_holder, fnvc("FuelBarrel"), Lootables()};
	setting_t<RadarLootables> RadarFuelBarrel{ &m_holder, fnvc("RadarFuelBarrel"), RadarLootables() };
	setting_t<Lootables> Corpse {&m_holder, fnvc("Corpse"), Lootables()};
	setting_t<RadarLootables> RadarCorpse{ &m_holder, fnvc("RadarCorpse"), RadarLootables() };
	setting_t<Lootables> LockedCrate {&m_holder, fnvc("LockedCrate"), Lootables()};
	setting_t<RadarLootables> RadarLockedCrate{ &m_holder, fnvc("RadarLockedCrate"), RadarLootables() };
	setting_t<Lootables> SupplyDrop {&m_holder, fnvc("SupplyDrop"), Lootables()};
	setting_t<RadarLootables> RadarSupplyDrop{ &m_holder, fnvc("RadarSupplyDrop"), RadarLootables() };
	setting_t<Lootables> EliteCrate {&m_holder, fnvc("EliteCrate"), Lootables()};
	setting_t<RadarLootables> RadarEliteCrate{ &m_holder, fnvc("RadarEliteCrate"), RadarLootables() };
	setting_t<Lootables> NormalCrate {&m_holder, fnvc("NormalCrate"), Lootables()};
	setting_t<RadarLootables> RadarNormalCrate{ &m_holder, fnvc("RadarNormalCrate"), RadarLootables() };
	setting_t<Lootables> MilitaryCrate {&m_holder, fnvc("MilitaryCrate"), Lootables()};
	setting_t<RadarLootables> RadarMilitaryCrate{ &m_holder, fnvc("RadarMilitaryCrate"), RadarLootables() };
	setting_t<Lootables> MedicalCrate {&m_holder, fnvc("MedicalCrate"), Lootables()};
	setting_t<RadarLootables> RadarMedicalCrate{ &m_holder, fnvc("RadarMedicalCrate"), RadarLootables() };
	setting_t<Lootables> FoodCrate {&m_holder, fnvc("FoodCrate"), Lootables()};
	setting_t<RadarLootables> RadarFoodCrate{ &m_holder, fnvc("RadarFoodCrate"), RadarLootables() };
	setting_t<Lootables> MineCart {&m_holder, fnvc("MineCart"), Lootables()};
	setting_t<RadarLootables> RadarMineCart{ &m_holder, fnvc("RadarMineCart"), RadarLootables() };
	setting_t<Lootables> ToolBox {&m_holder, fnvc("ToolBox"), Lootables()};
	setting_t<RadarLootables> RadarToolBox{ &m_holder, fnvc("RadarToolBox"), RadarLootables() };
	setting_t<Lootables> AmmoCrate {&m_holder, fnvc("AmmoCrate"), Lootables()};
	setting_t<RadarLootables> RadarAmmoCrate{ &m_holder, fnvc("RadarAmmoCrate"), RadarLootables() };
	setting_t<Lootables> FuelCrate {&m_holder, fnvc("FuelCrate"), Lootables()};
	setting_t<RadarLootables> RadarFuelCrate{ &m_holder, fnvc("RadarFuelCrate"), RadarLootables() };
	setting_t<Lootables> UnderWaterCrate {&m_holder, fnvc("UnderWaterCrate"), Lootables()};
	setting_t<RadarLootables> RadarUnderWaterCrate{ &m_holder, fnvc("RadarUnderWaterCrate"), RadarLootables() };
	setting_t<Lootables> HelicopterCrate {&m_holder, fnvc("HelicopterCrate"), Lootables()};
	setting_t<RadarLootables> RadarHelicopterCrate{ &m_holder, fnvc("RadarHelicopterCrate"), RadarLootables() };
	setting_t<Lootables> BradleyCrate {&m_holder, fnvc("BradleyCrate"), Lootables()};
	setting_t<RadarLootables> RadarBradleyCrate{ &m_holder, fnvc("RadarBradleyCrate"), RadarLootables() };
	setting_t<Lootables> TechCrate{ &m_holder, fnvc("TechCrate"), Lootables() };
	setting_t<RadarLootables> RadarTechCrate{ &m_holder, fnvc("RadarTechCrate"), RadarLootables() };
	//Looteable ESP Ends here

	//Other ESP Starts here
	setting_t<Deployables> Stash {&m_holder, fnvc("Stash"), Deployables()};
	setting_t<RadarDeployables> RadarStash{ &m_holder, fnvc("RadarStash"), RadarDeployables() };
	setting_t<Deployables> SleepingBag {&m_holder, fnvc("SleepingBag"), Deployables()};
	setting_t<RadarDeployables> RadarSleepingBag{ &m_holder, fnvc("RadarSleepingBag"), RadarDeployables() };
	setting_t<Deployables> LandMine {&m_holder, fnvc("LandMine"), Deployables()};
	setting_t<RadarDeployables> RadarLandMine{ &m_holder, fnvc("RadarLandMine"), RadarDeployables() };
	setting_t<Deployables> BearTrap {&m_holder, fnvc("BearTrap"), Deployables()};
	setting_t<RadarDeployables> RadarBearTrap{ &m_holder, fnvc("RadarBearTrap"), RadarDeployables() };
	setting_t<Deployables> ShotgunTrap {&m_holder, fnvc("ShotgunTrap"), Deployables()};
	setting_t<RadarDeployables> RadarShotgunTrap{ &m_holder, fnvc("RadarShotgunTrap"), RadarDeployables() };
	setting_t<Deployables> AutoTurret {&m_holder, fnvc("AutoTurret"), Deployables()};
	setting_t<RadarDeployables> RadarAutoTurret{ &m_holder, fnvc("RadarAutoTurret"), RadarDeployables() };
	setting_t<Deployables> FlameTurret {&m_holder, fnvc("FlameTurret"), Deployables()};
	setting_t<RadarDeployables> RadarFlameTurret{ &m_holder, fnvc("RadarFlameTurret"), RadarDeployables() };
	setting_t<Deployables> SamSite {&m_holder, fnvc("SamSite"), Deployables()};
	setting_t<RadarDeployables> RadarSamSite{ &m_holder, fnvc("RadarSamSite"), RadarDeployables() };
	setting_t<Deployables> ToolCupboard {&m_holder, fnvc("ToolCupboard"), Deployables()};
	setting_t<RadarDeployables> RadarToolCupboard{ &m_holder, fnvc("RadarToolCupboard"), RadarDeployables() };
	setting_t<Deployables> Planter {&m_holder, fnvc("Planter"), Deployables()};
	setting_t<RadarDeployables> RadarPlanter{ &m_holder, fnvc("RadarPlanter"), RadarDeployables() };
	setting_t<Deployables> LargeBox {&m_holder, fnvc("LargeBox"), Deployables()};
	setting_t<RadarDeployables> RadarLargeBox{ &m_holder, fnvc("RadarLargeBox"), RadarDeployables() };
	setting_t<Deployables> Workbench {&m_holder, fnvc("Workbench"), Deployables()};
	setting_t<RadarDeployables> RadarWorkbench{ &m_holder, fnvc("RadarWorkbench"), RadarDeployables() };
	setting_t<Deployables> Furnace {&m_holder, fnvc("Furnace"), Deployables()};
	setting_t<RadarDeployables> RadarFurnace{ &m_holder, fnvc("RadarFurnace"), RadarDeployables() };
	setting_t<Deployables> OilRefinery {&m_holder, fnvc("OilRefinery"), Deployables()};
	setting_t<RadarDeployables> RadarOilRefinery{ &m_holder, fnvc("RadarOilRefinery"), RadarDeployables() };
	setting_t<Deployables> Recycler {&m_holder, fnvc("Recycler"), Deployables()};
	setting_t<RadarDeployables> RadarRecycler{ &m_holder, fnvc("RadarRecycler"), RadarDeployables() };
	setting_t<Deployables> Battery {&m_holder, fnvc("Battery"), Deployables()};
	setting_t<RadarDeployables> RadarBattery{ &m_holder, fnvc("RadarBattery"), RadarDeployables() };
	//Other ESP Ends here

	/* Hardware mouse type */
	setting_t<int> hardware_mouse_type {&m_holder, fnvc("hardware_mouse_type"), 1};
	setting_t<bool> UnOptimiseBones{ &m_holder, fnvc("UnOptimiseBones"), false };

	setting_t<bool> Misc_MultiThreadedViewUpdate{ &m_holder, fnvc("Misc_MultiThreadedViewUpdate"), false};
	setting_t<bool> Misc_FastUpdatePlayers{ &m_holder, fnvc("Misc_FastUpdatePlayers"), false };
	setting_t<bool> Misc_ExceptionCatching{ &m_holder, fnvc("Misc_ExceptionCatching"), true };
	setting_t<bool> Misc_PerformanceSleep{ &m_holder, fnvc("Misc_PerformanceSleep"), false };
	setting_t<int> Misc_SleepAmount{ &m_holder, fnvc("Misc_SleepAmount"), 1 };
	setting_t<bool> Misc_HeapMemoryCheck{ &m_holder, fnvc("Misc_HeapMemoryCheck"), false };
	setting_t<bool> Misc_DisableESP{ &m_holder, fnvc("Misc_DisableESP"), false };
	setting_t<int> Misc_Font{ &m_holder, fnvc("Misc_Font"), 0 };
	setting_t<bool> Misc_Font_Override{ &m_holder, fnvc("Misc_Font_Override"), false };
	setting_t<int> Misc_Font_Override_Size{ &m_holder, fnvc("Misc_Font_Override_Size"), 16 };
	void save(const char* name)
	{
		std::string path = std::getenv(xorstr_("USERPROFILE"));
		path += xorstr_("\\Documents");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Moe");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Config");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Rust");
		if (!DoesPathExist(path))
			CreateFolder(path);
		m_holder.save(path.c_str(), name);
	}

	bool exists(const char* name)
	{
		std::string path = std::getenv(xorstr_("USERPROFILE"));
		path += xorstr_("\\Documents");
		if (!DoesPathExist(path))
			return false;
		path += xorstr_("\\Moe");
		if (!DoesPathExist(path))
			return false;
		path += xorstr_("\\Config");
		if (!DoesPathExist(path))
			return false;
		path += xorstr_("\\Rust");
		if (!DoesPathExist(path))
			return false;
		path += xorstr_("\\") + std::string(name);
		if (std::filesystem::exists(path))
			return true;
		return false;
	}

	void load(const char* name)
	{
		std::string path = std::getenv(xorstr_("USERPROFILE"));
		path += xorstr_("\\Documents");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Moe");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Config");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Rust");
		if (!DoesPathExist(path))
			CreateFolder(path);
		m_holder.load(path.c_str(), name);
	}

	void _delete(const char* name)
	{
		std::string path = std::getenv(xorstr_("USERPROFILE"));
		path += xorstr_("\\Documents");
		if (!DoesPathExist(path))
			return;
		path += xorstr_("\\Moe");
		if (!DoesPathExist(path))
			return;
		path += xorstr_("\\Config");
		if (!DoesPathExist(path))
			return;
		path += xorstr_("\\Rust");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\") + std::string(name);
		if (std::filesystem::exists(path))
			std::filesystem::remove(path);
	}

	std::vector<std::string> get_all_configs()
	{
		std::vector<std::string> configs;
		std::string path = std::getenv(xorstr_("USERPROFILE"));
		path += xorstr_("\\Documents");
		if (!DoesPathExist(path))
			return configs;
		path += xorstr_("\\Moe");
		if (!DoesPathExist(path))
			return configs;
		path += xorstr_("\\Config");
		if (!DoesPathExist(path))
			return configs;
		path += xorstr_("\\Rust");
		if (!DoesPathExist(path))
			return configs;
		//get all files in folder
		try
		{
			for (const auto& entry : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_regular_file(entry.path()))
					configs.push_back(entry.path().filename().string());
		}
		catch (const std::exception& e)
		{
		}
		return configs;
	}

	void save_conf(std::string& config) { m_holder.save(config); }
	void load_conf(std::string& config) { m_holder.load(config); }

	template <typename T>
	void Set(T* setting, T value)
	{
		*setting = value;
	}

	template <typename T>
	void Toggle(T* setting)
	{
		*setting = !*setting;
	}
};

extern c_settings settings;
