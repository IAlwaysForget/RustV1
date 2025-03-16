#include "Pch.h"
#include "MiscWeapons.h"

#include "context.h"
#include "MiscFeatures.h"
#include "globals.h"
#include "xorstr.hpp"
#include "Misc/CheatFunction.h"
#include <VMProtectSDK.h>

std::atomic<std::shared_ptr<Item>> HeldItem;
std::unordered_map<uint32_t, std::string> LocalItemNames;

std::unordered_map<std::string, WeaponID> WeaponIDS = {
	{xorstr_("Eoka Pistol"), WeaponID::Eoka},
	{xorstr_("M4 Shotgun"), WeaponID::M4},
	{xorstr_("M39 Rifle"), WeaponID::M39},
	{xorstr_("M92 Pistol"), WeaponID::M92},
	{xorstr_("MP5A4"), WeaponID::MP5},
	{xorstr_("M249"), WeaponID::M249},
	{xorstr_("Thompson"), WeaponID::Thompson},
	{xorstr_("Semi-Automatic Rifle"), WeaponID::SemiRifle},
	{xorstr_("Waterpipe Shotgun"), WeaponID::Waterpipe},
	{xorstr_("Semi-Automatic Pistol"), WeaponID::SemiPistol},
	{xorstr_("Spas-12 Shotgun"), WeaponID::Spas12},
	{xorstr_("Snowball Gun"), WeaponID::SnowballGun},
	{xorstr_("Double Barrel Shotgun"), WeaponID::DoubleBarrel},
	{xorstr_("Revolver"), WeaponID::Revolver},
	{xorstr_("Python Revolver"), WeaponID::Python},
	{xorstr_("Pump Shotgun"), WeaponID::PumpShotgun},
	{xorstr_("Nailgun"), WeaponID::NailGun},
	{xorstr_("Prototype 17"), WeaponID::Prototype},
	{xorstr_("Dragon Rocket Launcher"), WeaponID::RocketLauncher},
	{xorstr_("LR-300 Assault Rifle"), WeaponID::LR300},
	{xorstr_("L96 Rifle"), WeaponID::L96},
	{xorstr_("Hunting Bow"), WeaponID::Bow},
	{xorstr_("Custom SMG"), WeaponID::CustomSMG},
	{xorstr_("Flame Thrower"), WeaponID::FlameThrower},
	{xorstr_("Homing Missile Launcher"), WeaponID::HomingMissleLauncher},
	{xorstr_("Abyss Assault Rifle"), WeaponID::AK},
	{xorstr_("Legacy bow"), WeaponID::Bow},
	{xorstr_("Multiple Grenade Launcher"), WeaponID::GrenadeLauncher},
	{xorstr_("Assault Rifle"), WeaponID::AK},
	{xorstr_("Assault Rifle - ICE"), WeaponID::AK},
	{xorstr_("Bolt Action Rifle"), WeaponID::Bolt},
	{xorstr_("Crossbow"), WeaponID::Crossbow},
	{xorstr_("Compound Bow"), WeaponID::CompoundBow},
	{xorstr_("Rocket Launcher"), WeaponID::RocketLauncher},
	{xorstr_("HMLMG"), WeaponID::HMLMG},
	{xorstr_("Salvaged Icepick"), WeaponID::SalvagedIcepick},
	{xorstr_("Hatchet"), WeaponID::Hatchet},
	{xorstr_("Pickaxe"), WeaponID::Pickaxe},
	{xorstr_("Stone Pickaxe"), WeaponID::StonePickaxe},
	{xorstr_("Stone Hatchet	"), WeaponID::StoneHatchet},
	{xorstr_("Jackhammer"), WeaponID::JackHammer},
	{xorstr_("Combat Knife"), WeaponID::CombatKnife},
	{xorstr_("Minigun"), WeaponID::Minigun},
	{xorstr_("SKS"), WeaponID::SKS},
	{xorstr_("Chainsaw"), WeaponID::Chainsaw},

};
const std::vector<std::string> Tools = {
	xorstr_("Jackhammer"),
	xorstr_("Hatchet"),
	xorstr_("Skinning Knife"),
	xorstr_("Baseball Bat"),
	xorstr_("Bone Club"),
	xorstr_("Butcher Knife"),
	xorstr_("Paddle"),
	xorstr_("Pitchfork"),
	xorstr_("Salvaged Cleaver"),
	xorstr_("Stone Spear"),
	xorstr_("Vampire Stake"),
	xorstr_("Wooden Spear"),
	xorstr_("Bone Knife"),
	xorstr_("Candy Cane Club"),
	xorstr_("Chinese new year spear"),
	xorstr_("Longsword"),
	xorstr_("Mace"),
	xorstr_("Machete"),
	xorstr_("Abyss Metal Pickaxe"),
	xorstr_("Abyss Metal Hatchet"),
	xorstr_("Abyss Torch"),
	xorstr_("Chainsaw"),
	xorstr_("Concrete Hatchet"),
	xorstr_("Concrete Pickaxe"),
	xorstr_("Rock"),
	xorstr_("Torch"),
	xorstr_("Stone Pickaxe"),
	xorstr_("Stone Hatchet"),
	xorstr_("Shovel"),
	xorstr_("Salvaged Icepick"),
	xorstr_("Salvaged Hammer"),
	xorstr_("Salvaged Axe"),
	xorstr_("Prototype Pickaxe"),
	xorstr_("Prototype Hatchet"),
	xorstr_("Pickaxe"),
	xorstr_("Cultist Deer Torch"),
	xorstr_("Hammer"),
	xorstr_("Garry's Mod Tool Gun"),
};
Vector3 RotationOffset;
std::shared_ptr<CheatFunction> BuildingRotationManager = std::make_shared<CheatFunction>(10, []()
	{
		if(!ctx.m_settings.Weapon_Misc_FreeRotation)
			return;
		if (BaseLocalPlayer.load() == nullptr)
			return;
		if (HeldItem.load() == nullptr)
			return;
		if (!(HeldItem.load()->GetClass() > 0x2000000 && HeldItem.load()->GetClass() < 0x7FFFFFFFFFFF))
			return;

		uint32_t itemid = HeldItem.load()->GetItemID();
		std::string weaponname;
		if (itemid == 0)
			return;

		if (LocalItemNames.contains(itemid))
		{
			weaponname = LocalItemNames[itemid];
		}

		if (weaponname != xorstr_("Building Plan"))
		{
			RotationOffset = Vector3::Zero();
			return;
		}

		if (Input.load()->IsKeyClicked(VK_LEFT))
		{
			RotationOffset.y += ctx.m_settings.Weapon_Misc_FreeRotation_Degrees;
		}
		if (Input.load()->IsKeyClicked(VK_RIGHT))
		{
			RotationOffset.y -= ctx.m_settings.Weapon_Misc_FreeRotation_Degrees;
		}
		if (Input.load()->IsKeyClicked(VK_UP))
		{
			RotationOffset.x += ctx.m_settings.Weapon_Misc_FreeRotation_Degrees;
		}
		if (Input.load()->IsKeyClicked(VK_DOWN))
		{
			RotationOffset.x -= ctx.m_settings.Weapon_Misc_FreeRotation_Degrees;
		}
	});
