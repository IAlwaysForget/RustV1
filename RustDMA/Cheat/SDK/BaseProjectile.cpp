#include "Pch.h"
#include "Globals.h"
#include "BaseProjectile.h"
#include <iostream>
// We are hard coding these values, if we read them from the game and cache then if the user has issues and tries to reload the cheat 2 times the recoil will be nearly 0 no matter what.
// As we are a DMA cheat, we can't risk that. So we are hardcoding these values.
std::unordered_map<std::string, float> OriginalRecoilYawMin = { {xorstr_("Eoka Pistol"),0},{xorstr_("Compound Bow"),-3},
{xorstr_("Crossbow"),-3},
{xorstr_("Bolt Action Rifle"),-4},
{xorstr_("Assault Rifle - ICE"),1.5},
{xorstr_("Abyss Assault Rifle"),1.5},
{xorstr_("Assault Rifle"),1.5},
{xorstr_("Thompson"),-1},
{xorstr_("Spas - 12 Shotgun"),4},
{xorstr_("Snowball Gun"),-1},
{xorstr_("Semi - Automatic Rifle"),-0.5},
{xorstr_("SKS"),-0.5},
{xorstr_("Semi - Automatic Pistol"),-1},
{xorstr_("Revolver"),-1},
{xorstr_("Python Revolver"),-2},
{xorstr_("Pump Shotgun"),4},
{xorstr_("Prototype 17"),-1},
{xorstr_("Nailgun"),-1},
{xorstr_("Waterpipe Shotgun"),4},
{xorstr_("Dragon Rocket Launcher"),5},
{xorstr_("L96 Rifle"),-2},
{xorstr_("Hunting Bow"),-3},
{xorstr_("Double Barrel Shotgun"),8},
{xorstr_("Rocket Launcher"),5},
//{xorstr_("Flame Thrower"),0},
{xorstr_("Legacy bow"),-3},
{xorstr_("HMLMG"),-1.25},
{xorstr_("Homing Missile Launcher"),8},
{xorstr_("LR - 300 Assault Rifle"),-1},
{xorstr_("M4 Shotgun"),2},
{xorstr_("M92 Pistol"),-1},
{xorstr_("MP5A4"),-1},
{xorstr_("Multiple Grenade Launcher"),5},
{xorstr_("M39 Rifle"),1.5},
{xorstr_("M249"),1.25}, 
{xorstr_("Custom SMG"),-1},
{xorstr_("LR-300 Assault Rifle"),-1},
{xorstr_("Semi-Automatic Rifle"),-0.5},
{xorstr_("Semi-Automatic Pistol"),-1},
{xorstr_("Spas-12 Shotgun"),4}, 
};
std::unordered_map<std::string, float> OriginalRecoilYawMax = { {xorstr_("Eoka Pistol"),0},{xorstr_("Multiple Grenade Launcher"),10},
{xorstr_("MP5A4"),1},
{xorstr_("M92 Pistol"),1},
{xorstr_("M4 Shotgun"),4},
{xorstr_("M39 Rifle"),2.5},
{xorstr_("M249"),2.25},
{xorstr_("Semi-Automatic Rifle"),0.5},
{xorstr_("SKS"),0.5},
{xorstr_("Snowball Gun"),1},
{xorstr_("Double Barrel Shotgun"),15},
{xorstr_("Spas-12 Shotgun"),8},
{xorstr_("Semi-Automatic Pistol"),1},
{xorstr_("Waterpipe Shotgun"),8},
{xorstr_("Thompson"),1},
{xorstr_("Revolver"),1},
{xorstr_("Python Revolver"),2},
{xorstr_("Pump Shotgun"),8},
{xorstr_("Prototype 17"),1},
{xorstr_("Nailgun"),1},
{xorstr_("Hunting Bow"),3},
{xorstr_("Rocket Launcher"),10},
{xorstr_("L96 Rifle"),2},
{xorstr_("LR-300 Assault Rifle"),1},
{xorstr_("Dragon Rocket Launcher"),10},
{xorstr_("Legacy bow"),3},
{xorstr_("Abyss Assault Rifle"),2.5},
{xorstr_("HMLMG"),-2.5},
{xorstr_("Homing Missile Launcher"),8},
//{xorstr_("Flame Thrower"),0},
{xorstr_("Custom SMG"),1},
{xorstr_("Assault Rifle"),2.5},
{xorstr_("Assault Rifle - ICE"),2.5},
{xorstr_("Bolt Action Rifle"),4},
{xorstr_("Crossbow"),3},
{xorstr_("Compound Bow"),3}, };
std::unordered_map<std::string, float> OriginalRecoilPitchMin = { {xorstr_("Eoka Pistol"),0},{xorstr_("Compound Bow"),-3},
{xorstr_("Crossbow"),-3},
{xorstr_("Bolt Action Rifle"),-2},
{xorstr_("Assault Rifle - ICE"),-2.5},
{xorstr_("Assault Rifle"),-2.5},
{xorstr_("Multiple Grenade Launcher"),-15},
{xorstr_("Custom SMG"),-1.5},
//{xorstr_("Flame Thrower"),0},
{xorstr_("Homing Missile Launcher"),-8},
{xorstr_("HMLMG"),-3},
{xorstr_("Abyss Assault Rifle"),-2.5},
{xorstr_("Legacy bow"),-3},
{xorstr_("LR-300 Assault Rifle"),-2},
{xorstr_("Dragon Rocket Launcher"),-15},
{xorstr_("Rocket Launcher"),-15},
{xorstr_("L96 Rifle"),-1},
{xorstr_("Hunting Bow"),-3},
{xorstr_("Double Barrel Shotgun"),-10},
{xorstr_("Revolver"),-3},
{xorstr_("Python Revolver"),-15},
{xorstr_("Pump Shotgun"),-10},
{xorstr_("Prototype 17"),-2},
{xorstr_("Nailgun"),-3},
{xorstr_("Semi-Automatic Rifle"),-2},
{xorstr_("SKS"),-2},
{xorstr_("Thompson"),-1.5},
{xorstr_("Waterpipe Shotgun"),-10},
{xorstr_("Semi-Automatic Pistol"),-2},
{xorstr_("Spas-12 Shotgun"),-10},
{xorstr_("Snowball Gun"),-2},
{xorstr_("MP5A4"),-1},
{xorstr_("M92 Pistol"),-7},
{xorstr_("M4 Shotgun"),-7},
{xorstr_("M39 Rifle"),-3},
{xorstr_("M249"),-3},
{xorstr_("Minigun"),-0.5}, };
std::unordered_map<std::string, float> OriginalRecoilPitchMax = { {xorstr_("Eoka Pistol"),0}, {xorstr_("M4 Shotgun"),-10},
{xorstr_("M39 Rifle"),-4},
{xorstr_("M92 Pistol"),-8},
{xorstr_("MP5A4"),-3},
{xorstr_("M249"),-4},
{xorstr_("Thompson"),-2},
{xorstr_("Semi-Automatic Rifle"),-3},
{xorstr_("SKS"),-3},
{xorstr_("Waterpipe Shotgun"),-14},
{xorstr_("Semi-Automatic Pistol"),-2.5},
{xorstr_("Spas-12 Shotgun"),-14},
{xorstr_("Snowball Gun"),-3},
{xorstr_("Double Barrel Shotgun"),-15},
{xorstr_("Revolver"),-6},
{xorstr_("Python Revolver"),-16},
{xorstr_("Pump Shotgun"),-14},
{xorstr_("Nailgun"),-6},
{xorstr_("Prototype 17"),-2.5},
{xorstr_("Dragon Rocket Launcher"),-20},
{xorstr_("LR-300 Assault Rifle"),-3},
{xorstr_("L96 Rifle"),-1.5},
{xorstr_("Hunting Bow"),-6},
{xorstr_("Custom SMG"),-2},
//{xorstr_("Flame Thrower"),0},
{xorstr_("Homing Missile Launcher"),-8},
{xorstr_("Abyss Assault Rifle"),-3.5},
{xorstr_("Legacy bow"),-6},
{xorstr_("Multiple Grenade Launcher"),-20},
{xorstr_("Assault Rifle"),-3.5},
{xorstr_("Assault Rifle - ICE"),-3.5},
{xorstr_("Bolt Action Rifle"),-3},
{xorstr_("Crossbow"),-6},
{xorstr_("Compound Bow"),-6}, 
{xorstr_("Rocket Launcher"),-20},
{xorstr_("HMLMG"),-4}, 
{xorstr_("Minigun"),-1},
};
std::unordered_map<std::string, float> OriginalAimSway = { {xorstr_("Abyss Assault Rifle"),2},
{xorstr_("Assault Rifle"),2},
{xorstr_("Assault Rifle - ICE"),2},
{xorstr_("Bolt Action Rifle"),3},
{xorstr_("Compound Bow"),5},
{xorstr_("Crossbow"),3},
{xorstr_("Custom SMG"),3},
{xorstr_("Double Barrel Shotgun"),5},
{xorstr_("Dragon Rocket Launcher"),2},
{xorstr_("HMLMG"),2},
{xorstr_("Homing Missile Launcher"),2},
{xorstr_("Hunting Bow"),5},
{xorstr_("L96 Rifle"),2},
{xorstr_("Legacy bow"),5},
{xorstr_("M249"),2},
{xorstr_("LR-300 Assault Rifle"),1},
{xorstr_("M39 Rifle"),1},
{xorstr_("M4 Shotgun"),3},
{xorstr_("M92 Pistol"),2},
{xorstr_("MP5A4"),2},
{xorstr_("Multiple Grenade Launcher"),2},
{xorstr_("Nailgun"),2},
{xorstr_("Prototype 17"),2},
{xorstr_("Pump Shotgun"),5},
{xorstr_("Python Revolver"),2},
{xorstr_("Revolver"),2},
{xorstr_("Rocket Launcher"),2},
{xorstr_("Semi-Automatic Pistol"),2},
{xorstr_("Semi-Automatic Rifle"),1},
{xorstr_("SKS"),1},
{xorstr_("Snowball Gun"),2},
{xorstr_("Spas-12 Shotgun"),4},
{xorstr_("Thompson"),3},
{xorstr_("Waterpipe Shotgun"),5}, };

