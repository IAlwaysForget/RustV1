#include "GUI.h"
#include <thread>
#include "Fuser/fuser.h"
#include <imgui_internal.h>
#include <unordered_map>
#include <random>

#include "context.h"
#include "Font/fontawesome_duotone.h"
#include "Font/fontawesome_regular.h"
#include "Font/newIcons.h"
#include "Font/roboto.h"
#include "Font/textFont.h"
#include "Font/twemoji.h"
#include "Hotkey/Hotkey.h"
#include "Images/logo.h"
#include "misc/freetype/imgui_freetype.h"

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "winmm.lib")

std::string productName = "Moe";
ImVec2 ContainerPos;

namespace navigation
{
	std::string activeID = "Visuals";
	std::string lastActiveID;
	bool runAnimation;

	void setActive(std::string name)
	{
		runAnimation = true;
		lastActiveID = activeID;
		activeID = name;
	}

	bool isActive(std::string name)
	{
		if (activeID == name) return true;
		else return false;
	}

	std::string lastActive()
	{
		return lastActiveID;
	}
}

namespace Colors
{
	//White Theme
	/*ImColor grayText = ImColor(58, 58, 59, 255);
	ImColor whiteText = ImColor(98, 98, 99, 255);

	ImColor mainWindowBackground = ImColor(222, 222, 222, 255); //the feature background
	ImColor mainHeaderBackground = ImColor(189, 189, 189, 255); //nav
	ImColor mainSidebarBackground = ImColor(233, 233, 233, 255); //sidebar
	ImColor mainSidebarBackground2 = ImColor(200, 200, 200, 255); //config
	ImColor mainSidebarClockBackground = ImColor(210, 210, 210, 255);*/

	//dark theme
	//Text
	ImColor grayText = ImColor(196, 196, 196, 255);
	ImColor whiteText = ImColor(225, 225, 225, 255);

	//MainWindow
	ImColor mainWindowBackground = ImColor(26, 26, 38, 255);
	ImColor mainHeaderBackground = ImColor(15, 15, 26, 255);
	ImColor mainSidebarBackground = ImColor(19, 19, 28, 255);
	ImColor mainSidebarBackground2 = ImColor(22, 22, 31, 255);
	ImColor mainSidebarClockBackground = ImColor(6, 6, 10, 255);

	//Sidebar Userprofile
	ImColor userConnected = ImColor(68, 255, 59, 255);
	ImColor userDisconnected = ImColor(250, 37, 59, 255);
	ImColor userTag = ImColor(115, 115, 115, 255);

	//Sidebar button
	ImColor sidebarBackground = ImColor(6, 6, 10, 130);
	ImColor sidebarBackgroundBar = ImColor(70, 205, 250, 130);
	ImColor sidebarIcon = ImColor(94, 255, 176, 130);

	//SideBar rectangles
	ImColor sidebarRect1 = ImColor(56, 255, 159, 100);
	ImColor sidebarRect2 = ImColor(5, 156, 82, 100);
	ImColor sidebarRect3 = ImColor(76, 76, 107, 100);

	ImColor headerSeperater = ImColor(29, 29, 48, 255); //next to minimize / exit
}

namespace Custom
{
	bool modalOpen;

	bool IsMouseHoveringRect(ImVec2 min, ImVec2 max)
	{
		if (modalOpen)
			return false;

		if (ImGui::IsMouseHoveringRect(min, max))
			return true;

		return false;
	}
}

std::unordered_map<std::string, float> g_anim = { };

namespace Animation
{
	float EaseInOut(float startValue, float endValue, float t)
	{
		return (startValue + (endValue - startValue) * (t * t * (3.0f - 2.0f * t)));
	}

	float GetAnimData(std::string key, ImVec2 start, ImVec2 end, float animationPixels, float animationDuration, float start_value, float end_value)
	{
		if (g_anim.find(key) == g_anim.end())
		{
			// If key not found in the map, add it with an initial value
			g_anim[key] = 0.0f;
		}

		if (Custom::IsMouseHoveringRect(start, end))
		{
			if (g_anim[key] < animationPixels)
			{
				g_anim[key] += ImGui::GetIO().DeltaTime * (animationPixels / animationDuration);
				if (g_anim[key] >= animationPixels)
				{
					g_anim[key] = animationPixels;
				}
			}
		}
		else
		{
			if (g_anim.find(key) != g_anim.end() && g_anim[key] > 0.0f)
			{
				g_anim[key] -= ImGui::GetIO().DeltaTime * (animationPixels / animationDuration);
				if (g_anim[key] < 0.0f)
				{
					g_anim[key] = 0.0f;
				}
			}
		}

		float t = g_anim[key] / animationPixels;
		return Animation::EaseInOut(start_value, end_value, t);
	}

	float GetAnimData(std::string key, bool hovering, float animationPixels, float animationDuration, float start, float end)
	{
		if (g_anim.find(key) == g_anim.end())
		{
			// If key not found in the map, add it with an initial value
			g_anim[key] = 0.0f;
		}

		if (hovering)
		{
			if (g_anim[key] < animationPixels)
			{
				g_anim[key] += ImGui::GetIO().DeltaTime * (animationPixels / animationDuration);
				if (g_anim[key] >= animationPixels)
				{
					g_anim[key] = animationPixels;
				}
			}
		}
		else
		{
			if (g_anim.find(key) != g_anim.end() && g_anim[key] > 0.0f)
			{
				g_anim[key] -= ImGui::GetIO().DeltaTime * (animationPixels / animationDuration);
				if (g_anim[key] < 0.0f)
				{
					g_anim[key] = 0.0f;
				}
			}
		}

		float t = g_anim[key] / animationPixels;
		return Animation::EaseInOut(start, end, t);
	}

	float GetAnimData(std::string key, float animationPixels, float animationDuration, float start = 0.f, float end = 1.f)
	{
		if (g_anim.find(key) == g_anim.end())
		{
			// If key not found in the map, add it with an initial value
			g_anim[key] = 0.0f;
		}

		if (g_anim[key] < 0.95f)
		{
			if (g_anim[key] < animationPixels)
			{
				g_anim[key] += ImGui::GetIO().DeltaTime * (animationPixels / animationDuration);
				if (g_anim[key] >= animationPixels)
				{
					g_anim[key] = animationPixels;
				}
			}
		}
		else
		{
			if (g_anim.find(key) != g_anim.end() && g_anim[key] > 0.0f)
			{
				g_anim[key] -= ImGui::GetIO().DeltaTime * (animationPixels / animationDuration);
				if (g_anim[key] < 0.0f)
				{
					g_anim[key] = 0.0f;
				}
			}
		}

		float t = g_anim[key] / animationPixels;
		return Animation::EaseInOut(start, end, t);
	}

	void resetAnimData(std::string key)
	{
		g_anim[key] = 0.0f;
	}
}

namespace Font
{
	ImFont* icon;
	ImFont* iconlarge;
	ImFont* sidebarIcon;
	ImFont* bigText;
	ImFont* overlayFont;
	ImFont* overlayFontBold;
	ImFont* fontawesome_duotone;

	void initializeFonts()
	{
		std::string path = std::getenv(xorstr_("USERPROFILE"));
		path += xorstr_("\\Documents");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Moe");
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += xorstr_("\\Fonts");
		if (!DoesPathExist(path))
			CreateFolder(path);

		ImGuiIO& io = ImGui::GetIO();

		ImFontConfig config;
		//Moved this to be the default font of ImGui
		config.SignedDistanceFont = true;
		io.Fonts->AddFontFromMemoryCompressedTTF(newFont_compressed_data, newFont_compressed_size, 14.f, &config);
		config.SignedDistanceFont = false;
		bigText = io.Fonts->AddFontFromMemoryCompressedTTF(newFont_compressed_data, newFont_compressed_size, 20.f, &config);
		ImFontConfig configOverlay;
		configOverlay.OversampleH = 3;
		configOverlay.OversampleV = 3;
		configOverlay.SignedDistanceFont = true;
		path = xorstr_("C:\\Windows\\Fonts\\");
		if (ctx.m_settings.Misc_Font == 0)
		{
			overlayFont = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\verdana.ttf")).c_str(), 14.f, &configOverlay);
			overlayFontBold = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\verdanab.ttf")).c_str(), 14.f, &configOverlay);
		}
		if (ctx.m_settings.Misc_Font == 1)
		{
			overlayFont = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\tahoma.ttf")).c_str(), 14.f, &configOverlay);
			overlayFontBold = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\tahomab.ttf")).c_str(), 14.f, &configOverlay);
		}
		if (ctx.m_settings.Misc_Font == 2)
		{
			overlayFont = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\arial.ttf")).c_str(), 14.f, &configOverlay);
			overlayFontBold = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\arialb.ttf")).c_str(), 14.f, &configOverlay);
		}
		if (ctx.m_settings.Misc_Font == 3)
		{
			overlayFont = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\comic.ttf")).c_str(), 14.f, &configOverlay);
			overlayFontBold = io.Fonts->AddFontFromFileTTF((path + xorstr_("\\comicb.ttf")).c_str(), 14.f, &configOverlay);
		}
		ImFontConfig config2;
		const ImWchar newIcon_Range[] = {NEWICON_MIN, NEWICON_MAX, 0};
		sidebarIcon = io.Fonts->AddFontFromMemoryCompressedTTF(newIcons_compressed_data, newIcons_compressed_size, 20.f, &config2, newIcon_Range);
		icon = io.Fonts->AddFontFromMemoryCompressedTTF(newIcons_compressed_data, newIcons_compressed_size, 16.f, &config2, newIcon_Range);
		iconlarge = io.Fonts->AddFontFromMemoryCompressedTTF(newIcons_compressed_data, newIcons_compressed_size, 40.f, &config2, newIcon_Range);
		fontawesome_duotone = io.Fonts->AddFontFromMemoryCompressedTTF(newIcons_compressed_data, newIcons_compressed_size, 40.f, &config2, newIcon_Range);

		ImFontConfig config3;
		config3.SignedDistanceFont = false;
		const ImWchar DuotoneIcon_Range[] = {ICON_DUOTONE_MIN, ICON_DUOTONE_MAX, 0};
		fontawesome_duotone = io.Fonts->AddFontFromMemoryCompressedTTF(fontawesome_duotone_compressed_data, fontawesome_duotone_compressed_size, 24.f, &config2, DuotoneIcon_Range);

		io.Fonts->Build();
	}
}

ImVec2 Size = {900, 500};

namespace UII
{
	ImGuiWindowFlags menuFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	bool open = true;

	void Start()
	{

		ImVec2 pos = ctx.m_settings.Visuals_Misc_GUIPos;

		ImGui::SetNextWindowSize(ImVec2 {Size.x, Size.y}, ImGuiCond_::ImGuiCond_Always);
		ImGui::SetNextWindowPos(pos, ImGuiCond_::ImGuiCond_FirstUseEver);

		ImGui::Begin("##main", &open, menuFlags);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f; //remove 1 pixel border around window
		style.WindowPadding = ImVec2(0.f, 0.f); //Remove the weird window padding
		style.ItemSpacing = ImVec2(0.f, 0.f);
		style.AntiAliasedFill = true;
		style.AntiAliasedLines = true;
	}

	void End()
	{
		ImGui::End();
	}

