#pragma once
#include "Misc/vector.h"
void RefreshViewMatrix();
Vector2 WorldToScreen(const Vector3& position);
extern std::unordered_map<int, Vector2> ScreenMap;