std::unordered_map<std::string, float> OriginalAimCone = { {xorstr_("Abyss Assault Rifle"),0.2},
{xorstr_("Assault Rifle"),0.2},
{xorstr_("Assault Rifle - ICE"),0.2},
{xorstr_("Bolt Action Rifle"),0},
{xorstr_("Compound Bow"),0.2},
{xorstr_("Crossbow"),1},
{xorstr_("Custom SMG"),0.5},
{xorstr_("Double Barrel Shotgun"),0.5},
{xorstr_("Dragon Rocket Launcher"),2.25},
{xorstr_("HMLMG"),0.2},
{xorstr_("Homing Missile Launcher"),2.25},
{xorstr_("Hunting Bow"),1},
{xorstr_("L96 Rifle"),0},
{xorstr_("Legacy bow"),1},
{xorstr_("LR-300 Assault Rifle"),0.2},
{xorstr_("M249"),0.2},
{xorstr_("M39 Rifle"),0.1},
{xorstr_("M4 Shotgun"),0},
{xorstr_("M92 Pistol"),1},
{xorstr_("MP5A4"),0.5},
{xorstr_("Multiple Grenade Launcher"),2.25},
{xorstr_("Nailgun"),0.75},
{xorstr_("Prototype 17"),0.75},
{xorstr_("Pump Shotgun"),0},
{xorstr_("Python Revolver"),0.5},
{xorstr_("Revolver"),0.75},
{xorstr_("Rocket Launcher"),2.25},
{xorstr_("Semi-Automatic Pistol"),0.75},
{xorstr_("Semi-Automatic Rifle"),0.25},
{xorstr_("SKS"),0.25},
{xorstr_("Snowball Gun"),2.25},
{xorstr_("Spas-12 Shotgun"),0},
{xorstr_("Thompson"),0.5},
{xorstr_("Waterpipe Shotgun"),1},
{xorstr_("Minigun"),0},
 };