std::shared_ptr<BaseProjectile> CurrentWeapon = nullptr;
inline int LastWeaponID = 0;
inline std::string LastWeaponName;
inline bool DoNoRecoil = true;
void DrawRecoil()
{
	if (ctx.m_settings.Weapon_Misc_NoRecoilDisabler && ctx.m_settings.Weapon_Misc_NoRecoil)
	{
		Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		auto drawlist = ImGui::GetBackgroundDrawList();
		ImU32 colour = ImGui::GetColorU32(ImVec4(1, 0, 0, 1));
		if(DoNoRecoil)
		drawlist->AddText(ImVec2(centreofscreen.x, centreofscreen.y - 400), colour, xorstr_("No Recoil Enabled"));
		else
			drawlist->AddText(ImVec2(centreofscreen.x, centreofscreen.y - 400), colour, xorstr_("No Recoil Disabled"));
	}
}
std::shared_ptr<CheatFunction> WeaponBinds = std::make_shared<CheatFunction>(10, []()
	{
		if (!ctx.m_settings.Weapon_Misc_NoRecoilDisabler || !ctx.m_settings.Weapon_Misc_NoRecoil)
						return;
		if (BaseLocalPlayer.load() == nullptr)
			return;
		if (Input.load()->IsKeyClicked(ctx.m_settings.Weapon_Misc_NoRecoilDisablerKey))
		{
			DoNoRecoil = !DoNoRecoil;
		}

	}
	);