	//Basicly thebackground
	namespace MainWindow
	{
		void background()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = ImGui::GetWindowSize();
			drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), Colors::mainWindowBackground, 5);
		}
	}

	//Top part of the UI, the header as we should call it
	namespace Nav
	{
		ImVec2 navSize = {Size.x, 32};
		//HWND windowHWND = 0;

		bool notificationButton(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImColor color = Colors::grayText;

			bool ret = false;
			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + sizeAvail.x - 80.f, pos.y + (navSize.y / 2) - 9), ImVec2(pos.x + sizeAvail.x - 64.f, pos.y + (navSize.y / 2) - 6 + 16)))
			{
				color = Colors::whiteText;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					ret = true;
				}
			}

			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 80.f, pos.y + (navSize.y / 2) - 6), color, (const char*)ICON_BELL);
			ImGui::PopFont();

			return ret;
		}

		bool minimizeButton(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImColor color = Colors::grayText;
			bool ret = false;
			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + sizeAvail.x - 50.f, pos.y + (navSize.y / 2) - 9), ImVec2(pos.x + sizeAvail.x - 34.f, pos.y + (navSize.y / 2) - 6 + 16)))
			{
				color = Colors::whiteText;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					ret = true;
				}
			}

			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 47.f, pos.y + (navSize.y / 2) - 6), color, (const char*)ICON_MINUS);
			ImGui::PopFont();

			return ret;
		}

		bool ExitButton(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImColor color = Colors::grayText;
			bool ret = false;

			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + sizeAvail.x - 30.f, pos.y + (navSize.y / 2) - 9), ImVec2(pos.x + sizeAvail.x - 14.f, pos.y + (navSize.y / 2) - 6 + 16)))
			{
				color = Colors::whiteText;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					ret = true;
				}
			}

			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 25.f, pos.y + (navSize.y / 2) - 6), color, (const char*)ICON_XMARK);
			ImGui::PopFont();

			return ret;
		}

		void headerBackground()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			drawList->AddRectFilled(pos, ImVec2(pos.x + sizeAvail.x, pos.y + navSize.y), Colors::mainHeaderBackground, 5);
			drawList->AddRectFilled(pos, ImVec2(pos.x + sizeAvail.x - 50.f, pos.y + navSize.y), Colors::mainHeaderBackground, 5);
		}

		bool notification = false;

		void toggleMenu(bool* menuToggle)
		{
			*menuToggle = !*menuToggle;
		}

		void header()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			ImGui::BeginChild("Header", ImVec2(sizeAvail.x, navSize.y), false, ImGuiWindowFlags_NoScrollbar);
			{
				//Hide window
				if (minimizeButton(pos, sizeAvail))
				{
					toggleMenu(&ctx.m_settings.menu_menu_key);
					//ShowWindow(windowHWND, SW_MINIMIZE);
				}
				//Exit Process
				if (ExitButton(pos, sizeAvail))
				{
					exit(0);
				}
				//Notifications
				if (notificationButton(pos, sizeAvail))
				{
					notification = true;
				}

				//Seperator
				drawList->AddLine(ImVec2(pos.x + sizeAvail.x - 60.f, pos.y + 8.f), ImVec2(pos.x + sizeAvail.x - 60.f, pos.y + 22.f), Colors::headerSeperater);

				//Slogan?
			}
			ImGui::EndChild();
		}
	}

	namespace Sidebar
	{
		ImVec2 barSize = {200, Size.y};
		ImVec2 logoBarSize = {barSize.x, 105};
		ImVec2 barButtonSize = {barSize.x, 48};

		void sidebarBackground()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			drawList->AddRectFilled(pos, ImVec2(pos.x + barSize.x, pos.y + sizeAvail.y), Colors::mainSidebarBackground, 5);
			drawList->AddRectFilled(ImVec2(pos.x + barSize.x / 2, pos.y), ImVec2(pos.x + barSize.x, pos.y + sizeAvail.y), Colors::mainSidebarBackground, 5);
			drawList->Flags = ImDrawListFlags_AntiAliasedFill;
			//Points of where we drawings
			ImVec2 verts3[] = {ImVec2(pos.x + barSize.x - 110.f, pos.y), ImVec2(pos.x + barSize.x - 60.f, pos.y + 70), ImVec2(pos.x + barSize.x - 20, pos.y)};
			drawList->AddConvexPolyFilled(verts3, 3, Colors::sidebarRect3);

			ImVec2 verts2[] = {ImVec2(pos.x + barSize.x - 130.f, pos.y), ImVec2(pos.x + barSize.x - 35.f, pos.y + 55), ImVec2(pos.x + barSize.x, pos.y + 35), ImVec2(pos.x + barSize.x, pos.y)};
			drawList->AddConvexPolyFilled(verts2, 4, Colors::sidebarRect2);

			ImVec2 verts[] = {ImVec2(pos.x + barSize.x - 145.f, pos.y), ImVec2(pos.x + barSize.x - 60.f, pos.y + 30), ImVec2(pos.x + barSize.x, pos.y + 15), ImVec2(pos.x + barSize.x, pos.y)};
			drawList->AddConvexPolyFilled(verts, 4, Colors::sidebarRect1);
		}

		void logo(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			/*static bool loaded;
			static int my_image_width = 0;
			static int my_image_height = 0;
			static ID3D11ShaderResourceView* logoTexture = NULL;
			std::string userProfile = getenv("USERPROFILE");
			std::string pathh = (userProfile + "\\Documents\\" + productName + "\\");
			std::string path = (pathh + "\\logo.png");
			if (!loaded) {

				//UI::LoadTextureFromFile(path.c_str(), &logoTexture, &my_image_width, &my_image_height);
				loaded = true;
			}		  */
			//drawList->AddImage(logoTexture, ImVec2(pos.x - 5.f, pos.y - 10.f), ImVec2(pos.x + 75, pos.y + 70), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
			//drawList->AddRectFilled(ImVec2(pos.x + 10, pos.y), ImVec2(pos.x + 60, pos.y + 50), ImColor(255, 255, 0, 255));

			std::string productSlogan = "Rust DMA";

			drawList->AddText(ImVec2(pos.x + 65, pos.y + 20), Colors::whiteText, productName.c_str());
			drawList->AddText(ImVec2(pos.x + 65, pos.y + 20), Colors::whiteText, productName.c_str());
			drawList->AddText(ImVec2(pos.x + 65, pos.y + 35), Colors::userTag, productSlogan.c_str());
			drawList->AddText(ImVec2(pos.x + 65, pos.y + 35), Colors::userTag, productSlogan.c_str());
		}

		void clock(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			std::time_t current_time = std::time(nullptr);

			// Create a struct to hold the formatted time
			std::tm local_time;
			localtime_s(&local_time, &current_time); // Windows-specific, for other platforms use localtime

			// Format the time as "10:32 AM"
			char time_buffer[12]; // Buffer for the formatted time
			std::strftime(time_buffer, sizeof(time_buffer), "%I:%M:%S %p", &local_time);

			std::string currentTime = time_buffer;
			ImVec2 timeSize = ImGui::CalcTextSize(currentTime.c_str());

			drawList->AddRectFilled(ImVec2(pos.x + ((barSize.x / 2) - (timeSize.x / 2)) - 10.f, pos.y + 65.f), ImVec2(pos.x + (barSize.x / 2) + (timeSize.x / 2) + 10.f, pos.y + 95.f), Colors::mainSidebarClockBackground, 5.f);
			drawList->AddText(ImVec2(pos.x + ((barSize.x / 2) - (timeSize.x / 2)), pos.y + 75.f), Colors::grayText, currentTime.c_str());
		}

		//TODO: make a nice animation that will ease in the config system.
		void configButton(ImVec2 pos)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();;
			ImGui::PushFont(Font::sidebarIcon);
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y - Nav::navSize.y + 5.f), ImVec2(pos.x + 35.f, (pos.y - (Nav::navSize.y + 5.f)) + 40.f), Colors::mainSidebarClockBackground, 5.f);
			//Align text middle
			ImColor color = Colors::grayText;
			if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + 5.f, pos.y - Nav::navSize.y + 5.f), ImVec2(pos.x + 35.f, (pos.y - (Nav::navSize.y + 5.f)) + 40.f)))
			{
				color = Colors::whiteText;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
				}
			}
			drawList->AddText(ImVec2(pos.x + 11.f, (pos.y - (Nav::navSize.y + 5.f)) + 15), color, (const char*)ICON_BARS);
			ImGui::PopFont();
		}

		void logoAndClock()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			//configButton(pos);
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			ImGui::BeginChild("Sidebar Logo And clock", ImVec2(logoBarSize.x, logoBarSize.y), false);
			{
				clock(pos, sizeAvail);
				logo(pos, sizeAvail);
			}
			ImGui::EndChild();
		}

		void userprofile(/*Fuser fuser*/)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImGui::BeginChild("SidebarProfile", ImVec2(barSize.x, sizeAvail.y), false);
			{
				//#ifndef RESELLER
				//				pos = ImGui::GetCursorScreenPos();
				//				sizeAvail = ImGui::GetContentRegionAvail();
				//
				//				//Modal background for userprofile
				//				drawList->AddRectFilled(ImVec2(pos.x + 10.f, pos.y + 10.f), ImVec2(pos.x + sizeAvail.x - 10.f, pos.y + sizeAvail.y - 30.f), Colors::mainSidebarClockBackground, 5.f);
				//				//Users Profile (Discord)
				//
				//				//GetDiscord Profile
				//				static bool once = false;
				//				static ID3D11ShaderResourceView* discordPfpTexture11 = NULL;
				//				static ID3D12Resource* discordPfpTexture12 = NULL;
				//				if (!once && Discord::grabbed)
				//				{
				//					std::string path = (std::string)getenv("USERPROFILE") + "\\discord.jpg";
				//
				//					std::string url = std::string("https://cdn.discordapp.com/avatars/" + (std::string)Discord::userId + "/" + (std::string)Discord::avatarHash + ".png");
				//					URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);
				//
				//					int imagewidth, imageheight; ///unused
				//					if (fuser.GetType() == EType::TYPE_DX12)
				//					{
				//						auto g_pd3dDevice = fuser.GetDevice12();
				//						auto g_pd3dSrvDescHeap = fuser.GetSrvHeap12();
				//						UINT handle_increment = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				//						int descriptor_index = 1; // The descriptor table index to use (not normally a hard-coded constant, but in this case we'll assume we have slot 1 reserved for us)
				//						D3D12_CPU_DESCRIPTOR_HANDLE my_texture_srv_cpu_handle = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
				//						my_texture_srv_cpu_handle.ptr += (handle_increment * descriptor_index);
				//						D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();
				//						my_texture_srv_gpu_handle.ptr += (handle_increment * descriptor_index);
				//
				//						// Load the texture from a file
				//						bool ret = fuser.LoadTextureFromFile12(path.c_str(), my_texture_srv_cpu_handle, &discordPfpTexture12, &imagewidth, &imageheight);
				//					}
				//					else
				//					{
				//						fuser.LoadTextureFromFile11(path.c_str(), &discordPfpTexture11, &imagewidth, &imageheight);
				//					}
				//					once = true;
				//				} //TODO: Set default picture later
				//
				//				if (fuser.GetType() == EType::TYPE_DX12)
				//				{
				//					if (discordPfpTexture12 != NULL)
				//						drawList->AddImageRounded(discordPfpTexture12, ImVec2(pos.x + 15.f, pos.y + 15.f), ImVec2(pos.x + 65.f, pos.y + sizeAvail.y - 35.f), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), 5.f);
				//				}
				//				else
				//				{
				//					if (discordPfpTexture11 != NULL)
				//						drawList->AddImageRounded(discordPfpTexture11, ImVec2(pos.x + 15.f, pos.y + 15.f), ImVec2(pos.x + 65.f, pos.y + sizeAvail.y - 35.f), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), 5.f);
				//				}
				//				ImColor connected = Colors::userDisconnected;
				//				std::string user = "User";
				//				std::string tag = "#0000";
				//				std::string userId = "Not Connected";
				//				if (Discord::grabbed)
				//				{
				//					connected = Colors::userConnected;
				//					user = Discord::username;
				//					tag = "#" + (std::string)Discord::discrim;
				//					userId = /*Discord::userId*/"User Connected";
				//				}
				//
				//				//Users name & tag
				//				ImVec2 userSize = ImGui::CalcTextSize(user.c_str());
				//				drawList->AddText(ImVec2(pos.x + 70.f, pos.y + 25.f), Colors::grayText, user.c_str());
				//				drawList->AddText(ImVec2(pos.x + 70.f + userSize.x, pos.y + 25.f), Colors::userTag, tag.c_str());
				//
				//				//User Status
				//				drawList->AddText(ImVec2(pos.x + 70.f, pos.y + 45.f), Colors::userTag, userId.c_str());
				//
				//				//RPC connected?
				//				drawList->AddText(ImVec2(pos.x + 10.f, pos.y + sizeAvail.y - 20.f), Colors::whiteText, "Discord RPC Connection");
				//				//If connected = green else red
				//
				//				ImGui::PushFont(Font::sidebarIcon);
				//				drawList->AddText(ImVec2(pos.x + sizeAvail.x - 30.f, pos.y + sizeAvail.y - 30.f), connected, (const char*)ICON_WIFI_WEAK);
				//				ImGui::PopFont();
				//#endif
			}
			ImGui::EndChild();
		}

		void startSidebar()
		{
			logoAndClock();
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImGui::BeginChild("Sidebar", ImVec2(barSize.x, sizeAvail.y - /*100*/ 50), false);
			pos = ImGui::GetCursorScreenPos();
			sizeAvail = ImGui::GetContentRegionAvail();

			//drawList->AddRectFilled(pos, ImVec2(pos.x + barSize.x, pos.y + sizeAvail.y), ImColor(255, 255, 255, 255));
		}

		//ImColor sidebarBackground = ImColor(6, 6, 10, 130);
		//ImColor sidebarBackgroundBar = ImColor(94, 255, 176, 130);
		//ImColor sidebarIcon = ImColor(94, 255, 176, 255);

		bool starthovering = false;
		std::string isAtMenu = "";
		std::string isActive = "";

		void button(std::string name, std::string icon, float customIconPadding)
		{
			bool hovering = false;
			bool selected = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			//Color shit
			ImColor colorBg = ImColor(6, 6, 10, 0);
			ImColor colorBgBar = ImColor(94, 255, 176, 0);
			ImColor colorIcon = ImColor(94, 255, 176, 80);
			ImColor colorText = Colors::grayText;

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y));
			pos = ImGui::GetCursorScreenPos();

			if (ImGui::InvisibleButton(name.c_str(), ImVec2(barSize.x - 10.f, barButtonSize.y)))
			{
			}

			if (ImGui::IsItemHovered())
			{
				hovering = true;
				isAtMenu = name;
			}

			float alpha = Animation::GetAnimData(name, hovering, 30, 0.7f, 0.35, 1.f);
			colorIcon = ImColor(94, 255, 176, (int)(alpha * 200));
			colorText = ImColor(225, 225, 225, (int)(alpha * 255));

			if (ImGui::IsItemClicked())
			{
				navigation::setActive(name);
				isActive = name;
			}
			if (navigation::isActive(name))
				selected = true;

			float selectedalpha = Animation::GetAnimData(name + "sel", selected, 30, 0.7f);
			float alphabg = Animation::GetAnimData(name + "bg", selected, 30, 0.7f);
			if (selected || selectedalpha > 0.05f || alphabg > 0.05f)
			{
				if (selectedalpha < alpha)
					selectedalpha = alpha;
				colorBg = ImColor(6, 6, 10, (int)(alphabg * 130));
				colorBgBar = ImColor(94, 255, 176, (int)(alphabg * 180));
				colorIcon = ImColor(94, 255, 176, (int)(selectedalpha * 200));
				colorText = ImColor(225, 225, 225, (int)(selectedalpha * 255));
			}

			//Background
			drawList->AddRectFilled(pos, ImVec2(pos.x + ImGui::GetItemRectSize().x, pos.y + barButtonSize.y), colorBg, 5);
			//Background selected side bar thingy
			drawList->AddRectFilled(ImVec2(pos.x + ImGui::GetItemRectSize().x - 3.f, pos.y), ImVec2(pos.x + ImGui::GetItemRectSize().x, pos.y + barButtonSize.y), colorBgBar);

			//Logo
			ImVec2 iconSize = ImGui::CalcTextSize(icon.c_str());
			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + (barButtonSize.y / 2) - customIconPadding), ImColor(140, 234, 255, 80), icon.c_str()); //Placeholder behind 
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + (barButtonSize.y / 2) - customIconPadding), colorIcon, icon.c_str());
			ImGui::PopFont();

			//Text
			ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
			drawList->AddText(ImVec2(pos.x + 35.f, pos.y + (barButtonSize.y / 2) - (textSize.y / 2)), Colors::grayText, name.c_str()); //Placeholder behind
			drawList->AddText(ImVec2(pos.x + 35.f, pos.y + (barButtonSize.y / 2) - (textSize.y / 2)), colorText, name.c_str());
		}

		void endSidebar(/*Fuser fuser*/)
		{
			ImGui::EndChild();
			//Render userprofile

			userprofile(/*fuser*/);
		}
	}

	namespace Modal
	{
		ImVec2 tempPos = {0, 0};

		ImVec2 modalNav = {0, 32};

		void modalBackground(float alpha)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = ImGui::GetWindowSize();

			float a = alpha * 255;
			ImColor mainWindowBackground = ImColor(26, 26, 38, (int)a);
			drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), mainWindowBackground, 10.f);
		}

		bool ExitButton(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImColor color = Colors::grayText;
			bool ret = false;

			if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + sizeAvail.x - 30.f, pos.y + (modalNav.y / 2) - 9), ImVec2(pos.x + sizeAvail.x - 14.f, pos.y + (modalNav.y / 2) - 6 + 16)))
			{
				color = Colors::whiteText;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					ret = true;
				}
			}

			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 25.f, pos.y + (modalNav.y / 2) - 8), color, (const char*)ICON_XMARK);
			ImGui::PopFont();

			return ret;
		}

		void modalHeaderName(std::string name, ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());

			drawList->AddText(ImVec2(pos.x + ((sizeAvail.x / 2) - (nameSize.x / 2)), pos.y + ((modalNav.y / 2) - (nameSize.y / 2))), Colors::whiteText, name.c_str());
		}

		void modalHeader(std::string name, ImVec2 pos, ImVec2 sizeAvail, bool* open)
		{
			ImGui::BeginChild("ModalHeader", ImVec2(sizeAvail.x, modalNav.y));
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(pos, ImVec2(pos.x + sizeAvail.x, pos.y + modalNav.y), Colors::mainHeaderBackground, 10.f);
				drawList->AddRectFilled(ImVec2(pos.x, pos.y + 10.f), ImVec2(pos.x + sizeAvail.x, pos.y + modalNav.y), Colors::mainHeaderBackground);

				if (ExitButton(pos, sizeAvail))
				{
					Animation::resetAnimData(name + "modal");
					Custom::modalOpen = !Custom::modalOpen;
					*open = !*open;
				}

				//Window Name
				modalHeaderName(name, pos, sizeAvail);
			}
			ImGui::EndChild();
		}

		void blurMainWindow(ImVec2 pos, float alpha)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 size = ImGui::GetWindowSize();

			float a = alpha * 210;

			drawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(22, 22, 22, (int)a));
		}

		bool clickedOffModal(ImVec2 pos)
		{
			ImVec2 WindowSize = ImGui::GetWindowSize();
			bool ret = false;

			if (!ImGui::IsMouseHoveringRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + WindowSize.x, pos.y + WindowSize.y)))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					ret = true;
				}
			}

			return ret;
		}

		void startModal(std::string name, bool* open, ImVec2 pos, std::string windowName, ImVec2 size)
		{
			float alpha = Animation::GetAnimData(name + "modal", true, 20.f, 0.8f);
			//"blur" the main window
			ImGui::SetNextWindowPos(ImVec2(tempPos.x, tempPos.y), ImGuiCond_Appearing, ImVec2(0.f, 0.f));
			ImGui::SetNextWindowSize(ImVec2(Size.x, Size.y));
			ImGui::Begin((name + "2").c_str(), open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
			blurMainWindow(pos, alpha);
			ImGui::End();

			size = {size.x, size.y + 32}; //Add Header size

			if (windowName == "") windowName = name;

			//ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(ImVec2(tempPos.x + (Size.x / 2), tempPos.y + (Size.y / 2)), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(size);
			ImGui::SetNextWindowBgAlpha(alpha);
			ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
			ImVec2 curPos = ImGui::GetCursorScreenPos();
			ImVec2 availSize = ImGui::GetContentRegionAvail();

			Custom::modalOpen = open;

			//Draw background
			modalBackground(alpha);

			//Close if clicked outside of the modal
			if (alpha > 0.9)
			{
				if (clickedOffModal(curPos))
				{
					Animation::resetAnimData(name + "modal");
					Custom::modalOpen = !Custom::modalOpen;
					*open = !*open;
				}
			}

			//Header
			modalHeader(windowName, curPos, availSize, open);

			curPos = ImGui::GetCursorScreenPos();
			availSize = ImGui::GetContentRegionAvail();
			ImGui::SetCursorScreenPos(ImVec2(curPos.x + 5.f, curPos.y + 5.f));
			//Padding so it doesnt stick to the edges
			ImGui::BeginChild(name.c_str(), ImVec2(availSize.x - 5.f, availSize.y - 5.f));
		}

		void endModal()
		{
			ImGui::EndChild();
			ImGui::End();
		}
	}

	namespace Tooltip
	{
		std::vector<std::string> splitIntoLines(const std::string& data, size_t maxCharsPerLine)
		{
			std::vector<std::string> lines;
			std::istringstream stream(data);
			std::string word;
			std::string currentLine;

			while (stream >> word)
			{
				if (currentLine.length() + word.length() > maxCharsPerLine)
				{
					lines.push_back(currentLine);
					currentLine = word;
				}
				else
				{
					if (!currentLine.empty())
					{
						currentLine += " ";
					}
					currentLine += word;
				}
			}

			if (!currentLine.empty())
			{
				lines.push_back(currentLine);
			}

			return lines;
		}

		void tip(std::string data)
		{
			const size_t maxCharsPerLine = 50;
			std::vector<std::string> lines = splitIntoLines(data, maxCharsPerLine);

			ImGui::Begin("##", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 size = ImVec2(0, 0);

				ImColor background = ImColor(20, 20, 20, 255);
				for (const std::string& line : lines)
				{
					ImVec2 textSize = ImGui::CalcTextSize(line.c_str());
					size.x = std::max(size.x, textSize.x);
					size.y += textSize.y;
				}

				drawList->AddRectFilled(ImVec2(pos.x - 2.f, pos.y - 2.f), ImVec2(pos.x + (size.x + 4.f), pos.y + (size.y + 4.f)), background);

				for (const std::string& line : lines)
				{
					ImGui::Text("%s", line.c_str());
				}
			}
			ImGui::End();
		}
	}

	namespace Config
	{
		ImVec2 configSize = {130, 0}; //{160, 0};
		std::vector<std::string> configs;
		int currentConfig = 0;

		bool saveConfigModal = false;

		void getAllConfigs()
		{
			configs = ctx.m_settings.get_all_configs();
		}

		void deleteConfig(int index)
		{
			if (index == currentConfig)
			{
				if (!ctx.m_settings.exists("default.conf"))
					ctx.m_settings.save("default.conf");
				ctx.m_settings.load("default.conf");
			}

			ctx.m_settings._delete(configs[index].c_str());
			configs = ctx.m_settings.get_all_configs();
			if (index == currentConfig)
			{
				//find default config index
				auto it = std::find(configs.begin(), configs.end(), "default.conf");

				if (it != configs.end())
				{
					std::size_t index = std::distance(configs.begin(), it);
					currentConfig = index;
				}
				else
					currentConfig = 0;
			}
		}

		void createConfig(std::string name)
		{
			if (strlen(name.c_str()) < 1 /*|| !FileBrowser::valid_filename(name)*/)
				return;

			for (size_t i = 0; i < configs.size(); ++i)
			{
				// gotta make sure there are no issues with for example trying to create "nigga1" when theres already "nigga" etc
				if (strlen(name.c_str()) == strlen(configs[i].c_str()) && strncmp(configs[i].c_str(), name.c_str(), configs[i].size()) == 0)
					return;
			}

			// create new config
			configs.push_back(name);
			ctx.m_settings.save(name.c_str());
			configs = ctx.m_settings.get_all_configs();
			// set current cfg to newest, size counts from 1
			currentConfig = configs.size() - 1;
		}

		void grayLineBackground()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			drawList->AddLine(ImVec2(pos.x + UII::Sidebar::barSize.x, pos.y), ImVec2(pos.x + UII::Sidebar::barSize.x, pos.y + sizeAvail.y), Colors::headerSeperater);
			drawList->AddLine(ImVec2(pos.x + (UII::Sidebar::barSize.x + configSize.x), pos.y), ImVec2(pos.x + (UII::Sidebar::barSize.x + configSize.x), pos.y + sizeAvail.y), Colors::headerSeperater);
		}

		void drawBackground()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			drawList->AddRectFilled(ImVec2(pos.x + UII::Sidebar::barSize.x, pos.y), ImVec2(pos.x + (UII::Sidebar::barSize.x + configSize.x), pos.y + sizeAvail.y), Colors::mainSidebarBackground2);
		}

		void configHeader()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImGui::SetCursorScreenPos(ImVec2(pos.x + UII::Sidebar::barSize.x, pos.y - Size.y));

			ImGui::BeginChild("Configs", ImVec2(configSize.x, 125));
			{
				pos = ImGui::GetCursorScreenPos();
				ImGui::PushFont(Font::bigText);
				drawList->AddText(ImVec2(pos.x + 10.f, pos.y + 45.f), ImColor(255, 255, 255, 255), xorstr_("Configs"));
				ImGui::PopFont();

				drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 70.f), ImVec2(pos.x + configSize.x - 5.f, pos.y + 115.f), ImColor(0, 0, 0, 50), 5.f);
				drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 75.f), Colors::whiteText, xorstr_("Current Config"));

				ImColor color = ImColor(255, 255, 255, 100);
				if (Custom::IsMouseHoveringRect(ImVec2(pos.x + configSize.x - 35.f, pos.y + 35.f), ImVec2(pos.x + configSize.x - 5.f, pos.y + 65.f)))
				{
					color = ImColor(255, 255, 255, 150);
					if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
					{
						color = ImColor(255, 255, 255, 200);
					}
					//Save Config // Opens a Modal check Render.cpp bottom of script
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						saveConfigModal = true;
					}
				}

				//Save button  background
				drawList->AddRectFilled(ImVec2(pos.x + configSize.x - 35.f, pos.y + 35.f), ImVec2(pos.x + configSize.x - 5.f, pos.y + 65.f), ImColor(0, 0, 0, 50), 5.f);

				ImGui::PushFont(Font::icon);
				drawList->AddText(ImVec2(pos.x + configSize.x - (35.f / 2.f) - 10.5f, pos.y + (35.f * 1.5) - 11.5f), color, (const char*)ICON_DOWNLOAD);
				ImGui::PopFont();

				//Line left of the configs name
				drawList->AddLine(ImVec2(pos.x + 15.f, pos.y + 92.f), ImVec2(pos.x + 15.f, pos.y + 107.5f), ImColor(94, 255, 176, 255), 2.f);

				std::string config = xorstr_("N/A");
				if (!configs.empty())
					if (!configs[currentConfig].empty())
						config = configs[currentConfig];

				drawList->AddText(ImVec2(pos.x + 20.f, pos.y + 95.f), Colors::userTag, config.c_str());

				//seperator
				drawList->AddLine(ImVec2(pos.x + 15.f, pos.y + 125.f), ImVec2(pos.x + configSize.x - 15.f, pos.y + 125.f), Colors::headerSeperater);
			}
			ImGui::EndChild();
		}

		void beginConfig()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			drawBackground();
			configHeader();
			ImGui::SetCursorScreenPos(ImVec2(pos.x + UII::Sidebar::barSize.x, pos.y - Size.y));
			ImGui::BeginChild("Config", ImVec2(configSize.x, sizeAvail.y - 10.f), 0);
			//drawList->AddText(ImVec2(pos.x + (UI::Sidebar::barSize.x + 10.f), pos.y + 150), ImColor(255, 255, 255, 255), "Configs");
		}

		void configOption(std::string name, int index)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			//Initial padding between begin Config header to seperater 10 pixel under
			float padding = 136.f;
			ImColor colorDownload = ImColor(77, 157, 227, 100);
			ImColor colorTrashbin = ImColor(235, 68, 76, 100);
			ImColor colorSave = ImColor(209, 194, 61, 100);

			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + padding), ImVec2(pos.x + configSize.x - 5.f, pos.y + (padding + 55.f)), ImColor(0, 0, 0, 50), 5.f);

			//Config Title
			drawList->AddText(ImVec2(pos.x + 10.f, pos.y + padding + 7.f), Colors::whiteText, name.c_str());

			//Load Button
			drawList->AddRectFilled(ImVec2(pos.x + 10.f, pos.y + (padding + 25.f)), ImVec2(pos.x + 35.f, pos.y + (padding + 50.f)), ImColor(0, 0, 0, 100), 5.f);
			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + 15.f, pos.y + (padding + 25.f)), ImVec2(pos.x + 35.f, pos.y + (padding + 50.f))))
			{
				colorDownload = ImColor(77, 157, 227, 150);
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					colorDownload = ImColor(77, 157, 227, 200);
				}
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					//Load Config
					currentConfig = index;
					ctx.m_settings.load((name).c_str());
				}
				Tooltip::tip("Load");
			}
			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + 16.25f, pos.y + (padding + 30.75)), colorDownload, (const char*)ICON_UPLOAD);
			ImGui::PopFont();

			//Save Button
			drawList->AddRectFilled(ImVec2(pos.x + 40.f, pos.y + (padding + 25.f)), ImVec2(pos.x + 65.f, pos.y + (padding + 50.f)), ImColor(0, 0, 0, 100), 5.f);
			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + 40.f, pos.y + (padding + 25.f)), ImVec2(pos.x + 65.f, pos.y + (padding + 50.f))))
			{
				colorSave = ImColor(209, 194, 61, 150);
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					colorSave = ImColor(209, 194, 61, 200);
				}
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					//Save Config
					ctx.m_settings.save((UII::Config::configs[index]).c_str());
					//	configs = ctx.m_settings.get_all_configs();
				}
				Tooltip::tip("Save");
			}
			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + 46.25f, pos.y + (padding + 31.25)), colorSave, (const char*)ICON_DOWNLOAD);
			ImGui::PopFont();

			//Delete Button
			drawList->AddRectFilled(ImVec2(pos.x + 70.f, pos.y + (padding + 25.f)), ImVec2(pos.x + 95.f, pos.y + (padding + 50.f)), ImColor(0, 0, 0, 100), 5.f);
			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + 70.f, pos.y + (padding + 25.f)), ImVec2(pos.x + 95.f, pos.y + (padding + 50.f))))
			{
				colorTrashbin = ImColor(235, 68, 76, 150);
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					colorTrashbin = ImColor(235, 68, 76, 200);
				}
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					//Delete Config
					deleteConfig(index);
				}
				Tooltip::tip("Delete");
			}
			ImGui::PushFont(Font::icon);
			drawList->AddText(ImVec2(pos.x + 76.25f, pos.y + (padding + 31.25)), colorTrashbin, (const char*)ICON_TRASH_CAN);
			ImGui::PopFont();

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 60.f));
		}

		void endConfig()
		{
			ImGui::EndChild();
		}
	}

	namespace Controls
	{
		//Give amount of sizes & how many in total
		ImVec2 sectionNav = {0, 32};

		void drawBackground()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = ImGui::GetWindowSize();

			drawList->AddRectFilled(ImVec2(pos.x + 1, pos.y + 1), ImVec2(pos.x + size.x - 1, pos.y + size.y - 1), ImColor(14, 14, 23, 255), 10.f);
		}

		void drawBackgroundColor(float alpha)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = ImGui::GetWindowSize();

			float a = alpha * 255;

			drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(94, 255, 176, (int)a), 10.f);
		}

		bool isHoveringGroup(ImVec2 pos)
		{
			ImVec2 WindowSize = ImGui::GetWindowSize();
			bool ret = false;

			if (Custom::IsMouseHoveringRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + WindowSize.x, pos.y + WindowSize.y)))
			{
				ret = true;
			}

			return ret;
		}

		void sectionHeaderName(std::string name, ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());

			drawList->AddText(ImVec2(pos.x + ((sizeAvail.x / 2) - (nameSize.x / 2)), pos.y + ((sectionNav.y / 2) - (nameSize.y / 2)) + 1), Colors::whiteText, name.c_str());
		}

		void sectionHeader(std::string name, ImVec2 pos, ImVec2 sizeAvail)
		{
			ImGui::BeginChild("SectionHeader", ImVec2(sizeAvail.x, sectionNav.y));
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(ImVec2(pos.x + 1, pos.y + 1), ImVec2(pos.x + sizeAvail.x - 1, pos.y + sectionNav.y), ImColor(3, 3, 12, 255), 10.f);
				drawList->AddRectFilled(ImVec2(pos.x + 1, pos.y + 10.f), ImVec2(pos.x + sizeAvail.x - 1, pos.y + sectionNav.y), ImColor(3, 3, 12, 255));

				//Window Name
				sectionHeaderName(name, pos, sizeAvail);
			}
			ImGui::EndChild();
		}

		//Size value is hardcoded
		ImVec2 lastSize;

		void beginSection(std::string name, ImVec2 size)
		{
			lastSize = size;
			ImGui::BeginChild(name.c_str(), ImVec2(size.x, size.y), 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);

			ImVec2 curPos = ImGui::GetCursorScreenPos();
			ImVec2 availSize = ImGui::GetContentRegionAvail();

			float alpha = Animation::GetAnimData(name + "section", isHoveringGroup(curPos), 20.f, 0.8f);

			drawBackgroundColor(alpha);
			drawBackground();
			sectionHeader(name, curPos, availSize);

			ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(94, 255, 176, 0));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(94, 255, 176, 180));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(94, 255, 176, 200));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(94, 255, 176, 255));
			ImGui::GetStyle().ScrollbarSize = 3.f;
			ImGui::BeginChild(name.c_str(), ImVec2(size.x - 5.f, size.y - 50.f), 0);
		}

		ImVec2 lastPos;

		void endSection()
		{
			ImGui::EndChild();
			lastPos = ImGui::GetCursorScreenPos();

			lastPos = ImVec2(lastPos.x, lastPos.y - 10.f);
			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(4);
		}

		void nextSection()
		{
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			//Cuz default padding is a pixel of 2, to make it 10.f we add 8.f
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 8.f, ContainerPos.y + 9.f));
		}

		void sameSection()
		{
			//Cuz default padding is a pixel of 2, to make it 10.f we add 8.f
			ImGui::SetCursorScreenPos(ImVec2(lastPos.x, lastPos.y + 36));
		}

		bool Toggle(std::string name, std::string description, bool* toggle)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);

			//Title
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 15.f), Colors::whiteText, name.c_str());

			ImVec2 txtSize = ImGui::CalcTextSize(description.c_str());
			std::string desc = description;
			bool needTooltip = false;
			if (pos.x + 15.f + txtSize.x > pos.x + sizeAvail.x - 45.f)
			{
				needTooltip = true;
				while (pos.x + 15.f + txtSize.x > pos.x + sizeAvail.x - 45.f)
				{
					//Remove 4 last characters
					desc = desc.substr(0, desc.size() - 4);
					desc.append("...");
					txtSize = ImGui::CalcTextSize(desc.c_str());
				}
			}

			if (Custom::IsMouseHoveringRect(pos, pos + ImVec2(sizeAvail.x, 50.f)))
			{
				if (needTooltip)
					Tooltip::tip(description);
			}

			//Description
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 30.f), Colors::userTag, desc.c_str());

			ImColor backgroundCorner = ImColor(60, 60, 60, 255);
			bool ret = false;

			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + sizeAvail.x - 40.f, pos.y + 15.f), ImVec2(pos.x + sizeAvail.x - 15.f, pos.y + 40.f)))
			{
				hovering = true;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					*toggle = !*toggle;
					ret = true;
				}
			}
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f, 0.3f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 100));

			ImGui::PushFont(Font::sidebarIcon);
			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 40.f, pos.y + 17.f), backgroundCorner, (const char*)ICON_SQUARE);
			ImGui::PopFont();

			if (*toggle)
			{
				backgroundCorner = ImColor(94, 255, 176, 255);
				ImGui::PushFont(Font::sidebarIcon);
				drawList->AddText(ImVec2(pos.x + sizeAvail.x - 40.f, pos.y + 17.f), backgroundCorner, (const char*)ICON_SQUARE_CHECK);
				ImGui::PopFont();
			}

			///Old toggle style

			//drawList->AddRectFilled(ImVec2(pos.x + sizeAvail.x - 40.f, pos.y + 15.f), ImVec2(pos.x + sizeAvail.x - 15.f, pos.y + 40.f), backgroundCorner, 5); //Border Line
			//drawList->AddRectFilled(ImVec2(pos.x + sizeAvail.x - (40.f - 1.f), pos.y + (15.f + 1.f)), ImVec2(pos.x + sizeAvail.x - (15.f + 1.f), pos.y + (40.f - 1.f)), ImColor(60, 60, 60, 255), 5); //background space

			//ImGui::PushFont(Font::sidebarIcon);
			//drawList->AddText(ImVec2(pos.x + sizeAvail.x - 35.f, pos.y + 20.f), backgroundCorner, (const char*)ICON_CHECK_CIRCLE);
			//ImGui::PopFont();
			//drawList->AddRectFilled(ImVec2(pos.x + sizeAvail.x - 35.f, pos.y + 20.f), ImVec2(pos.x + sizeAvail.x - 20.f, pos.y + 35.f), backgroundCorner, 5); //Inside Space

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 50.f));
			return ret;
		}

		bool ColorPicker(std::string name, std::string description, ImColor* color)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);

			//Title
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 15.f), Colors::whiteText, name.c_str());
			//Description
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 30.f), Colors::userTag, description.c_str());
			ImVec4 colorr = color->Value;
			ImGui::SetCursorScreenPos(ImVec2(pos.x + sizeAvail.x - 40.f, pos.y + 17.5f));
			ImGui::PushID(name.c_str());

			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor(22, 22, 22, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(40, 40, 40, 150).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(60, 60, 60, 200).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(60, 60, 60, 255).Value);
			ImGui::ColorEdit4("", (float*)&colorr, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB);
			ImGui::PopStyleColor(4);
			ImGui::PopID();
			color->Value = colorr;

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 50.f));
			return true;
		}

		bool ColorPicker(std::string name, std::string description, ImColor* color, ImColor* color2)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);

			//Title
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 15.f), Colors::whiteText, name.c_str());
			//Description
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 30.f), Colors::userTag, description.c_str());
			ImVec4 colorr = color->Value;
			ImGui::SetCursorScreenPos(ImVec2(pos.x + sizeAvail.x - 40.f, pos.y + 17.5f));
			ImGui::PushID(name.c_str());

			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor(22, 22, 22, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(40, 40, 40, 150).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(60, 60, 60, 200).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(60, 60, 60, 255).Value);
			ImGui::ColorEdit4("", (float*)&colorr, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB);
			ImGui::PopStyleColor(4);
			if (ImGui::IsItemHovered())
			{
				Tooltip::tip("Visible Colour");
			}

			ImVec4 colorr2 = color2->Value;
			ImGui::SetCursorScreenPos(ImVec2(pos.x + sizeAvail.x - 65.f, pos.y + 17.5f));
			ImGui::PushID((name + "2").c_str());

			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor(22, 22, 22, 255).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(40, 40, 40, 150).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(60, 60, 60, 200).Value);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(60, 60, 60, 255).Value);
			ImGui::ColorEdit4("", (float*)&colorr2, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB);
			ImGui::PopStyleColor(4);
			ImGui::PopID();
			if (ImGui::IsItemHovered())
			{
				Tooltip::tip("Not-Visible Colour");
			}

			//	ImGui::SetCursorScreenPos(ImVec2(pos.x - sizeAvail.x + 40.f, pos.y - 25.f);
			color->Value = colorr;
			color2->Value = colorr2;

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 50.f));
			return true;
		}

		bool Button2(std::string name, std::string description)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->Flags = ImDrawListFlags_AntiAliasedFill;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			ImColor background = ImColor(66, 66, 66, 25);
			bool ret = false;
			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f)))
			{
				hovering = true;
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
					ret = true;
			}
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);

			float a1 = alpha * 50;
			if (a1 < 25)
				a1 = 25;
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));
			background = ImColor(66, 66, 66, (int)a1);

			if (ret && hovering)
			{
				background = ImColor(66, 66, 66, (int)(alpha * 80));
			}

			//Background
			drawList->AddRectFilled(ImVec2(pos.x + (5.f + 1.f), pos.y + (5.f + 1.f)), ImVec2(pos.x + sizeAvail.x - (5.f + 1.f), pos.y + (50.f - 1.f)), background, 5.f);
			drawList->AddRect(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), backgroundCorner, 5.f);
			//Title
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 15.f), Colors::whiteText, name.c_str());
			//Description
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 30.f), Colors::userTag, description.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 50.f));

			return ret;
		}

		bool Button(std::string name, std::string description)
		{
			bool hovering = false;

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->Flags = ImDrawListFlags_AntiAliasedFill;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			ImColor background = ImColor(66, 66, 66, 25);
			bool ret = false;

			if (Custom::IsMouseHoveringRect(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f)))
			{
				hovering = true;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					ret = true;
			}
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);

			float a1 = alpha * 50;
			if (a1 < 25)
				a1 = 25;
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));
			background = ImColor(66, 66, 66, (int)a1);
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && hovering)
				background = ImColor(66, 66, 66, (int)(alpha * 80));

			//Background
			drawList->AddRectFilled(ImVec2(pos.x + (5.f + 1.f), pos.y + (5.f + 1.f)), ImVec2(pos.x + sizeAvail.x - (5.f + 1.f), pos.y + (50.f - 1.f)), background, 5.f);
			drawList->AddRect(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), backgroundCorner, 5.f);
			//Title
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 15.f), Colors::whiteText, name.c_str());

			ImVec2 txtSize = ImGui::CalcTextSize(description.c_str());
			std::string desc = description;
			bool needTooltip = false;
			if (pos.x + 15.f + txtSize.x > pos.x + sizeAvail.x - 5.f)
			{
				needTooltip = true;
				while (pos.x + 15.f + txtSize.x > pos.x + sizeAvail.x - 5.f)
				{
					//Remove 4 last characters
					desc = desc.substr(0, desc.size() - 4);
					desc.append("...");
					txtSize = ImGui::CalcTextSize(desc.c_str());
				}
			}

			if (Custom::IsMouseHoveringRect(pos, pos + ImVec2(sizeAvail.x, 50.f)))
			{
				if (needTooltip)
					Tooltip::tip(description);
			}

			//Description
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 30.f), Colors::userTag, desc.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 50.f));

			return ret;
		}

		std::unordered_map<std::string, int> g_finished;

		void HKThread(std::string name, int& key)
		{
			if (!g_finished[name])
				return;
			g_finished[name] = 0;
			Sleep(200);
			while (true)
			{
				for (auto it = Hotkey::virtualKeyToString.begin(); it != Hotkey::virtualKeyToString.end(); ++it)
				{
					if (GetAsyncKeyState(it->first) & 0x8000)
					{
						key = it->first;
						g_finished[name] = true;
						break;
					}
					if (Hotkey::IsDown(it->first))
					{
						key = it->first;
						g_finished[name] = true;
						break;
					}
				}
				if (g_finished[name])
					break;
			}
		}

		void Hotkey(std::string name, int* key)
		{
			std::string KeyName = Hotkey::virtualKeyToString.find(*key) != Hotkey::virtualKeyToString.end() ? Hotkey::virtualKeyToString[*key].c_str() : std::to_string(*key).c_str();

			if (g_finished.find(name) == g_finished.end())
				g_finished[name] = 1;
			if (!g_finished[name])
			{
				if (Button(name + xorstr_(": <Press a key>"), xorstr_("Waiting for you to set a key...")))
				{
				}
			}
			else
			{
				if (Button(name + xorstr_(": ") + KeyName, xorstr_("Click to set key")))
				{
					std::thread(HKThread, name, std::ref(*key)).detach();
				}
			}
		}

		bool Input(std::string name, char* buf, size_t bufSize)
		{
			bool ret = false;
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			//ImColor background = ImColor(20, 20, 20, 255);
			ImColor line = ImColor(69, 69, 69, 255);
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();

			//ImVec4 = Background
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.07f, 0.07f, 0.07f, 1.f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
			ImGui::PushID(name.c_str());
			ret = ImGui::InputText("", buf, bufSize);
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				hovering = true;

			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			///Input rect
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), backgroundCorner, 2.f);

			//Compensate -10.f for what we set previous above
			ImGui::SetCursorScreenPos(ImVec2(pos.x - 10.f, pos.y + 25.f));
			return ret;
		}

		bool Input(std::string name, std::string* buf, size_t bufSize)
		{
			bool hovering = false;
			bool ret = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			//ImColor background = ImColor(20, 20, 20, 255);
			ImColor line = ImColor(69, 69, 69, 255);
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();

			//ImVec4 = Background
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.07f, 0.07f, 0.07f, 1.f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
			ImGui::PushID(name.c_str());
			ret = ImGui::InputText("", (char*)buf->c_str(), bufSize);
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			///Input rect
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), backgroundCorner, 2.f);

			//Compensate -10.f for what we set previous above
			ImGui::SetCursorScreenPos(ImVec2(pos.x - 10.f, pos.y + 25.f));
			return ret;
		}

		void InputInt(std::string name, int* buf)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			//ImColor background = ImColor(20, 20, 20, 255);
			ImColor line = ImColor(69, 69, 69, 255);
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();
			//ImVec4 = Background
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.07f, 0.07f, 0.07f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImColor(20, 20, 20, 255).Value); // Normal state
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(30, 30, 30, 255).Value); // Hovered state
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(40, 40, 40, 255).Value); // Hovered state
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
			ImGui::PushID(name.c_str());
			ImGui::InputInt("", buf);
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(4);

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			///Input rect
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), backgroundCorner, 2.f);

			//Compensate -10.f for what we set previous above
			ImGui::SetCursorScreenPos(ImVec2(pos.x - 10.f, pos.y + 25.f));
		}

		void Slider(std::string name, float* out, int min, int max)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());

			//Display amount
			drawList->AddRectFilled(ImVec2(pos.x + sizeAvail.x - 50.f, pos.y + 12.5f), ImVec2(pos.x + sizeAvail.x - 12.5f, pos.y + 42.5f), ImColor(60, 60, 60, 255), 5);

			std::string str = "";
			sprintf((char*)str.c_str(), "%.2f", *out);

			ImVec2 textSize = ImGui::CalcTextSize(str.c_str());

			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 31.f - (textSize.x / 2), pos.y + 28.5f - (textSize.y / 2)), Colors::whiteText, str.c_str());

			if (*out < min)*out = min;
			else if (*out > max)*out = max;

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f); ///Im mad lazy
			ImGui::SliderFloat(name.c_str(), out, min, max);
			ImGui::PopStyleVar();
			ImVec2 size = ImGui::GetItemRectSize();

			ImColor line = ImColor(69, 69, 69, 255);
			ImColor background = ImColor(20, 20, 20, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);
			ImColor slider = ImColor(94, 255, 176, 100);

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			auto txtSize = ImGui::CalcTextSize(name.c_str());
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, line, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, lineColor, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
			{
				drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

				drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);
			}
			//Compensate -10.f for what we set previous above
			ImGui::SetCursorScreenPos(ImVec2(pos.x - 10.f, pos.y + 25.f));
		}

		//static float f1 = -0.5f, f2 = 0.75f;
		//ImGui::RangeSliderFloat("range slider float", &f1, &f2, -1.0f, 1.0f, "(%.3f, %.3f)");
		///https://github.com/wasikuss/imgui
		void SliderRange(std::string name, float* out, float* out2, float min, float max, float power)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());

			if (*out < min)*out = min;
			else if (*out > max)*out = max;

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f); ///Im mad lazy
			//ImGui::RangeSliderFloat(name.c_str(), out, out2, min, max, "(%.2f, %.2f)", power);
			ImGui::PopStyleVar();
			ImVec2 size = ImGui::GetItemRectSize();

			ImColor line = ImColor(69, 69, 69, 255);
			ImColor background = ImColor(20, 20, 20, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImColor slider = ImColor(94, 255, 176, 100);
			ImColor sliderBox = ImColor(94, 255, 176, 255);

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			auto txtSize = ImGui::CalcTextSize(name.c_str());

			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, line, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, lineColor, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

			drawList->AddRectFilled({pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + 1}, {pos.x + ((((float)*out2 - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);

			std::string str = "";
			sprintf((char*)str.c_str(), "%.2f - %.2f", *out, *out2);

			auto txtSize2 = ImGui::CalcTextSize(str.c_str());

			drawList->AddRectFilled({pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100) - 3.f, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100) + 3.f, pos.y + size.y - 1}, sliderBox);
			drawList->AddRectFilled({pos.x + ((((float)*out2 - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100) - 3.f, pos.y + 1}, {pos.x + ((((float)*out2 - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100) + 3.f, pos.y + size.y - 1}, sliderBox);
			drawList->AddText(ImVec2(pos.x + (((size.x - txtSize.x) / 2) - (txtSize2.x / 2)), pos.y + ((size.y / 2) - txtSize2.y / 2)), Colors::whiteText, str.c_str());

			//Compensate -10.f for what we set previous above
			ImGui::SetCursorScreenPos(ImVec2(pos.x - 10.f, pos.y + 25.f));
		}

		bool SliderInt(std::string name, int* out, int min, int max)
		{
			bool ret = false;

			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());

			//Display amount
			drawList->AddRectFilled(ImVec2(pos.x + sizeAvail.x - 50.f, pos.y + 12.5f), ImVec2(pos.x + sizeAvail.x - 12.5f, pos.y + 42.5f), ImColor(60, 60, 60, 255), 5);
			std::string str = std::format("{:0}", *out);
			ImVec2 textSize = ImGui::CalcTextSize(str.c_str());

			drawList->AddText(ImVec2(pos.x + sizeAvail.x - 31.f - (textSize.x / 2), pos.y + 28.5f - (textSize.y / 2)), Colors::whiteText, str.c_str());

			if (*out < min)
			{
				*out = min;
			}
			else if (*out > max)*out = max;

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f); ///Im mad lazy
			ret = ImGui::SliderInt(name.c_str(), out, min, max);
			ImGui::PopStyleVar();
			ImVec2 size = ImGui::GetItemRectSize();

			ImColor line = ImColor(69, 69, 69, 255);
			ImColor background = ImColor(20, 20, 20, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImColor slider = ImColor(94, 255, 176, 100);

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			auto txtSize = ImGui::CalcTextSize(name.c_str());
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, line, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, lineColor, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
			{
				drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

				drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);
			}
			//Compensate -10.f for what we set previous above
			ImGui::SetCursorScreenPos(ImVec2(pos.x - 10.f, pos.y + 25.f));

			return ret;
		}

		void Listbox(std::string name, int* current_item, const char* const items[], int items_count, int height_in_items)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			auto txt = ImGui::CalcTextSize(name.c_str());
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 200.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) - (txt.x / 2), pos.y + 10.f), Colors::whiteText, name.c_str());

			ImColor line = ImColor(69, 69, 69, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 15.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();
			ImGui::PushItemWidth(sizeAvail.x - 30.f);
			//Mouse hovered over colors
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(66, 66, 66, 50));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(66, 66, 66, 130));
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(66, 66, 66, 80));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(20, 20, 20, 255));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			ImGui::PushID(name.c_str());

			ImGui::ListBox("", current_item, items, items_count, height_in_items);
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(4);
			ImGui::PopItemWidth();

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), lineColor, 2.f);
			pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 10.f));
		}

		bool Listbox(std::string name, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			//Background
			auto txt = ImGui::CalcTextSize(name.c_str());
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 212.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) - (txt.x / 2), pos.y + 10.f), Colors::whiteText, name.c_str());

			ImColor line = ImColor(69, 69, 69, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 15.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();
			ImGui::PushItemWidth(sizeAvail.x - 30.f);
			//Mouse hovered over colors
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(66, 66, 66, 50));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(66, 66, 66, 130));
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(66, 66, 66, 80));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(20, 20, 20, 255));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			ImGui::PushID(name.c_str());

			bool ret = ImGui::ListBox("", current_item, items_getter, data, items_count, height_in_items);
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(4);
			ImGui::PopItemWidth();

			if (ImGui::IsItemHovered())
				hovering = true;
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), lineColor, 2.f);
			pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 10.f));

			return ret;
		}

		void Combo(std::string name, std::vector<std::string> items, int& index)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + sizeAvail.x - 5.f, pos.y + 50.f), ImColor(66, 66, 66, 25), 5.f);
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());
			ImColor line = ImColor(69, 69, 69, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 15.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();

			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(66, 66, 66, 50));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(66, 66, 66, 130));
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(66, 66, 66, 80));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(20, 20, 20, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(66, 66, 66, 50));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(66, 66, 66, 80));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			ImGui::PushItemWidth(sizeAvail.x - 30.f);
			ImGui::PushID(name.c_str());
			if (ImGui::BeginCombo("", items.at(index).c_str(), ImGuiComboFlags_NoArrowButton))
			{
				for (int n = 0; n < items.size(); n++)
				{
					const bool is_selected = (index == n);
					if (ImGui::Selectable(items.at(n).c_str(), is_selected))index = n;

					if (is_selected)ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				}
				ImGui::EndCombo();
			}
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(6);
			ImGui::PopItemWidth();

			if (ImGui::IsItemHovered())
			{
				UII::Tooltip::tip("Click Me!");
				hovering = true;
			}
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), lineColor, 2.f);
			pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 6.f));
		}
	}

	namespace ModalController
	{
		bool crosshairEditor = false;
		bool ProfileSelector = false;

		//Center Dot
		bool dot = true;

		//Gap between Dot and 4 things
		float gap = 0.f;

		//Length of the 4 things
		float Length = 5.f;

		//Thickness of lines & dot
		float Thicc = 2.f;

		//Still gotto make
		bool outline = true;
		float Outline = 2.f;

		//By default gap between Center & 4 points = 2 pixels, GAP 0 = 2 pixels
		//don't forget there is a extra padding from left of 5.f pixels by default
		void crosshair()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			//if (outline) {
			//Center Outline
			//	drawList->AddRectFilled(ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) - 2.f, pos.y + 68.f), ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) + 2.f, pos.y + 72.f), ImColor(0, 0, 0, 255));
			//}
			//Dot
			drawList->AddRectFilled(ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) - (Thicc / 2), pos.y + (70.f - (Thicc / 2))), ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) + (Thicc / 2), pos.y + (70.f + (Thicc / 2))), ImColor(255, 255, 255, 255));

			//Top
			drawList->AddRectFilled(ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) - (Thicc / 2), pos.y + ((70.f - (Thicc / 2)) - ((2.f + gap) + Length))), ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) + (Thicc / 2), pos.y + ((70.f - (Thicc / 2)) - (2.f + gap))), ImColor(255, 255, 255, 255));

			//Bottom
			drawList->AddRectFilled(ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) - (Thicc / 2), pos.y + ((70.f + (Thicc / 2)) + ((2.f + gap) + Length))), ImVec2(pos.x + ((sizeAvail.x - 5.f) / 2) + (Thicc / 2), pos.y + ((70.f + (Thicc / 2)) + (2.f + gap))), ImColor(255, 255, 255, 255));

			//Left
			drawList->AddRectFilled(ImVec2(pos.x + ((((sizeAvail.x - 5.f) / 2) - (Thicc / 2)) - ((2.f + gap) + Length)), pos.y + (70.f - (Thicc / 2))), ImVec2(pos.x + ((((sizeAvail.x - 5.f) / 2) - (Thicc / 2)) - (2.f + gap)), pos.y + (70.f + (Thicc / 2))), ImColor(255, 255, 255, 255));

			//Right
			drawList->AddRectFilled(ImVec2(pos.x + ((((sizeAvail.x - 5.f) / 2) + (Thicc / 2)) + ((2.f + gap) + Length)), pos.y + (70.f - (Thicc / 2))), ImVec2(pos.x + ((((sizeAvail.x - 5.f) / 2) + (Thicc / 2)) + (2.f + gap)), pos.y + (70.f + (Thicc / 2))), ImColor(255, 255, 255, 255));
		}

		void DisplayCrosshairOnScreen(ImVec2 pos, ImVec2 sizeAvail)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(ImVec2(pos.x + ((sizeAvail.x) / 2) - (Thicc / 2), pos.y + (sizeAvail.y - (Thicc / 2))), ImVec2(pos.x + (sizeAvail.x / 2) + (Thicc / 2), pos.y + (sizeAvail.y + (Thicc / 2))), ImColor(255, 255, 255, 255));

			//Top
			drawList->AddRectFilled(ImVec2(pos.x + (sizeAvail.x / 2) - (Thicc / 2), pos.y + ((sizeAvail.y - (Thicc / 2)) - ((2.f + gap) + Length))), ImVec2(pos.x + (sizeAvail.x / 2) + (Thicc / 2), pos.y + ((sizeAvail.y - (Thicc / 2)) - (2.f + gap))), ImColor(255, 255, 255, 255));

			//Bottom
			drawList->AddRectFilled(ImVec2(pos.x + (sizeAvail.x / 2) - (Thicc / 2), pos.y + ((sizeAvail.y + (Thicc / 2)) + ((2.f + gap) + Length))), ImVec2(pos.x + (sizeAvail.x / 2) + (Thicc / 2), pos.y + ((sizeAvail.y + (Thicc / 2)) + (2.f + gap))), ImColor(255, 255, 255, 255));

			//Left
			drawList->AddRectFilled(ImVec2(pos.x + (((sizeAvail.x / 2) - (Thicc / 2)) - ((2.f + gap) + Length)), pos.y + (sizeAvail.y - (Thicc / 2))), ImVec2(pos.x + (((sizeAvail.x / 2) - (Thicc / 2)) - (2.f + gap)), pos.y + (sizeAvail.y + (Thicc / 2))), ImColor(255, 255, 255, 255));

			//Right
			drawList->AddRectFilled(ImVec2(pos.x + (((sizeAvail.x / 2) + (Thicc / 2)) + ((2.f + gap) + Length)), pos.y + (sizeAvail.y - (Thicc / 2))), ImVec2(pos.x + (((sizeAvail.x / 2) + (Thicc / 2)) + (2.f + gap)), pos.y + (sizeAvail.y + (Thicc / 2))), ImColor(255, 255, 255, 255));
		}

		void CrosshairDisplay()
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			drawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + (sizeAvail.x - 5.f), pos.y + 140), ImColor(255, 255, 255, 30), 5.f);

			//Draw Crosshair
			crosshair();

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 140));
		}

		void Combo(std::string name, std::vector<std::string> items, int& index)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();
			drawList->AddText(ImVec2(pos.x + 15.f, pos.y + 10.f), Colors::whiteText, name.c_str());
			ImColor line = ImColor(69, 69, 69, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 15.f, pos.y + 25.f));
			pos = ImGui::GetCursorScreenPos();

			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(66, 66, 66, 50));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(66, 66, 66, 130));
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(66, 66, 66, 80));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(20, 20, 20, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(66, 66, 66, 50));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(66, 66, 66, 80));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			ImGui::PushItemWidth(sizeAvail.x - 30.f);
			ImGui::PushID(name.c_str());
			if (ImGui::BeginCombo("", items.at(index).c_str(), ImGuiComboFlags_NoArrowButton))
			{
				for (int n = 0; n < items.size(); n++)
				{
					const bool is_selected = (index == n);
					if (ImGui::Selectable(items.at(n).c_str(), is_selected))index = n;

					if (is_selected)ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				}
				ImGui::EndCombo();
			}
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(6);
			ImGui::PopItemWidth();

			if (ImGui::IsItemHovered())
			{
				UII::Tooltip::tip("Click Me!");
				hovering = true;
			}
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y), lineColor, 2.f);
		}

		void Input(std::string name, char* buf, size_t bufSize, float width, bool alignMiddle)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			//ImColor background = ImColor(20, 20, 20, 255);
			ImColor line = ImColor(69, 69, 69, 255);

			float weirdpadding = 2.5f;
			float padding = 10.f;
			if (alignMiddle)
			{
				padding = ((sizeAvail.x / 2) - (width / 2)) - weirdpadding;
			}

			ImGui::SetCursorScreenPos(ImVec2(pos.x + padding, pos.y + 15.f));
			pos = ImGui::GetCursorScreenPos();

			//ImVec4 = Background
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.07f, 0.07f, 0.07f, 1.f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
			ImGui::PushItemWidth(width);
			ImGui::PushID(name.c_str());
			ImGui::InputText("", buf, bufSize);
			ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				hovering = true;

			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");

			drawList->AddText(ImVec2(pos.x + ((padding) - (txtSize.x / 2)), pos.y - 16.f), Colors::whiteText, name.c_str());
			///Input rect
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), backgroundCorner, 2.f);
		}

		void InputInt(std::string name, int* buf, size_t bufSize, float width, bool alignMiddle)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			//ImColor background = ImColor(20, 20, 20, 255);
			ImColor line = ImColor(69, 69, 69, 255);

			float weirdpadding = 2.5f;
			float padding = 10.f;
			if (alignMiddle)
			{
				padding = ((sizeAvail.x / 2) - (width / 2)) - weirdpadding;
			}

			ImGui::SetCursorScreenPos(ImVec2(pos.x + padding, pos.y + 15.f));
			pos = ImGui::GetCursorScreenPos();

			//ImVec4 = Background
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(0.07f, 0.07f, 0.07f, 1.f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
			ImGui::PushItemWidth(width);
			ImGui::PushID(name.c_str());
			ImGui::InputInt("", buf);
			ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
				hovering = true;

			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));

			ImVec2 size = ImGui::GetItemRectSize();
			auto txtSize = ImGui::CalcTextSize("");

			drawList->AddText(ImVec2(pos.x + ((padding) - (txtSize.x / 2)), pos.y - 16.f), Colors::whiteText, name.c_str());
			///Input rect
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), line, 2.f);
			drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x - txtSize.x, pos.y + size.y + 1), backgroundCorner, 2.f);
		}

		bool Button(std::string name, float width, float heigth)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->Flags = ImDrawListFlags_AntiAliasedFill;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			ImColor backgroundCorner = ImColor(0, 0, 0, 0);
			ImColor line = ImColor(69, 69, 69, 255);
			ImColor background = ImColor(20, 20, 20, 255);
			bool ret = false;

			if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + (width + 5.f), pos.y + heigth)))
			{
				hovering = true;
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					ret = true;
			}
			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			backgroundCorner = ImColor(94, 255, 176, (int)(alpha * 150));

			//Background
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + (width + 5.f), pos.y + heigth), line, 5.f);
			drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + (width + 5.f), pos.y + heigth), backgroundCorner, 5.f);
			drawList->AddRectFilled(ImVec2(pos.x + 6.f, pos.y + 6.f), ImVec2(pos.x + (width + 4.f), pos.y + heigth - 1.f/*49.f*/), background, 5.f);

			//drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + ((sizeAvail.x - 5.f) - width), pos.y + 50.f ), line, 5.f);	//Corner background	1	
			//drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + ((sizeAvail.x - 5.f) - width), pos.y + 50.f), backgroundCorner, 5.f);	//Corner background	2				
			//drawList->AddRectFilled(ImVec2(pos.x + (5.f + 1.f), pos.y + (5.f + 1.f)), ImVec2(pos.x + ((sizeAvail.x - (5.f + 1.f)) - width), pos.y + (50.f - 1.f)), background, 5.f);   //Real background
			//drawList->AddRect(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + ((sizeAvail.x - 5.f) - width), pos.y + 50.f), backgroundCorner, 5.f);
			//Title
			auto txtSize = ImGui::CalcTextSize(name.c_str());
			drawList->AddText(ImVec2(pos.x + ((width / 2) - (txtSize.x / 2)) + 5.5f/*Magic Padding number*/, pos.y + (/*25.f*/(heigth / 2) - (txtSize.y / 2) / 2)), Colors::whiteText, name.c_str());
			//drawList->AddText(ImVec2(pos.x + ((((sizeAvail.x - 10.f) - width) - (txtSize.x / 2)) / 2) - 3.f /*Weird padding*/, pos.y + (25.f - (txtSize.y / 2) / 2)), Colors::whiteText, name.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + heigth));

			return ret;
		}

		void Slider(std::string name, float* out, int min, int max, float width)
		{
			bool hovering = false;
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 sizeAvail = ImGui::GetContentRegionAvail();

			if (*out < min)*out = min;
			else if (*out > max)*out = max;

			ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + 5.f));
			ImGui::Text(name.c_str());
			pos = ImGui::GetCursorScreenPos();
			ImGui::PushItemWidth(width);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f); ///Im mad lazy
			ImGui::SliderFloat(name.c_str(), out, min, max);
			ImGui::PopStyleVar();
			ImGui::PopItemWidth();
			ImVec2 size = ImGui::GetItemRectSize();

			ImColor line = ImColor(69, 69, 69, 255);
			ImColor background = ImColor(20, 20, 20, 255);
			ImColor lineColor = ImColor(94, 255, 176, 0);

			ImColor slider = ImColor(94, 255, 176, 100);

			if (ImGui::IsItemHovered())
				hovering = true;

			float alpha = Animation::GetAnimData(name + "controls", hovering, 20.f, 0.8f);
			lineColor = ImColor(94, 255, 176, (int)(alpha * 150));

			auto txtSize = ImGui::CalcTextSize(name.c_str());
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, line, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x, pos.y}, {pos.x + size.x - txtSize.x, pos.y + size.y}, lineColor, 2.f); ///Line around slider
			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

			drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
			{
				drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + size.x - (1 + txtSize.x), pos.y + size.y - 1}, background, 2.f); ///Slider actual background

				drawList->AddRectFilled({pos.x + 1, pos.y + 1}, {pos.x + ((((float)*out - min) / (max - min)) * 100) * ((size.x - (1 + txtSize.x)) / 100), pos.y + size.y - 1}, slider);
			}

			//Display amount
			std::string str = "";
			sprintf((char*)str.c_str(), "%.2f", *out);

			ImVec2 txtSize2 = ImGui::CalcTextSize(str.c_str());
			drawList->AddText(ImVec2(pos.x + (((size.x - txtSize.x) / 2) - (txtSize2.x / 2)), pos.y + ((size.y - txtSize2.y) / 2)), Colors::whiteText, str.c_str());
		}
	}
}

