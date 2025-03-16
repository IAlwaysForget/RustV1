#pragma once
#include "SDK/Item.h"

enum class WeaponID : int
{
	None = 0,
	AK = 1,
	Bolt = 2,
	Thompson = 3,
	SemiRifle = 4,
	MP5 = 5,
	M249 = 6,
	Revolver = 7,
	SemiPistol = 8,
	PumpShotgun = 9,
	Spas12 = 10,
	DoubleBarrel = 11,
	M4 = 12,
	HMLMG = 13,
	Bow = 14,
	Crossbow = 15,
	CompoundBow = 16,
	Python = 17,
	CustomSMG = 18,
	RocketLauncher = 19,
	Eoka = 20,
	FlameThrower = 21,
	HomingMissleLauncher = 22,
	L96 = 23,
	LR300 = 24,
	M39 = 25,
	M92 = 26,
	GrenadeLauncher = 27,
	NailGun = 28,
	Prototype = 29,
	SnowballGun = 30,
	Waterpipe = 31,
	JackHammer = 32,
	SalvagedIcepick = 33,
	Pickaxe = 34,
	StonePickaxe = 35,
	StoneHatchet = 36,
	CombatKnife = 37,
	Hatchet = 38,
	Rock = 39,
	Minigun = 40,
	SKS = 41,
	Chainsaw = 42,
};

extern std::unordered_map<std::string, WeaponID> WeaponIDS;
extern std::unordered_map<uint32_t, std::string> LocalItemNames;
extern std::atomic<std::shared_ptr<Item>> HeldItem;
extern void WeaponFunctions();
extern void DrawRecoil();
extern int GetWeaponHeirarchy(WeaponID id);