std::shared_ptr<CheatFunction> Weapons = std::make_shared<CheatFunction>(500, []()
{
	
	//	DECLARE_TIME(process);
	//	START_TIME(process);
	if (BaseLocalPlayer.load() == nullptr)
		return;

	HeldItem.store(BaseLocalPlayer.load()->GetActiveItem());

	if (HeldItem.load() == nullptr)
		return;
	if (!(HeldItem.load()->GetClass() > 0x2000000 && HeldItem.load()->GetClass() < 0x7FFFFFFFFFFF))
		return;
	uint32_t itemid = HeldItem.load()->GetItemID();
	std::string weaponname;
	if (itemid == 0)
		return;
	CurrentWeapon = HeldItem.load()->GetBaseProjectile();
	if (CurrentWeapon == nullptr)
		return;
	if(CurrentWeapon->GetClass() == 0)
		return;

	auto testhandle = mem.CreateScatterHandle();
	uint64_t readweapon;
	mem.AddScatterReadRequest(testhandle, CurrentWeapon->GetClass(), &readweapon, sizeof(uint64_t));
	mem.ExecuteReadScatter(testhandle);
	if (!(readweapon > 0x2000000 && readweapon < 0x7FFFFFFFFFFF))
		return;

	if (!LocalItemNames.contains(itemid) || LocalItemNames[itemid].size() == 0)
	{
		if (HeldItem.load()->GetName().size() == 0)
		{
			HeldItem.load()->UpdateDisplayName();
			HeldItem.load()->UpdateEnglish();
			HeldItem.load()->SetName();
			LocalItemNames[itemid] = HeldItem.load()->GetName();
		}
		else
			LocalItemNames[itemid] = HeldItem.load()->GetName();
	}
	if (LocalItemNames.contains(itemid))
	{
		weaponname = LocalItemNames[itemid];
	}
	else
	{
		weaponname = HeldItem.load()->GetName();
		LocalItemNames[itemid] = weaponname;
	}
	if(weaponname.size() ==0)
		return;
	if (CurrentWeapon->IsValidWeapon())
	{
		if (ctx.m_settings.Weapon_Misc_NoRecoil)
		{
			if ((ctx.m_settings.Weapon_Misc_NoRecoilDisabler && DoNoRecoil) || !ctx.m_settings.Weapon_Misc_NoRecoilDisabler)
			{
				CurrentWeapon->WriteRecoilPitch(MiscWriteScatterHandle, weaponname, ctx.m_settings.Weapon_Misc_RecoilAmounty);
				CurrentWeapon->WriteRecoilYaw(MiscWriteScatterHandle, weaponname, ctx.m_settings.Weapon_Misc_RecoilAmountx);
			
			}
			else if (ctx.m_settings.Weapon_Misc_NoRecoilDisabler && !DoNoRecoil)
			{
				CurrentWeapon->WriteRecoilPitch(MiscWriteScatterHandle, weaponname,100);
				CurrentWeapon->WriteRecoilYaw(MiscWriteScatterHandle, weaponname, 100);
			
			}
			MiscWriteScatterCount++;
		}
		if (ctx.m_settings.Weapon_Misc_NoSway)
		{
			MiscWriteScatterCount++;
			CurrentWeapon->WriteSway(MiscWriteScatterHandle, weaponname, ctx.m_settings.Weapon_Misc_SwayAmount);
		}
		if (ctx.m_settings.Weapon_Misc_NoAimCone)
		{
			MiscWriteScatterCount++;
			CurrentWeapon->WriteAimcone(MiscWriteScatterHandle, weaponname, ctx.m_settings.Weapon_Misc_AimConeAmount);
		}
		if (ctx.m_settings.Weapon_Misc_ForceAutomatic)
		{
			MiscWriteScatterCount++;
			CurrentWeapon->WriteAutomatic(MiscWriteScatterHandle, weaponname, true);
		}
		// printf  LocalItemNames[itemid]
		//	printf(xorstr_("Weapon: %s\n"), LocalItemNames[itemid].c_str());
	}
	if (ctx.m_settings.Weapon_Misc_InstantEoka)
	{
		HeldItem.load()->SetEokaStrikes(MiscWriteScatterHandle, weaponname);
		MiscWriteScatterCount++;
	}
	if (ctx.m_settings.Weapon_Misc_InstantCompound)
	{
		HeldItem.load()->SetCompoundBow(MiscWriteScatterHandle, weaponname);
		MiscWriteScatterCount++;
	}
	if (ctx.m_settings.Weapon_Misc_InstantHammer)
	{
		HeldItem.load()->FastHammer(MiscWriteScatterHandle, weaponname);
		MiscWriteScatterCount++;
	}
	if (ctx.m_settings.Weapon_Misc_FreeRotation)
	{
		HeldItem.load()->SetRotation(MiscWriteScatterHandle, weaponname, RotationOffset);
		MiscWriteScatterCount++;
	}
	if (std::count(Tools.begin(), Tools.end(), weaponname) != 0) // is tool
	{
		if (ctx.m_settings.Weapon_Misc_AttackDistance)
		{
			HeldItem.load()->SetAttackDistance(MiscWriteScatterHandle);
			MiscWriteScatterCount++;
		}
		if (ctx.m_settings.Weapon_Misc_AttackableSprinting)
		{
			HeldItem.load()->SetAttackableSprinting(MiscWriteScatterHandle);
			MiscWriteScatterCount++;
		}
	}
	//STOP_TIME(process);
	//	std::cout << "Elapsed time in milliseconds: " << GET_TIME_MS(process) << std::endl;
});

