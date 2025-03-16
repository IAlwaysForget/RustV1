#include "Pch.h"
#include "SkinChanger.h"

#include "context.h"
#include "MiscFeatures.h"
#include "Globals.h"
#include "MiscWeapons.h"
#include "Misc/CheatFunction.h"

class WeaponSkins
{
	std::unordered_map<int, ULONG> SkinIDs;

public:
	WeaponSkins(std::unordered_map<int, ULONG> skins)
	{
		SkinIDs = skins;
	}

	ULONG GetID(int id)
	{
		if (id == 0)
			return 0;
		if (SkinIDs.contains(id))
			return SkinIDs[id];
		return 0;
	}
};

WeaponID GetWeaponID(const std::string& name)
{
	if (!WeaponIDS.contains(name))
		return WeaponID::None;
	return WeaponIDS[name];
}

// alien red, victoria, Glory, anubis, cold hunter, from hell, tempered, Phantom, no mercy
std::unordered_map<int, ULONG> AkSkins = {{0, 0}, {1, 859845460}, {2, 1372945520}, {3, 889710179}, {4, 1259716979}, {5, 1583542371}, {6, 1202410378}, {7, 10138}, {8, 1522034435}, {9, 1760078043}};
// Daemon strike, Polymer, Fury Ex Red, Cold Hunter, Survivor, Deathcore, Holographic, Jungle Fighter, Phantom, no mercy, tempered, redemption
std::unordered_map<int, ULONG> LRSkins = {{0, 0}, {1, 1174458060}, {2, 1308037543}, {3, 1419392688}, {4, 1553359638}, {5, 1176479716}, {6, 1225499752}, {7, 1700177871}, {8, 1660691287}, {9, 1671985039}, {10, 1953108368}, {11, 1481478360}, {12, 2888800551}};
// bombshell, tempered, rainbow pony, the beast, black gold, redemption, cold hunter
std::unordered_map<int, ULONG> M2Skins = {{0, 0}, {1, 1719536313}, {2, 2723336741}, {3, 2655521139}, {4, 1712378771}, {5, 2814779587}, {6, 2979503014}, {7, 2891224181}};
// metalhead, desert warrior, Ornate, Devourer, Tempered
std::unordered_map<int, ULONG> M39Skins = {{0, 0}, {1, 1736532811}, {2, 1708343082}, {3, 1720530850}, {4, 1708365495}, {5, 2446970844}};
// Blackout, GhostFlame, Rainbow pony, evil snowman, no mercy, Timmy
std::unordered_map<int, ULONG> L96Skins = {{0, 0}, {1, 2473291137}, {2, 2628950838}, {3, 2760357876}, {4, 2678415938}, {5, 2447784160}, {6, 2533821331}};
// tempered, military camo, polymer, digital camo, charitable rust, cold hunter,batteries not included, phantom, no mercy
std::unordered_map<int, ULONG> MP5Skins = {{0, 0}, {1, 800974015}, {2, 796687275}, {3, 808554348}, {4, 3043259011}, {5, 1693898215}, {6, 1603970802}, {7, 1673754411}, {8, 1865099372}};
// Glory, Imp, Comics, Gold Phoenix, Urban light, sublime, tempered, cold hunter, no mercy, redemption
std::unordered_map<int, ULONG> SemiRifleSkins = {{0, 0}, {1, 959955205}, {2, 1863834018}, {3, 1448088345}, {4, 1279791283}, {5, 875259741}, {6, 2637366545}, {7, 1616628843}, {8, 1788152335}, {9, 2985524644}};
// Demonic pistol, halloween bat, direct threat, polymer, golden leaf, cold forged, chrome, tempered, golden dragon
std::unordered_map<int, ULONG> SemiPistolSkins = {{0, 0}, {1, 1158943049}, {2, 539539196}, {3, 910681058}, {4, 798375117}, {5, 805925675}, {6, 1428766159}, {7, 876007573}, {8, 1553005167}, {9, 1121906926}};
// glory, trust in rust, Santa's little helper, blackout, black gold, cold hunter, no mercy, azul, doodle
std::unordered_map<int, ULONG> ThompsonSkins = {{0, 0}, {1, 1689944021}, {2, 3137160380}, {3, 561462394}, {4, 2393671891}, {5, 2786022713}, {6, 2490318017}, {7, 2496760647}, {8, 1772377862}, {9, 2393708019}};
// Dragon glass, aztec, ancient gods, vampire, ruby, bloody tooth, silver leaf
std::unordered_map<int, ULONG> StonePickSkins = {{0, 0}, {1, 1428014003}, {2, 127557296}, {3, 1100085862}, {4, 1852292345}, {5, 2138887265}, {6, 840380436}, {7, 1988077270},};
// Dragon glass, aztec jade, ice tooth, volcanic, ruby,geode,silver leaf,vampire
std::unordered_map<int, ULONG> StoneHatchetSkins = {{0, 0}, {1, 1428010053}, {2, 1117098080}, {3, 797445687}, {4, 878257758}, {5, 2138883683}, {6, 1058431260}, {7, 1988082179}, {8, 1852290423}};
// WarHawk, Beans Cannon, Army Surplus, Dragon Launcher, Glory
std::unordered_map<int, ULONG> RocketLauncherSkins = {{0, 0}, {1, 812737524}, {2, 894679426}, {3, 813795591}, {4, 1926503780}, {5, 1657103887}};
// Gold Rock, Shard of true ice, frozen turkey, fresh brain, big burger, mother's day rock, fish rock, pug rock, hamster rock, capynara rock
std::unordered_map<int, ULONG> RockSkins = {{0, 0}, {1, 804791902}, {2, 824904841}, {3, 858285362}, {4, 864277659}, {5, 1636101879}, {6, 1739062735}, {7, 2422851732}, {8, 3034565372}, {9, 2979393128}, {10, 2976241877}};
// Morocco, blackout, cold hunter, pearl, tempered, batteries not included
std::unordered_map<int, ULONG> PythonSkins = {{0, 0}, {1, 1418647979}, {2, 2894360042}, {3, 1305704747}, {4, 1305704747}, {5, 1529514494}, {6, 1631920728}};
// Raventale,damascus,rainbwo pony,Ancient amber, shinobi, montezuma, gold digger, inflatable
std::unordered_map<int, ULONG> PickaxeSkins = {{0, 0}, {1, 1428399027}, {2, 1143157014}, {3, 2171468844}, {4, 2637158928}, {5, 1298797381}, {6, 1444912645}, {7, 3145252844}};
// Raider hatchet, red chopper, firefighter, raventale, damascus, shinobi, amber, inflatable
std::unordered_map<int, ULONG> HatchetSkins = {{0, 0}, {1, 1108554409}, {2, 910196839}, {3, 814630559}, {4, 1422179740}, {5, 1143274085}, {6, 1291775183}, {7, 2637157054}, {8, 3145249854}};
// comics jackhammer, jackhammer from hell, toy jackhammer, ghost catcher jackhammer, azul, carrot jackhammer
std::unordered_map<int, ULONG> JackHammerSkins = {{0, 0}, {1, 2805788816}, {2, 2949907518}, {3, 2375721158}, {4, 2621167084}, {5, 1143274085}, {6, 2455001792}, {7, 2957302997},};
// combat knife from hell, emerald knife, dread lord knife, thug knife , glory knife, carbon elite
std::unordered_map<int, ULONG> CombatKnifeSkins = {{0, 0}, {1, 1719795241}, {2, 1738307827}, {3, 1910941833}, {4, 1715608877}, {5, 1702783691}, {6, 1706788762}};
// Raven, dark arts, stinger, lavapick,inferno hazard
std::unordered_map<int, ULONG> SalvagedIcepickSkins = {{0, 0}, {1, 933760454}, {2, 911446362}, {3, 1276627079}, {4, 844666224}, {5, 820810719},};
// banana eoka, flintlock, last blast, hotdog
std::unordered_map<int, ULONG> EokaSkins = {{0, 0}, {1, 856053892}, {2, 1171935313}, {3, 1413731110}, {4, 2375110897},};
// Gold spirit, ronin, victoria,cold hunter, tempered, chrome, from hell
std::unordered_map<int, ULONG> CrossBowSkins = {{0, 0}, {1, 1349337546}, {2, 1328219905}, {3, 1383105648}, {4, 1772979668}, {5, 2570034151}, {6, 874268567}, {7, 2826600154}};
// Blackout, gold leaf, from hell, tempered, abyss
std::unordered_map<int, ULONG> BowSkins = {{0, 0}, {1, 2192571819}, {2, 1846220912}, {3, 1793915047}, {4, 2475197041}, {5, 2108510511}};
// Alien relic, polymer, looters, dark flame, tempered, phantom, toy
std::unordered_map<int, ULONG> CustomSMGSkins = {{0, 0}, {1, 820350952}, {2, 1198145190}, {3, 904964438}, {4, 1128840196}, {5, 1523699528}, {6, 1685722307}, {7, 2281845451}};
// Bandito, phantom, hot tempered, frostbitten, eightball,outlaw
std::unordered_map<int, ULONG> RevolverSkins = {{0, 0}, {1, 910665268}, {2, 1235996786}, {3, 809822151}, {4, 873242795}, {5, 815532676}, {6, 809897611}};
// Army bolt, polymer,glory, survivor,from hell,no mercy,glory,tundra,Redemption,tempered,cold hunter
std::unordered_map<int, ULONG> BoltSkins = {{0, 0}, {1, 818403150}, {2, 840105253}, {3, 1161165984}, {4, 1517933342}, {5, 2024514125}, {6, 933509449}, {7, 518199212}, {8, 3002533749}, {9, 2412215351}, {10, 2510556864}};
std::unordered_map<WeaponID, std::shared_ptr<WeaponSkins>> SkinMap = {
	{WeaponID::AK, std::make_shared<WeaponSkins>(AkSkins)},
	{WeaponID::LR300, std::make_shared<WeaponSkins>(LRSkins)},
	{WeaponID::M249, std::make_shared<WeaponSkins>(M2Skins)},
	{WeaponID::MP5, std::make_shared<WeaponSkins>(MP5Skins)},
	{WeaponID::CustomSMG, std::make_shared<WeaponSkins>(CustomSMGSkins)},
	{WeaponID::Thompson, std::make_shared<WeaponSkins>(ThompsonSkins)},
	{WeaponID::SemiRifle, std::make_shared<WeaponSkins>(SemiRifleSkins)},
	{WeaponID::SemiPistol, std::make_shared<WeaponSkins>(SemiPistolSkins)},
	{WeaponID::Python, std::make_shared<WeaponSkins>(PythonSkins)},
	{WeaponID::M39, std::make_shared<WeaponSkins>(M39Skins)},
	{WeaponID::L96, std::make_shared<WeaponSkins>(L96Skins)},
	{WeaponID::Bow, std::make_shared<WeaponSkins>(BowSkins)},
	{WeaponID::Crossbow, std::make_shared<WeaponSkins>(CrossBowSkins)},
	{WeaponID::Eoka, std::make_shared<WeaponSkins>(EokaSkins)},
	{WeaponID::JackHammer, std::make_shared<WeaponSkins>(JackHammerSkins)},
	{WeaponID::SalvagedIcepick, std::make_shared<WeaponSkins>(SalvagedIcepickSkins)},
	{WeaponID::Pickaxe, std::make_shared<WeaponSkins>(PickaxeSkins)},
	{WeaponID::Hatchet, std::make_shared<WeaponSkins>(HatchetSkins)},
	{WeaponID::StonePickaxe, std::make_shared<WeaponSkins>(StonePickSkins)},
	{WeaponID::StoneHatchet, std::make_shared<WeaponSkins>(StoneHatchetSkins)},
	{WeaponID::CombatKnife, std::make_shared<WeaponSkins>(CombatKnifeSkins)},
	{WeaponID::Rock, std::make_shared<WeaponSkins>(RockSkins)},
	{WeaponID::RocketLauncher, std::make_shared<WeaponSkins>(RocketLauncherSkins)},
	{WeaponID::Revolver, std::make_shared<WeaponSkins>(RevolverSkins)},
	{WeaponID::Revolver, std::make_shared<WeaponSkins>(BoltSkins)},

};