std::unordered_map<std::string, float> OriginalHipAimCone = {
{xorstr_("Waterpipe Shotgun"),2},
{xorstr_("Spas-12 Shotgun"),1.8},
{xorstr_("Snowball Gun"),1.8},
{xorstr_("Thompson"),1.8},
{xorstr_("Pump Shotgun"),1.8},
{xorstr_("Python Revolver"),2},
{xorstr_("Revolver"),2},
{xorstr_("Rocket Launcher"),1.8},
{xorstr_("Semi-Automatic Pistol"),2},
{xorstr_("Semi-Automatic Rifle"),1.8},
{xorstr_("SKS"),1.8},
{xorstr_("M4 Shotgun"),0.7},
{xorstr_("M92 Pistol"),1.8},
{xorstr_("MP5A4"),3},
{xorstr_("Multiple Grenade Launcher"),1.8},
{xorstr_("Nailgun"),2},
{xorstr_("Prototype 17"),2},
{xorstr_("Hunting Bow"),1},
{xorstr_("L96 Rifle"),1.8},
{xorstr_("Legacy bow"),1},
{xorstr_("LR-300 Assault Rifle"),3},
{xorstr_("M249"),4},
{xorstr_("M39 Rifle"),3},
{xorstr_("Homing Missile Launcher"),1.8},
{xorstr_("HMLMG"),4},
{xorstr_("Dragon Rocket Launcher"),1.8},
{xorstr_("Double Barrel Shotgun"),2},
{xorstr_("Custom SMG"),1.8},
{xorstr_("Crossbow"),1.8},
{xorstr_("Compound Bow"),0.2},
{xorstr_("Bolt Action Rifle"),1.8},
{xorstr_("Assault Rifle - ICE"),2.5},
{xorstr_("Assault Rifle"),2.5},
{xorstr_("Abyss Assault Rifle"),2.5},
{xorstr_("Minigun"),0.8},

};
std::unordered_map<std::string, float> OriginalAimConeMaxPenalty = { {xorstr_("Thompson"),1},
{xorstr_("Snowball Gun"),0},
{xorstr_("Spas-12 Shotgun"),0},
{xorstr_("Waterpipe Shotgun"),0},
{xorstr_("Pump Shotgun"),0},
{xorstr_("Python Revolver"),3},
{xorstr_("Revolver"),2.5},
{xorstr_("Rocket Launcher"),0},
{xorstr_("Semi-Automatic Pistol"),1.5},
{xorstr_("Semi-Automatic Rifle"),1.5},
{xorstr_("SKS"),1.5},
{xorstr_("M4 Shotgun"),0},
{xorstr_("M92 Pistol"),1.5},
{xorstr_("MP5A4"),1},
{xorstr_("Multiple Grenade Launcher"),0},
{xorstr_("Nailgun"),1},
{xorstr_("Prototype 17"),1.5},
{xorstr_("Hunting Bow"),0},
{xorstr_("L96 Rifle"),0},
{xorstr_("Legacy bow"),0},
{xorstr_("LR-300 Assault Rifle"),1},
{xorstr_("M249"),3},
{xorstr_("M39 Rifle"),1},
{xorstr_("Homing Missile Launcher"),0},
{xorstr_("HMLMG"),3},
{xorstr_("Dragon Rocket Launcher"),0},
{xorstr_("Double Barrel Shotgun"),0},
{xorstr_("Custom SMG"),1},
{xorstr_("Crossbow"),0},
{xorstr_("Compound Bow"),0},
{xorstr_("Bolt Action Rifle"),0},
{xorstr_("Assault Rifle - ICE"),1},
{xorstr_("Assault Rifle"),1},
{xorstr_("Abyss Assault Rifle"),1}, 
{xorstr_("Minigun"),0.0},
 };

