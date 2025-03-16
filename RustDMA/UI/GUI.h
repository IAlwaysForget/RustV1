#pragma once
#include <functional>
#include <string>
#include <vector>

#include "imgui.h"
#include "xorstr.hpp"

namespace navigation
{
	extern void setActive(std::string name);

	extern bool isActive(std::string name);

	extern std::string lastActive();
}

namespace Font
{
	extern ImFont* icon;
	extern ImFont* iconlarge;
	extern ImFont* sidebarIcon;
	extern ImFont* bigText;
	extern ImFont* fontawesome_duotone;
	extern ImFont* overlayFont;
	extern ImFont* overlayFontBold;
	extern void initializeFonts();
}

namespace Animation
{
	extern float GetAnimData(std::string key, ImVec2 start, ImVec2 end, float animationPixels, float animationDuration, float start_value = 0.f, float end_value = 1.f);
	extern float GetAnimData(std::string key, bool hovering, float animationPixels, float animationDuration, float start = 0.f, float end = 1.f);
	extern float EaseInOut(float startValue, float endValue, float t);
	extern void resetAnimData(std::string key);
}

namespace UII
{
	extern ImGuiWindowFlags menuFlags;

	extern void Start();

	extern void End();

	//Basicly thebackground
	namespace MainWindow
	{
		extern void background();
	}

	//Top part of the UI, the header as we should call it
	namespace Nav
	{
		extern bool notification;
		extern ImVec2 navSize;
		//extern HWND windowHWND;

		extern bool notificationButton(ImVec2 pos, ImVec2 sizeAvail);

		extern bool minimizeButton(ImVec2 pos, ImVec2 sizeAvail);

		extern bool ExitButton(ImVec2 pos, ImVec2 sizeAvail);

		extern void headerBackground();

		extern void header();
	}

	namespace Sidebar
	{
		extern ImVec2 barSize;

		extern void sidebarBackground();

		extern void logo(ImVec2 pos, ImVec2 sizeAvail);

		extern void clock(ImVec2 pos, ImVec2 sizeAvail);

		extern void logoAndClock();

		extern void startSidebar();

		extern void button(std::string name, std::string icon, float customIconPadding);
	}

	namespace Modal
	{
		extern ImVec2 tempPos;

		extern void modalBackground(float alpha);

		extern bool ExitButton(ImVec2 pos, ImVec2 sizeAvail);

		extern void modalHeaderName(std::string name, ImVec2 pos, ImVec2 sizeAvail);

		extern void modalHeader(std::string name, ImVec2 pos, ImVec2 sizeAvail, bool* open);

		extern void blurMainWindow(ImVec2 pos, float alpha);

		extern bool clickedOffModal(ImVec2 pos);

		extern void startModal(std::string name, bool* open, ImVec2 pos, std::string windowName = "", ImVec2 size = ImVec2(300, 200));

		extern void endModal();
	}

	namespace Config
	{
		extern ImVec2 configSize;
		extern std::vector<std::string> configs;
		extern bool saveConfigModal;
		extern int currentConfig;

		extern void getAllConfigs();

		extern void createConfig(std::string name);

		extern void grayLineBackground();

		extern void drawBackground();

		extern void configHeader();

		extern void beginConfig();

		extern void configOption(std::string name, int index);

		extern void endConfig();
	}

	namespace Controls
	{
		extern void drawBackground();

		extern void drawBackgroundColor(float alpha);

		extern bool isHoveringGroup(ImVec2 pos);

		extern void sectionHeaderName(std::string name, ImVec2 pos, ImVec2 sizeAvail);

		extern void sectionHeader(std::string name, ImVec2 pos, ImVec2 sizeAvail);

		extern void beginSection(std::string name, ImVec2 size = {265, 450});

		extern void endSection();

		extern void nextSection();

		extern void sameSection();

		extern bool Toggle(std::string name, std::string description, bool* toggle);

		extern bool ColorPicker(std::string name, std::string description, ImColor* color);

		extern bool ColorPicker(std::string name, std::string description, ImColor* color, ImColor* color2);

		extern bool Button2(std::string name, std::string description);

		extern bool Button(std::string name, std::string description);

		extern void Hotkey(std::string name, int* key);

		extern bool Input(std::string name, char* buf, size_t bufSize);

		extern bool Input(std::string name, std::string* buf, size_t bufSize);

		extern void InputInt(std::string name, int* buf);

		extern void Slider(std::string name, float* out, int min, int max);

		extern bool SliderInt(std::string name, int* out, int min, int max);

		extern void SliderRange(std::string name, float* out, float* out2, float min, float max, float power);

		extern void Listbox(std::string name, int* current_item, const char* const items[], int items_count, int height_in_items = 12);

		extern bool Listbox(std::string name, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = 12);

		extern void Combo(std::string name, std::vector<std::string> items, int& index);
	}

	namespace ModalController
	{
		extern bool crosshairEditor;
		//Center Dot
		extern bool dot;

		//Gap between Dot and 4 things
		extern float gap;

		//Length of the 4 things
		extern float Length;

		//Thickness of lines & dot
		extern float Thicc;

		extern void CrosshairDisplay();

		extern void DisplayCrosshairOnScreen(ImVec2 pos, ImVec2 sizeAvail);

		extern void Input(std::string name, char* buf, size_t bufSize, float width = 100.f, bool alignMiddle = false);

		extern void InputInt(std::string name, int* buf, size_t bufSize, float width = 100.f, bool alignMiddle = false);

		extern bool Button(std::string name, float width = 100.f, float heigth = 50.f);

		extern void Slider(std::string name, float* out, int min, int max, float width = 100);
	}
}

namespace FloatingSection
{
	extern void BeginSection(std::string name, bool open = true, ImVec2 pos = ImVec2(0, 0), ImVec2 size = ImVec2(0, 0), bool pin = false, ImVec2 offset = ImVec2(5, 40), bool allow_resize = false, bool transparent = false);
	extern void BeginSection(std::string name, bool open, ImVec2* pos, ImVec2 size = ImVec2(0, 0), bool pin = false, ImVec2 offset = ImVec2(5, 40), bool allow_resize = false, bool transparent = false, bool change_size = false);
	extern void Text(std::string text, ImColor color);
	extern void EndSection();
}

/*
namespace FloatinSection
{
	extern void DrawHeader(std::string name, ImVec2 size, float headerHeight, bool pin, bool allow_resize);
	extern void Draw(std::string name, ImVec2 size, bool pin, bool allow_resize);

	extern void BeginSection(std::string name, bool open, ImVec2 pos, ImVec2 size, bool pin = false, ImVec2 offset = ImVec2(5, 40), bool allow_resize = false, bool transparent = false);

	extern void EndSection();
}
*/

struct TabButtons
{
	std::string Name;
	const char8_t* icon;
	float icon_padding = 9.5f;

	TabButtons(const std::string& Name, const char8_t* icon, float icon_padding) : Name(Name), icon(icon), icon_padding(icon_padding)
	{
	}
};

class GUI
{
private:
	ImVec2 startPos;

public:
	GUI()
	{
	}

	void Begin();
	void End();
	void DisplayBrand(std::string text);

	void BeginMenu();
	void EndMenu();

	void DrawBackground();

	void DisplayTab(std::vector<TabButtons> buttons);
	void DisplayConfigs();
	//Modals should always be rendered at the bottom.
	void DisplayModals();
	//Padding to align Control settings properly
	void DisplayPadding();
	//Allows you to move the window around on the Nav header
	void MoveWindow();

	bool IsActive(std::string name);
};
