#pragma once
#include "Misc/CheatFunction.h"
#include "SDK/BasePlayer.h"
extern void GetAimbotTarget();
extern void DrawAimbot();
extern std::shared_ptr<CheatFunction> AimbotKeyManager;
extern void Aimbot();
extern void QuickSortPlayers(std::vector<std::shared_ptr<BasePlayer>>& entities, int low, int high);