uint64_t BaseProjectile::GetClass()
{
	return Class;
}
BaseProjectile::BaseProjectile(uint64_t address)
{
//	printf("[BaseProjectile] Initialized\n");
	this->Class = address;
	if (Class == 0)
		return;
	//printf("[BaseProjectile] Class: 0x%llX\n", Class);
	this->RecoilProperties = mem.Read<uint64_t>(Class + RecoilProperties);
	//printf("[BaseProjectile] RecoilProperties: 0x%llX\n", RecoilProperties);
	if (IsValidWeapon())
	{
		uint64_t recoiloverride = mem.Read<uint64_t>(RecoilProperties + RecoilOverride);
		if (recoiloverride == 0)
			RecoilOverride = RecoilProperties; // some guns don't have a new recoil pattern and use an old one. which is located at recoilproperties instead of override
		else
			RecoilOverride = recoiloverride;
		WriteAmmoName();
		WriteProjectileWeaponMod();
		VelocityScale = mem.Read<float>(Class + ProjectileVelocityScale);

	}
}
float BaseProjectile::GetVelocityScale()
{
	if (Class == 0)
		return 0;
	return VelocityScale;
}
void BaseProjectile::WriteProjectileWeaponMod()
{
	if (Class == 0)
		return;
	if (GetAmmoName().size() != 0 && IsValidWeapon())
	{
		uint64_t childrenlist = mem.Read<uint64_t>(Class + Children);
		if (childrenlist == 0)
		{
			ProjectileModifier.Enabled = false;
			return;
		}
		auto handle = mem.CreateScatterHandle();
		uint64_t childrenlistsize = 0;
		mem.AddScatterReadRequest(handle, childrenlist + 0x18, reinterpret_cast<void*>(&childrenlistsize), sizeof(uint32_t));
		uint64_t childrenlistbuffer = 0;
		mem.Read<uint64_t>(childrenlist + 0x10);
		mem.AddScatterReadRequest(handle, childrenlist + 0x10, reinterpret_cast<void*>(&childrenlistbuffer), sizeof(uint64_t));
		mem.ExecuteReadScatter(handle);
		mem.CloseScatterHandle(handle);

		if (childrenlistsize == 0)
		{
			ProjectileModifier.Enabled = false;
			return;
		}
		for (int i = 0; i < childrenlistsize; i++)
		{
			uintptr_t childentity = mem.Read<uintptr_t>(childrenlistbuffer + (0x20 + (i * 0x8)));
			if (childentity == 0)
			{
				ProjectileModifier.Enabled = false;
				continue;
			}
			ProjectileWeaponModModifier mod;
			if (mem.Read<bool>(childentity + IsMuzzleBoost))
			{
				mod.Enabled = true;
				mod.Scalar = 0.9f;
				ProjectileModifier = mod;
				break;
			}
			else if (mem.Read<bool>(childentity + IsSilencer))
			{
				mod.Enabled = true;
				mod.Scalar = 0.75f;
				ProjectileModifier = mod;
				break;
			}
			else if (mem.Read<bool>(childentity + IsMuzzleBrake))
			{
				mod.Enabled = true;
				mod.Scalar = 0.8f;
				ProjectileModifier = mod;
				break;
			}
				mod.Enabled = false;
				mod.Scalar = 1.0f;
				ProjectileModifier = mod;
			
	

		}
	}
}