ULONG GetSkinID(WeaponID weapon, int id)
{
	if (weapon == WeaponID::None)
		return 0;
	if (!SkinMap.contains(weapon))
		return 0;
	return SkinMap[weapon]->GetID(id);
}

std::unordered_map<WeaponID, int> ConfigMap = {
	{WeaponID::AK, ctx.m_settings.Skins_AkSkin},
	{WeaponID::LR300, ctx.m_settings.Skins_LR300Skin},
	{WeaponID::M249, ctx.m_settings.Skins_M249Skin},
	{WeaponID::MP5, ctx.m_settings.Skins_MP5Skin},
	{WeaponID::CustomSMG, ctx.m_settings.Skins_CustomSMGSkin},
	{WeaponID::Thompson, ctx.m_settings.Skins_ThompsonSkin},
	{WeaponID::SemiRifle, ctx.m_settings.Skins_SemiAutoRifleSkin},
	{WeaponID::SemiPistol, ctx.m_settings.Skins_SemiAutoPistolSkin},
	{WeaponID::Python, ctx.m_settings.Skins_PythonRevolverSkin},
	{WeaponID::M39, ctx.m_settings.Skins_M39Skin},
	{WeaponID::L96, ctx.m_settings.Skins_L96Skin},
	{WeaponID::Bow, ctx.m_settings.Skins_BowSkin},
	{WeaponID::Crossbow, ctx.m_settings.Skins_CrossbowSkin},
	{WeaponID::Eoka, ctx.m_settings.Skins_EokaSkin},
	{WeaponID::JackHammer, ctx.m_settings.Skins_JackHammerSkin},
	{WeaponID::SalvagedIcepick, ctx.m_settings.Skins_SalvagedIcepickSkin},
	{WeaponID::Pickaxe, ctx.m_settings.Skins_PickaxeSkin},
	{WeaponID::Hatchet, ctx.m_settings.Skins_HatchetSkin},
	{WeaponID::StonePickaxe, ctx.m_settings.Skins_StonePickaxeSkin},
	{WeaponID::StoneHatchet, ctx.m_settings.Skins_StoneHatchetSkin},
	{WeaponID::CombatKnife, ctx.m_settings.Skins_CombatKnifeSkin},
	{WeaponID::Rock, ctx.m_settings.Skins_RockSkin},
	{WeaponID::RocketLauncher, ctx.m_settings.Skins_RocketLauncherSkin},
	{WeaponID::Revolver, ctx.m_settings.Skins_RevolverSkin},
	{WeaponID::Bolt, ctx.m_settings.Skins_BoltSkin},

};
// even passing pointer into the map doesn't let it update so we are just going to remake the map
void UpdateSkinConfig()
{
	ConfigMap = {
		{WeaponID::AK, ctx.m_settings.Skins_AkSkin},
		{WeaponID::LR300, ctx.m_settings.Skins_LR300Skin},
		{WeaponID::M249, ctx.m_settings.Skins_M249Skin},
		{WeaponID::MP5, ctx.m_settings.Skins_MP5Skin},
		{WeaponID::CustomSMG, ctx.m_settings.Skins_CustomSMGSkin},
		{WeaponID::Thompson, ctx.m_settings.Skins_ThompsonSkin},
		{WeaponID::SemiRifle, ctx.m_settings.Skins_SemiAutoRifleSkin},
		{WeaponID::SemiPistol, ctx.m_settings.Skins_SemiAutoPistolSkin},
		{WeaponID::Python, ctx.m_settings.Skins_PythonRevolverSkin},
		{WeaponID::M39, ctx.m_settings.Skins_M39Skin},
		{WeaponID::L96, ctx.m_settings.Skins_L96Skin},
		{WeaponID::Bow, ctx.m_settings.Skins_BowSkin},
		{WeaponID::Crossbow, ctx.m_settings.Skins_CrossbowSkin},
		{WeaponID::Eoka, ctx.m_settings.Skins_EokaSkin},
		{WeaponID::JackHammer, ctx.m_settings.Skins_JackHammerSkin},
		{WeaponID::SalvagedIcepick, ctx.m_settings.Skins_SalvagedIcepickSkin},
		{WeaponID::Pickaxe, ctx.m_settings.Skins_PickaxeSkin},
		{WeaponID::Hatchet, ctx.m_settings.Skins_HatchetSkin},
		{WeaponID::StonePickaxe, ctx.m_settings.Skins_StonePickaxeSkin},
		{WeaponID::StoneHatchet, ctx.m_settings.Skins_StoneHatchetSkin},
		{WeaponID::CombatKnife, ctx.m_settings.Skins_CombatKnifeSkin},
		{WeaponID::Rock, ctx.m_settings.Skins_RockSkin},
		{WeaponID::RocketLauncher, ctx.m_settings.Skins_RocketLauncherSkin},
		{WeaponID::Revolver, ctx.m_settings.Skins_RevolverSkin},
		{WeaponID::Bolt, ctx.m_settings.Skins_BoltSkin},

	};
}

int LastItemID = 0;
std::shared_ptr<CheatFunction> SkinChanger = std::make_shared<CheatFunction>(40, []()
{
	if (!ctx.m_settings.Skins_Enable)
		return;
	if (BaseLocalPlayer.load() == nullptr)
		return;
	HeldItem.store(BaseLocalPlayer.load()->GetActiveItem());
	if (HeldItem.load() != nullptr)
	{
		uint32_t itemid = HeldItem.load()->GetItemID();

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

		WeaponID weapon = GetWeaponID(LocalItemNames[itemid]);

		if (weapon != WeaponID::None)
		{
			ULONG skinid = GetSkinID(weapon, ConfigMap[weapon]);
			if (skinid != 0)
			{
				HeldItem.load()->SetSkin(MiscWriteScatterHandle, skinid);
				MiscWriteScatterCount++;
			}
		}
	}
});

void SkinChangerFunction()
{
	SkinChanger->Execute();
}