void GUI::Begin()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Once);
	ImGui::Begin(xorstr_("Fbi Moe Cheats"), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
}

void GUI::End()
{
	ImGui::End();
}

void GUI::DisplayBrand(std::string text)
{
	auto drawList = ImGui::GetWindowDrawList();
	//Draw Filled Rect around Text
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
	drawList->AddLine(ImVec2(10, 9), ImVec2(10 + textSize.x + 10, 9), ImColor(51, 224, 255, 130), 2.f);
	drawList->AddRectFilled(ImVec2(10, 10), ImVec2(10 + textSize.x + 10, 10 + textSize.y + 10), ImColor(32, 35, 36, 130));
	drawList->AddText(ImVec2(15, 15), ImColor(255, 255, 255), text.c_str());
}

void GUI::BeginMenu()
{
	UII::Start();
	this->startPos = ImGui::GetCursorScreenPos();
	ctx.m_settings.Set<ImVec2>(&ctx.m_settings.Visuals_Misc_GUIPos, this->startPos);
	UII::Modal::tempPos = this->startPos;
}

void GUI::EndMenu()
{
	UII::End();
}

void GUI::DrawBackground()
{
	UII::MainWindow::background();
	UII::Nav::headerBackground();
	UII::Sidebar::sidebarBackground();
	UII::Config::drawBackground();
	UII::Config::grayLineBackground();

	UII::Nav::header();
}