ProjectileWeaponModModifier BaseProjectile::GetProjectileWeaponMod()
{
	if(Class ==0)
		return ProjectileWeaponModModifier();
	return ProjectileModifier;
}
bool BaseProjectile::IsValidWeapon()
{

	return Class != 0 &&RecoilProperties != 0;
}
inline std::string LastWeaponName;
inline float LastRecoilYawMin = 0;
inline float LastRecoilYawMax = 0;
inline float LastRecoilPitchMin = 0;
inline float LastRecoilPitchMax = 0;
inline float LastAimSway = 0;
inline float LastAimCone = 0;
inline float LastHipAimCone = 0;
inline float LastAimConeMaxPenalty = 0;
void BaseProjectile::WriteRecoilYaw(VMMDLL_SCATTER_HANDLE handle, const std::string& name, int percent)
{
	if (Class == 0)
		return;
	if (OriginalRecoilYawMax.find(name) == OriginalRecoilYawMax.end())
	{
	//	OriginalRecoilYawMin[name] = mem.Read<float>(RecoilOverride + RecoilYawMin);
	//	OriginalRecoilYawMax[name] = mem.Read<float>(RecoilOverride + RecoilYawMax);
	//	std::cout << "{" << "xorstr_(" << "\"" << name << "\"" << ")" << "," << OriginalRecoilYawMax[name] << "}," << std::endl;
		return;
		// for adding new weapons
		
	//	if (name == "")1
	//		return;
	//	std::string name2 = name + GetAmmoName();
	//	printf("dsgdgs: %s\n",name2.c_str());
		//	std::cout << "{" << "xorstr_(" << "\"" << name2 << "\"" << ")" << "," << ".0f" << "}," << std::endl;
		//	OriginalRecoilYawMax2[name2] = 0;
	}
	if(name != LastWeaponName)
	{
	LastWeaponName = name;
	LastRecoilYawMin = OriginalRecoilYawMin[name];
	LastRecoilYawMax = OriginalRecoilYawMax[name];
	LastRecoilPitchMin = OriginalRecoilPitchMin[name];
	LastRecoilPitchMax = OriginalRecoilPitchMax[name];
	LastAimSway = OriginalAimSway[name];
	LastAimCone = OriginalAimCone[name];
	LastHipAimCone = OriginalHipAimCone[name];
	LastAimConeMaxPenalty = OriginalAimConeMaxPenalty[name];
	}
	// print last yaw min and max
	float yawminpercent = LastRecoilYawMin * (percent / 100.0f);
	float yawmaxpercent = LastRecoilYawMax * (percent / 100.0f);
	if (mem.Read<float>(RecoilOverride + RecoilPitchMin) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilYawMin, yawminpercent);
	if (mem.Read<float>(RecoilOverride + RecoilPitchMax) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilYawMax, yawmaxpercent);
}

