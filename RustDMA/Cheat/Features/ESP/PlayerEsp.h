#pragma once
#include "Misc/CheatFunction.h"
extern std::mutex PlayerCacheMatrix;
extern std::shared_ptr<CheatFunction> CachePlayers;
extern void PlayerEsp();
extern std::shared_ptr<CheatFunction> UpdatePlayers;
extern std::shared_ptr<CheatFunction> UpdatePlayersPerFrame;