void GUI::DisplayTab(std::vector<TabButtons> buttons)
{
	UII::Sidebar::startSidebar();
	{
		for (size_t i = 0; i < buttons.size(); i++)
		{
			auto button = buttons[i];
			UII::Sidebar::button(button.Name, (std::string)(const char*)button.icon, button.icon_padding);
		}
	}
	UII::Sidebar::endSidebar();
}

void GUI::DisplayConfigs()
{
	UII::Config::getAllConfigs();
	UII::Config::beginConfig();
	{
		for (int i = 0; i < UII::Config::configs.size(); i++)
		{
			UII::Config::configOption(UII::Config::configs[i], i);
		}
	}
	UII::Config::endConfig();
}

void GUI::DisplayModals()
{
	if (UII::Config::saveConfigModal)
	{
		UII::Modal::startModal(xorstr_("Config Editor"), &UII::Config::saveConfigModal, startPos, xorstr_("Save Config"), ImVec2(200, 100));
		{
			static char buffer[128];
			UII::ModalController::Input(xorstr_("Config Name"), buffer, sizeof(buffer), 180.f, true);
			if (UII::ModalController::Button(xorstr_("Create"), 85.f))
			{
				UII::Config::createConfig(buffer);
				memcpy(buffer, xorstr_(""), 0);
				//UII::Config::saveConfigModal = false;
			}
			//Sorry not sorry
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 90.f, pos.y - 50.f));

			if (UII::ModalController::Button(xorstr_("Save"), 85.f))
			{
				if (!UII::Config::configs.empty())
				{
					ctx.m_settings.save((UII::Config::configs[UII::Config::currentConfig]).c_str());
					memcpy(buffer, xorstr_(""), 0);
				}
				//UII::Config::saveConfigModal = false;
			}
		}
		UII::Modal::endModal();
	}

	if (UII::ModalController::crosshairEditor)
	{
		UII::Modal::startModal("Configure Crosshair", &UII::ModalController::crosshairEditor, startPos, "Configure Crosshair", ImVec2(200, 300));
		{
			UII::ModalController::CrosshairDisplay();

			UII::ModalController::Slider("Gap", &UII::ModalController::gap, -10, 10, 184.5f);
			//Enable dot toggle here
			UII::ModalController::Slider("Length", &UII::ModalController::Length, 0, 25, 184.5f);
			UII::ModalController::Slider("Thickness", &UII::ModalController::Thicc, 1, 10, 184.5f);
		}
		UII::Modal::endModal();
	}

	if (UII::Nav::notification)
	{
		UII::Modal::startModal("Notifications", &UII::Nav::notification, startPos, "Notifications", ImVec2(300, 500));
		{
			//TODO:
		}
		UII::Modal::endModal();
	}
}