void BaseProjectile::WriteRecoilPitch(VMMDLL_SCATTER_HANDLE handle,const std::string& name, int percent)
{
	if (Class == 0)
		return;
	if (OriginalRecoilPitchMin.find(name) == OriginalRecoilPitchMin.end())
	{
		

	//	OriginalRecoilPitchMin[name] = mem.Read<float>(RecoilOverride + RecoilPitchMin);
	//OriginalRecoilPitchMax[name] = mem.Read<float>(RecoilOverride + RecoilPitchMax);
	//	std::cout << "{" << "xorstr_(" << "\"" << name << "\"" << ")" << "," << OriginalRecoilPitchMin[name] << "}," << std::endl;
		return;
	}
	if (name != LastWeaponName)
	{
		LastWeaponName = name;
		LastRecoilYawMin = OriginalRecoilYawMin[name];
		LastRecoilYawMax = OriginalRecoilYawMax[name];
		LastRecoilPitchMin = OriginalRecoilPitchMin[name];
		LastRecoilPitchMax = OriginalRecoilPitchMax[name];
		LastAimSway = OriginalAimSway[name];
		LastAimCone = OriginalAimCone[name];
		LastHipAimCone = OriginalHipAimCone[name];
		LastAimConeMaxPenalty = OriginalAimConeMaxPenalty[name];
	}
	float pitchminpercent = LastRecoilPitchMin * (percent / 100.0f);
	float pitchmaxpercent = LastRecoilPitchMax * (percent / 100.0f);
	if (mem.Read<float>(RecoilOverride + RecoilPitchMin) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilPitchMin, pitchminpercent);
	if (mem.Read<float>(RecoilOverride + RecoilPitchMax) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilPitchMax, pitchmaxpercent);
}

