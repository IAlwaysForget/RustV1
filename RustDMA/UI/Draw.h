#pragma once
#include "GUI.h"
#include "Fuser/Fuser.h"

enum FontAlignment
{
	Left = 0,
	Centre = 1,
	Right = 2,
	CentreLeft = 3,
	CentreRight = 4,
	CentreCentre = 5,
	None = 6
};

class c_draw : public GUI
{
public:
	c_draw()
	{
	}

	~c_draw() = default;

	Fuser fuser;

	void Init(Fuser& fuse);
	void Run();

	void Visuals();
	void Aimbot();
	void Miscellaneous();
	void Important();
	void Weapons();
	void Movement();
	void SkinChanger();
	void Radar();

	void DrawString(std::string text, ImVec2 pos, ImColor color, int fontSize = 0, FontAlignment alignment = FontAlignment::None);
	void DrawRect(ImVec2 min, int width, int height, float thickness, ImColor color);
	void DrawFilledRect(ImVec2 min, int width, int height, ImColor color);
	void DrawLine(ImVec2 p1, ImVec2 p2, ImColor color, float thickness);
	void DrawCircle(ImVec2 center, float raduis, ImColor color, int segments, float thickness);
	void DrawFilledCircle(ImVec2 center, float raduis, ImColor color, int segments);
};
