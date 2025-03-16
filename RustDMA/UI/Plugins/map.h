#pragma once
#include <string>
#include <atomic>
#include "imgui.h"
#include "Misc/vector.h"

namespace Map
{
	struct MapData
	{
		std::string Name;
		float Scale;
		ImVec2 Offset;
	};

	extern ImVec2 mapOffset;
	extern std::atomic<float> mapZoom;
	extern int CurrentMapLevel();
	extern void CreateMapDirectory();
	extern void RenderMap();
	extern void Goto(Vector3 WorldPos);
	extern bool ReloadMap;
	extern bool ReloadCorruptedMap;
	extern std::atomic<bool> CentreRadar;
}