void BaseProjectile::WriteSway(VMMDLL_SCATTER_HANDLE handle,const std::string& name, int percent)
{
	if (Class == 0)
		return;
	if (!OriginalAimSway.contains(name))
	{
		//float sway = mem.Read<float>(Class + AimSway);
		//std::cout << "{" << "xorstr_(" << "\"" << name << "\"" << ")" << "," << sway << "}," << std::endl;
		//OriginalAimSway[name] = sway;
		return;
	}
	if (name != LastWeaponName)
	{
		LastWeaponName = name;
		LastRecoilYawMin = OriginalRecoilYawMin[name];
		LastRecoilYawMax = OriginalRecoilYawMax[name];
		LastRecoilPitchMin = OriginalRecoilPitchMin[name];
		LastRecoilPitchMax = OriginalRecoilPitchMax[name];
		LastAimSway = OriginalAimSway[name];
		LastAimCone = OriginalAimCone[name];
		LastHipAimCone = OriginalHipAimCone[name];
		LastAimConeMaxPenalty = OriginalAimConeMaxPenalty[name];
	}
	float swaypercent = LastAimSway * (percent / 100.0f);
	if (mem.Read<float>(Class + AimSway) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, Class + AimSway, swaypercent);
}

void BaseProjectile::WriteAimcone(VMMDLL_SCATTER_HANDLE handle,const std::string& name, int percent)
{

	if (Class == 0)
		return;
	if (!OriginalAimCone.contains(name) || !OriginalHipAimCone.contains(name) || !OriginalAimConeMaxPenalty.contains(name))
	{

	//		std::cout << "{" << "xorstr_(" << "\"" << name << "\"" << ")" << "," << mem.Read<float>(Class + AimConeMaxPenalty) << "}," << std::endl;
		return;
	}
	if (name != LastWeaponName)
	{
		LastWeaponName = name;
		LastRecoilYawMin = OriginalRecoilYawMin[name];
		LastRecoilYawMax = OriginalRecoilYawMax[name];
		LastRecoilPitchMin = OriginalRecoilPitchMin[name];
		LastRecoilPitchMax = OriginalRecoilPitchMax[name];
		LastAimSway = OriginalAimSway[name];
		LastAimCone = OriginalAimCone[name];
		LastHipAimCone = OriginalHipAimCone[name];
		LastAimConeMaxPenalty = OriginalAimConeMaxPenalty[name];
	}
	float aimconepercent = LastAimCone * (percent / 100.0f);
	float hipaimconepercent = LastHipAimCone * (percent / 100.0f);
	float aimconemaxpenaltypercent = LastAimConeMaxPenalty * (percent / 100.0f);
	if (mem.Read<float>(Class + AimCone) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, Class + AimCone, aimconepercent);
	if (mem.Read<float>(Class + AimConeHip) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, Class + AimConeHip, hipaimconepercent);
	if (mem.Read<float>(Class + AimConeMaxPenalty) != 0.0f);
	mem.AddScatterWriteRequest<float>(handle, Class + AimConeMaxPenalty, aimconemaxpenaltypercent);
	float sightedscale = 1 * (percent / 100.0f);
	float sway = mem.AddScatterWriteRequest<float>(handle, Class + AimConeSightedScale, sightedscale);
}


void BaseProjectile::WriteAutomatic(VMMDLL_SCATTER_HANDLE handle, const std::string& name, bool value)
{
	if (!OriginalAimCone.contains(name) || !OriginalHipAimCone.contains(name) || !OriginalAimConeMaxPenalty.contains(name))
	{
		return;
	}
	if(Class == 0)
		return;
	mem.AddScatterWriteRequest<bool>(handle,Class + Automatic,value);

}

void BaseProjectile::WriteAmmoName()
{
	if (Class == 0)
		return;
	PrimaryMagazine = mem.Read<uint64_t>(Class + PrimaryMagazine);
	if(PrimaryMagazine != 0)
	AmmoType = mem.Read<uint64_t>(PrimaryMagazine + AmmoType);
	if (AmmoType != 0)
	ShortName = mem.Read<uint64_t>(AmmoType + ShortName);
	if (ShortName != 0)
	mem.Read(ShortName + 0x14, reinterpret_cast<void*>(&AmmoName), sizeof(AmmoName));
	std::wstring tempname = AmmoName;
	AmmoNameString = std::string(tempname.begin(), tempname.end());
}

std::string BaseProjectile::GetAmmoName()
{
	return AmmoNameString;
}