void GUI::DisplayPadding()
{
	static std::string old_active = "";
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	ImGui::SetCursorPos(ImVec2(UII::Sidebar::barSize.x + UII::Config::configSize.x, UII::Nav::navSize.y));
	ImVec2 avail = ImGui::GetContentRegionAvail();

	if (old_active != navigation::activeID)
	{
		Animation::resetAnimData(xorstr_("Space Left"));
		old_active = navigation::activeID;
	}
	float alpha = Animation::GetAnimData(xorstr_("Space Left"), true, 20.f, 0.8f);

	ImVec2 pos = ImGui::GetCursorScreenPos();
	drawList->AddRectFilled(ImVec2(pos.x + 5.f, pos.y + 5.f), ImVec2(pos.x + avail.x, pos.y + avail.y), ImColor(26, 26, 38, (int)(255 - (alpha * 255))));
	ImGui::BeginChild(xorstr_("Space Left"), avail);
	ContainerPos = ImGui::GetCursorScreenPos();

	pos = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(pos.x + 16.25f, pos.y + 10.f));
}

void GUI::MoveWindow()
{
	///move around window around on screen
	if (ImGui::IsMouseHoveringRect(ImVec2(startPos.x + UII::Sidebar::barSize.x, startPos.y), ImVec2(startPos.x + UII::Nav::navSize.x - 100.f/*100.f padding for minimize, exit button & notification button*/, startPos.y + UII::Nav::navSize.y)))
	{
		UII::menuFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	}
	else
	{
		UII::menuFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	}
}