void WeaponFunctions()
{
	Weapons->Execute();
	BuildingRotationManager->Execute();
	WeaponBinds->Execute();
}

int GetWeaponHeirarchy(WeaponID id)
{

	if (id == WeaponID::Bow)
		return 0;
	if(id == WeaponID::Eoka)
		return 1;
	if (id == WeaponID::Crossbow)
		return 2;
	if (id == WeaponID::CompoundBow)
		return 3;
	if (id == WeaponID::NailGun)
		return 4;
	if(id == WeaponID::Waterpipe)
		return 5;
	if (id == WeaponID::DoubleBarrel)
		return 6;
	if(id == WeaponID::Revolver)
		return 7;
	if(id == WeaponID::PumpShotgun)
		return 8;
	if (id == WeaponID::Spas12)
		return 9;
	if (id == WeaponID::SemiPistol)
		return 10;
	if (id == WeaponID::CustomSMG)
		return 11;
	if (id == WeaponID::M4)
		return 12;
	if (id == WeaponID::M92)
		return 13;
	if (id == WeaponID::Prototype)
		return 14;
	if (id == WeaponID::SemiRifle)
		return 15;
	if (id == WeaponID::Thompson)
		return 16;
	if (id == WeaponID::MP5)
		return 17;
	if (id == WeaponID::LR300)
		return 18;
	if (id == WeaponID::M39)
		return 19;
	if (id == WeaponID::Bolt)
		return 20;
	if (id == WeaponID::L96)
		return 21;
	if (id == WeaponID::HMLMG)
		return 22;
	if (id == WeaponID::AK)
		return 23;
	if (id == WeaponID::M249)
		return 24;



	return -1;
}