bool GUI::IsActive(std::string name)
{
	return navigation::isActive(name);
}
#pragma optimize("", on)
namespace FloatingSection
{
	inline std::unordered_map<std::string, ImGuiWindowFlags> floatingSectionFlags;
	inline std::unordered_map<std::string, bool> floatinSectionPins;
	inline std::unordered_map<std::string, ImVec2> floatinSectionResize;

	void DrawHeader(std::string name, ImVec2 size, float headerHeight, bool pin, bool allow_resize, bool transparent)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		if (ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + availableSize.x, pos.y + headerHeight)))
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		else
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		//2 rects because we wanna have a smooth transition without making the bottom part rounded.
		//Offset by 1 / removeal by 1 for the line around the section
		if (!transparent)
		{
			ImColor background = ctx.m_settings.Visuals_Misc_Hud_BackgroundColour;
			drawList->AddRectFilled(pos + ImVec2(1.f, 1.f), pos + (availableSize - ImVec2(1.f, 1.f)), background, 5.f);
		}
		else
		{
			drawList->AddRectFilled(pos + ImVec2(1.f, 1.f), pos + ImVec2(availableSize.x, 30.f), Colors::mainWindowBackground, 5.f);
			drawList->AddRect(pos + ImVec2(1.f, 1.f), pos + (availableSize - ImVec2(1.f, 1.f)), Colors::mainWindowBackground, 5.f);
		}
		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
		drawList->AddText(ImVec2(pos.x + ((size.x / 2) - (textSize.x / 2)), pos.y + ((headerHeight / 2) - (textSize.y / 2))), Colors::whiteText, name.c_str());

		if (pin)
		{
			ImGui::PushFont(Font::icon);
			float iconSize = ImGui::GetFontSize();
			ImColor pinColor = ImColor(94, 255, 176, 100);
			if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), ImVec2(pos.x + (size.x - 10.f), pos.y + ((headerHeight / 2) + (iconSize / 2)))))
			{
				pinColor = ImColor(94, 255, 176, 150);
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					floatinSectionPins[name] = !floatinSectionPins[name];
			}
			if (!floatinSectionPins[name])
				drawList->AddText(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), pinColor, (char*)ICON_PIN_OUTLINE);
			else
				drawList->AddText(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), pinColor, (char*)ICON_PIN);
			ImGui::PopFont();
		}

		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight));
	}

	void Draw(std::string name, ImVec2 size, bool pin, bool allow_resize, bool transparent)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		float HeaderHeight = 30.f;
		bool hovering = false;

		if (!transparent)
			drawList->AddRectFilled(pos, pos + availableSize, Colors::sidebarBackground, 5.f);

		if (ImGui::IsMouseHoveringRect(pos, pos + availableSize))
			hovering = true;

		//float alpha = UI::Animation::GetAnimDataHover(name + xorstr_("section_impl"), hovering, 20.f, 0.8f);
		//ImColor minimizeColor = Animation::InterpolateColor(Color::SectionBackgroundRectColor, Color::SectionBackgroundRectHoverColor, alpha);
		/*if (alpha > 0.f)
		{
			drawList->AddRectFilled(ImVec2(pos.x, (pos.y + (availableSize.y / 2) - (alpha * (availableSize.y / 2)))), ImVec2(pos.x + availableSize.x, pos.y + (availableSize.y / 2)), minimizeColor, 5.f);
			drawList->AddRectFilled(ImVec2(pos.x, pos.y + (availableSize.y / 2) - 7.f/*idk why i have to do this magic number tbh#1#), ImVec2(pos.x + availableSize.x, (pos.y + (availableSize.y / 2)) + (alpha * (availableSize.y / 2))), minimizeColor, 5.f);
		}*/
		DrawHeader(name, size, HeaderHeight, pin, allow_resize, transparent);
		if (!transparent)
			drawList->AddRectFilled(pos + ImVec2(1.f, 30.f), pos + (availableSize - 1.f), Colors::sidebarBackground, 5.f);
	}

	void BeginSection(std::string name, bool open, ImVec2* pos, ImVec2 size, bool pin, ImVec2 offset, bool allow_resize, bool transparent, bool change_size)
	{
		if (size.x == 0.f && size.y == 0.f)
			size = ImVec2(ImGui::GetContentRegionAvail().x - 10.f, ImGui::GetContentRegionAvail().y - 10.f);
		if (allow_resize)
		{
			if (floatinSectionResize.find(name) == floatinSectionResize.end())
				floatinSectionResize[name] = size;
			size = floatinSectionResize[name];
		}
		if (pin)
			if (floatinSectionPins.find(name) == floatinSectionPins.end())
				floatinSectionPins[name] = false;
		if (floatingSectionFlags.find(name) == floatingSectionFlags.end())
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		if (pin)
			if (floatinSectionPins[name])
				ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(*pos + 5.f, ImGuiCond_FirstUseEver);
		if (change_size)
			ImGui::SetNextWindowSize(size, ImGuiCond_Always);
		else
			ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(94, 255, 176, 180));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(94, 255, 176, 200));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(94, 255, 176, 255));
		ImGui::GetStyle().ScrollbarSize = 3.f;
		ImGui::Begin((name + xorstr_("##section2")).c_str(), &open, floatingSectionFlags[name]);
		if (pos->x != 0.f && pos->y != 0.f)
			*pos = ImGui::GetCursorScreenPos();
		Draw(name, size, pin, allow_resize, transparent);
		if (allow_resize)
		{
			size = ImGui::GetWindowSize();
			floatinSectionResize[name] = size;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 1.f, pos.y));
			offset.x += 1.f;
		}
		ImGui::BeginChild((name + xorstr_("##section3")).c_str(), ImVec2(size.x - offset.x, size.y - offset.y), 0);
	}

	void BeginSection(std::string name, bool open, ImVec2 pos, ImVec2 size, bool pin, ImVec2 offset, bool allow_resize, bool transparent)
	{
		if (size.x == 0.f && size.y == 0.f)
			size = ImVec2(ImGui::GetContentRegionAvail().x - 10.f, ImGui::GetContentRegionAvail().y - 10.f);
		if (pos.x == 0.f && pos.y == 0.f)
			pos = ImGui::GetCursorScreenPos();
		if (allow_resize)
		{
			if (floatinSectionResize.find(name) == floatinSectionResize.end())
				floatinSectionResize[name] = size;
			size = floatinSectionResize[name];
		}
		if (pin)
			if (floatinSectionPins.find(name) == floatinSectionPins.end())
				floatinSectionPins[name] = false;
		if (floatingSectionFlags.find(name) == floatingSectionFlags.end())
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		if (pin)
			if (floatinSectionPins[name])
				ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(pos + 5.f, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(94, 255, 176, 180));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(94, 255, 176, 200));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(94, 255, 176, 255));
		ImGui::GetStyle().ScrollbarSize = 3.f;
		ImGui::Begin((name + xorstr_("##section2")).c_str(), &open, floatingSectionFlags[name]);

		Draw(name, size, pin, allow_resize, transparent);
		if (allow_resize)
		{
			size = ImGui::GetWindowSize();
			floatinSectionResize[name] = size;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 1.f, pos.y));
			offset.x += 1.f;
		}
		ImGui::BeginChild((name + xorstr_("##section3")).c_str(), ImVec2(size.x - offset.x, size.y - offset.y), 0);
	}

	void Text(std::string text, ImColor color)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImVec2 txtSize = ImGui::CalcTextSize(text.c_str());
		//Draw Text Middle

		drawList->AddText(ImVec2(pos.x + (size.x / 2) - (txtSize.x / 2), pos.y - (txtSize.y / 2)), color, text.c_str());
		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + txtSize.y + 3.f));
	}

	void EndSection()
	{
		ImGui::EndChild();
		ImGui::End();
		ImGui::PopStyleColor(4);
	}
}
#pragma optimize("", off)
/*
namespace FloatinSection
{
	std::unordered_map<std::string, ImGuiWindowFlags> floatingSectionFlags;
	std::unordered_map<std::string, bool> floatinSectionPins;
	std::unordered_map<std::string, ImVec2> floatinSectionResize;

	void DrawHeader(std::string name, ImVec2 size, float headerHeight, bool pin, bool allow_resize)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		if (ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + availableSize.x, pos.y + headerHeight)))
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		else
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		//2 rects because we wanna have a smooth transition without making the bottom part rounded.
		//Offset by 1 / removeal by 1 for the line around the section
		drawList->AddRectFilled(pos + ImVec2(1.f, 1.f), pos + (availableSize - ImVec2(1.f, 1.f)), Colors::mainWindowBackground, 5.f);

		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
		drawList->AddText(ImVec2(pos.x + ((size.x / 2) - (textSize.x / 2)), pos.y + ((headerHeight / 2) - (textSize.y / 2))), Colors::whiteText, name.c_str());

		if (pin)
		{
			ImGui::PushFont(Font::icon);
			float iconSize = ImGui::GetFontSize();
			ImColor pinColor = ImColor(94, 255, 176, 100);
			if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), ImVec2(pos.x + (size.x - 10.f), pos.y + ((headerHeight / 2) + (iconSize / 2)))))
			{
				pinColor = ImColor(94, 255, 176, 150);
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					floatinSectionPins[name] = !floatinSectionPins[name];
			}
			if (!floatinSectionPins[name])
				drawList->AddText(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), pinColor, (char*)ICON_PIN_OUTLINE);
			else
				drawList->AddText(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), pinColor, (char*)ICON_PIN);
			ImGui::PopFont();
		}

		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight));
	}

	void Draw(std::string name, ImVec2 size, bool pin, bool allow_resize, bool transparent)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		float HeaderHeight = 30.f;
		bool hovering = false;

		if (!transparent)
			drawList->AddRectFilled(pos, pos + availableSize, Colors::sidebarBackground, 5.f);

		if (ImGui::IsMouseHoveringRect(pos, pos + availableSize))
			hovering = true;

		float alpha = 1.f;
		ImColor minimizeColor = Colors::sidebarBackground;
		if (alpha > 0.f)
		{
			if (!transparent)
			{
				drawList->AddRectFilled(ImVec2(pos.x, (pos.y + (availableSize.y / 2) - (alpha * (availableSize.y / 2)))), ImVec2(pos.x + availableSize.x, pos.y + (availableSize.y / 2)), minimizeColor, 5.f);
				drawList->AddRectFilled(ImVec2(pos.x, pos.y + (availableSize.y / 2) - 7.f/*idk why i have to do this magic number tbh#1#), ImVec2(pos.x + availableSize.x, (pos.y + (availableSize.y / 2)) + (alpha * (availableSize.y / 2))), minimizeColor, 5.f);
			}
		}
		DrawHeader(name, size, HeaderHeight, pin, allow_resize);
		if (!transparent)
			drawList->AddRectFilled(pos + ImVec2(1.f, 30.f), pos + (availableSize - 1.f), Colors::sidebarBackground, 5.f);
	}

	void BeginSection(std::string name, bool open, ImVec2 pos, ImVec2 size, bool pin, ImVec2 offset, bool allow_resize, bool transparent)
	{
		if (size.x == 0.f && size.y == 0.f)
			size = ImVec2(ImGui::GetContentRegionAvail().x - 10.f, ImGui::GetContentRegionAvail().y - 10.f);
		if (pos.x == 0.f && pos.y == 0.f)
			pos = ImGui::GetCursorScreenPos();
		if (allow_resize)
		{
			if (floatinSectionResize.find(name) == floatinSectionResize.end())
				floatinSectionResize[name] = size;
			size = floatinSectionResize[name];
		}
		if (pin)
			if (floatinSectionPins.find(name) == floatinSectionPins.end())
				floatinSectionPins[name] = false;
		if (floatingSectionFlags.find(name) == floatingSectionFlags.end())
		{
			if (!allow_resize)
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
			else
				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
		}
		if (pin)
			if (floatinSectionPins[name])
				ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(pos + 5.f, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(94, 255, 176, 180));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(94, 255, 176, 200));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(94, 255, 176, 255));
		ImGui::GetStyle().ScrollbarSize = 3.f;
		ImGui::Begin((name + xorstr_("##section2")).c_str(), &open, floatingSectionFlags[name]);

		Draw(name, size, pin, allow_resize, transparent);
		if (allow_resize)
		{
			size = ImGui::GetWindowSize();
			floatinSectionResize[name] = size;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 1.f, pos.y));
			offset.x += 1.f;
		}
		ImGui::BeginChild((name + xorstr_("section3")).c_str(), ImVec2(size.x - offset.x, size.y - offset.y), 0);
	}

	void Text(std::string text, ImColor color)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImVec2 txtSize = ImGui::CalcTextSize(text.c_str());
		//Draw Text Middle
		drawList->AddText(ImVec2(pos.x + (size.x / 2) - (txtSize.x / 2), pos.y - (txtSize.y / 2)), color, text.c_str());
		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + txtSize.y + 3.f));
	}

	void EndSection()
	{
		ImGui::EndChild();
		ImGui::End();
		ImGui::PopStyleColor(4);
	}
}*/

namespace UI
{
	//namespace FloatingSection
	//{
	//	std::unordered_map<std::string, ImGuiWindowFlags> floatingSectionFlags;
	//	std::unordered_map<std::string, bool> floatinSectionPins;
	//	std::unordered_map<std::string, ImVec2> floatinSectionResize;

	//	void DrawHeader(std::string name, ImVec2 size, float headerHeight, bool pin, bool allow_resize)
	//	{
	//		ImDrawList* drawList = ImGui::GetWindowDrawList();
	//		ImVec2 availableSize = ImGui::GetContentRegionAvail();
	//		ImVec2 pos = ImGui::GetCursorScreenPos();
	//		if (ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + availableSize.x, pos.y + headerHeight)))
	//		{
	//			if (!allow_resize)
	//				floatingSectionFlags[name] = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	//			else
	//				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	//		}
	//		else
	//		{
	//			if (!allow_resize)
	//				floatingSectionFlags[name] = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	//			else
	//				floatingSectionFlags[name] = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	//		}
	//		//2 rects because we wanna have a smooth transition without making the bottom part rounded.
	//		//Offset by 1 / removeal by 1 for the line around the section
	//		drawList->AddRectFilled(pos + ImVec2(1.f, 1.f), pos + (availableSize - ImVec2(1.f, 1.f)), Color::SectionBackgroundHeaderColor, 5.f);

	//		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
	//		drawList->AddText(ImVec2(pos.x + ((size.x / 2) - (textSize.x / 2)), pos.y + ((headerHeight / 2) - (textSize.y / 2))), Color::textColor, name.c_str());

	//		if (pin)
	//		{
	//			ImGui::PushFont(Font::icon);
	//			float iconSize = ImGui::GetFontSize();
	//			ImColor pinColor = Color::SectionPinColor;
	//			if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), ImVec2(pos.x + (size.x - 10.f), pos.y + ((headerHeight / 2) + (iconSize / 2)))))
	//			{
	//				pinColor = Color::SectionPinHoverColor;
	//				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	//					floatinSectionPins[name] = !floatinSectionPins[name];
	//			}
	//			if (!floatinSectionPins[name])
	//				drawList->AddText(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), pinColor, (char*)ICON_PIN_OUTLINE);
	//			else
	//				drawList->AddText(ImVec2(pos.x + (size.x - 10.f) - iconSize, pos.y + ((headerHeight / 2) - (iconSize / 2))), pinColor, (char*)ICON_PIN);
	//			ImGui::PopFont();
	//		}

	//		ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight));
	//	}

	//	void Draw(std::string name, ImVec2 size, bool pin, bool allow_resize)
	//	{
	//		ImDrawList* drawList = ImGui::GetWindowDrawList();
	//		ImVec2 availableSize = ImGui::GetContentRegionAvail();
	//		ImVec2 pos = ImGui::GetCursorScreenPos();
	//		float HeaderHeight = 30.f;
	//		bool hovering = false;

	//		drawList->AddRectFilled(pos, pos + availableSize, Color::SectionBackgroundRectColor, 5.f);

	//		if (ImGui::IsMouseHoveringRect(pos, pos + availableSize))
	//			hovering = true;

	//		float alpha = UI::Animation::GetAnimDataHover(name + xorstr_("section_impl"), hovering, 20.f, 0.8f);
	//		ImColor minimizeColor = Animation::InterpolateColor(Color::SectionBackgroundRectColor, Color::SectionBackgroundRectHoverColor, alpha);
	//		if (alpha > 0.f)
	//		{
	//			drawList->AddRectFilled(ImVec2(pos.x, (pos.y + (availableSize.y / 2) - (alpha * (availableSize.y / 2)))), ImVec2(pos.x + availableSize.x, pos.y + (availableSize.y / 2)), minimizeColor, 5.f);
	//			drawList->AddRectFilled(ImVec2(pos.x, pos.y + (availableSize.y / 2) - 7.f/*idk why i have to do this magic number tbh*/), ImVec2(pos.x + availableSize.x, (pos.y + (availableSize.y / 2)) + (alpha * (availableSize.y / 2))), minimizeColor, 5.f);
	//		}
	//		DrawHeader(name, size, HeaderHeight, pin, allow_resize);

	//		drawList->AddRectFilled(pos + ImVec2(1.f, 30.f), pos + (availableSize - 1.f), Color::SectionBackgroundColor, 5.f);
	//	}

	//	void BeginSection(std::string name, bool open, ImVec2 pos, ImVec2 size, bool pin, ImVec2 offset, bool allow_resize)
	//	{
	//		if (size.x == 0.f && size.y == 0.f)
	//			size = ImVec2(ImGui::GetContentRegionAvail().x - 10.f, ImGui::GetContentRegionAvail().y - 10.f);
	//		if (pos.x == 0.f && pos.y == 0.f)
	//			pos = ImGui::GetCursorScreenPos();
	//		if (allow_resize)
	//		{
	//			if (floatinSectionResize.find(name) == floatinSectionResize.end())
	//				floatinSectionResize[name] = size;
	//			size = floatinSectionResize[name];
	//		}
	//		if (pin)
	//			if (floatinSectionPins.find(name) == floatinSectionPins.end())
	//				floatinSectionPins[name] = false;
	//		if (floatingSectionFlags.find(name) == floatingSectionFlags.end())
	//		{
	//			if (!allow_resize)
	//				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	//			else
	//				floatingSectionFlags[name] = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	//		}
	//		if (pin)
	//			if (floatinSectionPins[name])
	//				ImGui::SetNextWindowFocus();
	//		ImGui::SetNextWindowPos(pos + 5.f, ImGuiCond_FirstUseEver);
	//		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	//		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(0, 0, 0, 0));
	//		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, Color::scrollbarColor.Value);
	//		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, Color::scrollbarHoverColor.Value);
	//		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, Color::scrollbarActiveColor.Value);
	//		ImGui::GetStyle().ScrollbarSize = 3.f;
	//		ImGui::Begin((name + xorstr_("##section2")).c_str(), &open, floatingSectionFlags[name]);

	//		Draw(name, size, pin, allow_resize);
	//		if (allow_resize)
	//		{
	//			size = ImGui::GetWindowSize();
	//			floatinSectionResize[name] = size;
	//			ImVec2 pos = ImGui::GetCursorScreenPos();
	//			ImGui::SetCursorScreenPos(ImVec2(pos.x + 1.f, pos.y));
	//			offset.x += 1.f;
	//		}
	//		ImGui::BeginChild((name + xorstr_("section3")).c_str(), ImVec2(size.x - offset.x, size.y - offset.y), 0);
	//	}

	//	void EndSection()
	//	{
	//		ImGui::EndChild();
	//		ImGui::End();
	//		ImGui::PopStyleColor(4);
	//	}
	//}

	namespace Map
	{
	}
}
