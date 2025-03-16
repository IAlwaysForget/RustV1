#include "Misc/kmboxNet.h"
#include "Draw.h"
#include "Font/newIcons.h"
#include "Font/textFont.h"
#include "context.h"
#include "Misc/kmbox.h"
#include "Cheat/Globals.h"
#include "Cheat/SDK/ConvarGraphics.h"
#include "Cheat/SDK/ConvarAdmin.h"
#include "Cheat/SDK/OcclusionCulling.h"
#include "cheat/Features/Misc Features/SkinChanger.h"
#include "UI/Plugins/map.h"
//"Player", "Friend", "Scientist", "Bear", "Polar Bear", "Boar", "Stag", "Wolf", "Horse", "Chicken",  "Attack Helicopter", "Bradley", "MiniCopter", "Attack MiniCopter", "Scrap Helicopter", "Train", "Car", "TugBoat", "RowBoat", "RHIB"
Players GetPlayerEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.Player;
	case 1:
		return ctx.m_settings.Friend;
	case 2:
		return ctx.m_settings.Scientist;
	default:
		break;
	}
	return Players();
}

RadarPlayers GetRadarPlayerEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.PlayerRadar;
	case 1:
		return ctx.m_settings.FriendRadar;
	case 2:
		return ctx.m_settings.ScientistRadar;
	default:
		break;
	}
	return RadarPlayers();
}

Animals GetAnimalEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.Bear;
	case 1:
		return ctx.m_settings.PolarBear;
	case 2:
		return ctx.m_settings.Boar;
	case 3:
		return ctx.m_settings.Stag;
	case 4:
		return ctx.m_settings.Wolf;
	case 5:
		return ctx.m_settings.Horse;
	case 6:
		return ctx.m_settings.Chicken;
	case 7:
		return ctx.m_settings.Shark;
	default:
		break;
	}
	return Animals();
}

RadarAnimals GetRadarAnimalEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarBear;
	case 1:
		return ctx.m_settings.RadarPolarBear;
	case 2:
		return ctx.m_settings.RadarBoar;
	case 3:
		return ctx.m_settings.RadarStag;
	case 4:
		return ctx.m_settings.RadarWolf;
	case 5:
		return ctx.m_settings.RadarHorse;
	case 6:
		return ctx.m_settings.RadarChicken;
	case 7:
		return ctx.m_settings.RadarShark;
	default:
		break;
	}
	return RadarAnimals();
}

Vehicles GetVehicleEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.PatrolHelicopter;
	case 1:
		return ctx.m_settings.Bradley;
	case 2:
		return ctx.m_settings.MiniCopter;
	case 3:
		return ctx.m_settings.AttackMiniCopter;
	case 4:
		return ctx.m_settings.ScrapHeli;
	case 5:
		return ctx.m_settings.Train;
	case 6:
		return ctx.m_settings.Car;
	case 7:
		return ctx.m_settings.TugBoat;
	case 8:
		return ctx.m_settings.RowBoat;
	case 9:
		return ctx.m_settings.RHIB;
	case 10:
		return ctx.m_settings.Pedalbike;
	case 11:
		return ctx.m_settings.Motorbike;
		case 12:
			return ctx.m_settings.MotorbikeSidecar;
	default:
		break;
	}
	return Vehicles();
}

RadarVehicles GetRadarVehicleEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarPatrolHelicopter;
	case 1:
		return ctx.m_settings.RadarBradley;
	case 2:
		return ctx.m_settings.RadarMiniCopter;
	case 3:
		return ctx.m_settings.RadarAttackMiniCopter;
	case 4:
		return ctx.m_settings.RadarScrapHeli;
	case 5:
		return ctx.m_settings.RadarTrain;
	case 6:
		return ctx.m_settings.RadarCar;
	case 7:
		return ctx.m_settings.RadarTugBoat;
	case 8:
		return ctx.m_settings.RadarRowBoat;
	case 9:
		return ctx.m_settings.RadarRHIB;
	case 10:
		return ctx.m_settings.RadarPedalbike;
	case 11:
		return ctx.m_settings.RadarMotorbike;
	case 12:
		return ctx.m_settings.RadarMotorbikeSidecar;
	default:
		break;
	}
	return RadarVehicles();
}

Ores GetOresEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.Stone;
	case 1:
		return ctx.m_settings.Metal;
	case 2:
		return ctx.m_settings.Sulphur;
	default:
		break;
	}
	return Ores();
}

RadarOres GetRadarOresEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarStone;
	case 1:
		return ctx.m_settings.RadarMetal;
	case 2:
		return ctx.m_settings.RadarSulphur;
	default:
		break;
	}
	return RadarOres();
}

Collectables GetCollectablesEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.Wood;
	case 1:
		return ctx.m_settings.Hemp;
	case 2:
		return ctx.m_settings.Pumpkin;
	case 3:
		return ctx.m_settings.Corn;
	case 4:
		return ctx.m_settings.Mushroom;
	case 5:
		return ctx.m_settings.Potato;
	case 6:
		return ctx.m_settings.RedBerry;
	case 7:
		return ctx.m_settings.WhiteBerry;
	case 8:
		return ctx.m_settings.BlackBerry;
	case 9:
		return ctx.m_settings.YellowBerry;
	case 10:
		return ctx.m_settings.BlueBerry;
	case 11:
		return ctx.m_settings.GreenBerry;
	case 12:
		return ctx.m_settings.StoneCollectable;
	case 13:
		return ctx.m_settings.MetalCollectable;
	case 14:
		return ctx.m_settings.SulphurCollectable;
	case 15:
		return ctx.m_settings.Diesel;
	case 16:
		return ctx.m_settings.MetalDetector;
	default:
		break;
	}
	return Collectables();
}

RadarCollectables GetRadarCollectablesEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarWood;
	case 1:
		return ctx.m_settings.RadarHemp;
	case 2:
		return ctx.m_settings.RadarPumpkin;
	case 3:
		return ctx.m_settings.RadarCorn;
	case 4:
		return ctx.m_settings.RadarMushroom;
	case 5:
		return ctx.m_settings.RadarPotato;
	case 6:
		return ctx.m_settings.RadarRedBerry;
	case 7:
		return ctx.m_settings.RadarWhiteBerry;
	case 8:
		return ctx.m_settings.RadarBlackBerry;
	case 9:
		return ctx.m_settings.RadarYellowBerry;
	case 10:
		return ctx.m_settings.RadarBlueBerry;
	case 11:
		return ctx.m_settings.RadarGreenBerry;
	case 12:
		return ctx.m_settings.RadarStoneCollectable;
	case 13:
		return ctx.m_settings.RadarMetalCollectable;
	case 14:
		return ctx.m_settings.RadarSulphurCollectable;
	case 15:
		return ctx.m_settings.RadarDiesel;
	case 16:
		return ctx.m_settings.RadarMetalDetector;
	default:
		break;
	}
	return RadarCollectables();
}

DroppedItems GetDroppedItemsEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.OtherItems;
	case 1:
		return ctx.m_settings.GunItems;
	case 2:
		return ctx.m_settings.ToolItems;
	case 3:
		return ctx.m_settings.ArmourItems;
	case 4:
		return ctx.m_settings.MedicalItems;
	case 5:
		return ctx.m_settings.ResourceItems;
	case 6:
		return ctx.m_settings.FoodItems;
	case 7:
		return ctx.m_settings.ExplosiveItems;
	case 8:
		return ctx.m_settings.AmmoItems;
	case 9:
		return ctx.m_settings.ComponentsItems;
	default:
		break;
	}
	return DroppedItems();
}

RadarDroppedItems GetRadarDroppedItemsEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarOtherItems;
	case 1:
		return ctx.m_settings.RadarGunItems;
	case 2:
		return ctx.m_settings.RadarToolItems;
	case 3:
		return ctx.m_settings.RadarArmourItems;
	case 4:
		return ctx.m_settings.RadarMedicalItems;
	case 5:
		return ctx.m_settings.RadarResourceItems;
	case 6:
		return ctx.m_settings.RadarFoodItems;
	case 7:
		return ctx.m_settings.RadarExplosiveItems;
	case 8:
		return ctx.m_settings.RadarAmmoItems;
	case 9:
		return ctx.m_settings.RadarComponentsItems;
	default:
		break;
	}
	return RadarDroppedItems();
}

Lootables GetLootablesEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.Barrel;
	case 1:
		return ctx.m_settings.FuelBarrel;
	case 2:
		return ctx.m_settings.Corpse;
	case 3:
		return ctx.m_settings.LockedCrate;
	case 4:
		return ctx.m_settings.SupplyDrop;
	case 5:
		return ctx.m_settings.EliteCrate;
	case 6:
		return ctx.m_settings.NormalCrate;
	case 7:
		return ctx.m_settings.MilitaryCrate;
	case 8:
		return ctx.m_settings.MedicalCrate;
	case 9:
		return ctx.m_settings.FoodCrate;
	case 10:
		return ctx.m_settings.MineCart;
	case 11:
		return ctx.m_settings.ToolBox;
	case 12:
		return ctx.m_settings.AmmoCrate;
	case 13:
		return ctx.m_settings.FuelCrate;
	case 14:
		return ctx.m_settings.UnderWaterCrate;
	case 15:
		return ctx.m_settings.HelicopterCrate;
	case 16:
		return ctx.m_settings.BradleyCrate;
	case 17:
		return ctx.m_settings.TechCrate;
	default:
		break;
	}
	return Lootables();
}

RadarLootables GetRadarLootablesEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarBarrel;
	case 1:
		return ctx.m_settings.RadarFuelBarrel;
	case 2:
		return ctx.m_settings.RadarCorpse;
	case 3:
		return ctx.m_settings.RadarLockedCrate;
	case 4:
		return ctx.m_settings.RadarSupplyDrop;
	case 5:
		return ctx.m_settings.RadarEliteCrate;
	case 6:
		return ctx.m_settings.RadarNormalCrate;
	case 7:
		return ctx.m_settings.RadarMilitaryCrate;
	case 8:
		return ctx.m_settings.RadarMedicalCrate;
	case 9:
		return ctx.m_settings.RadarFoodCrate;
	case 10:
		return ctx.m_settings.RadarMineCart;
	case 11:
		return ctx.m_settings.RadarToolBox;
	case 12:
		return ctx.m_settings.RadarAmmoCrate;
	case 13:
		return ctx.m_settings.RadarFuelCrate;
	case 14:
		return ctx.m_settings.RadarUnderWaterCrate;
	case 15:
		return ctx.m_settings.RadarHelicopterCrate;
	case 16:
		return ctx.m_settings.RadarBradleyCrate;
	case 17:
		return ctx.m_settings.RadarTechCrate;
	default:
		break;
	}
	return RadarLootables();
}

Deployables GetDeployablesEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.Stash;
	case 1:
		return ctx.m_settings.SleepingBag;
	case 2:
		return ctx.m_settings.LandMine;
	case 3:
		return ctx.m_settings.BearTrap;
	case 4:
		return ctx.m_settings.ShotgunTrap;
	case 5:
		return ctx.m_settings.AutoTurret;
	case 6:
		return ctx.m_settings.FlameTurret;
	case 7:
		return ctx.m_settings.SamSite;
	case 8:
		return ctx.m_settings.ToolCupboard;
	case 9:
		return ctx.m_settings.Planter;
	case 10:
		return ctx.m_settings.LargeBox;
	case 11:
		return ctx.m_settings.Workbench;
	case 12:
		return ctx.m_settings.Furnace;
	case 13:
		return ctx.m_settings.OilRefinery;
	case 14:
		return ctx.m_settings.Recycler;
	case 15:
		return ctx.m_settings.Battery;
	default:
		break;
	}
	return Deployables();
}

RadarDeployables GetRadarDeployablesEntityConfig(int entity_index)
{
	switch (entity_index)
	{
	case 0:
		return ctx.m_settings.RadarStash;
	case 1:
		return ctx.m_settings.RadarSleepingBag;
	case 2:
		return ctx.m_settings.RadarLandMine;
	case 3:
		return ctx.m_settings.RadarBearTrap;
	case 4:
		return ctx.m_settings.RadarShotgunTrap;
	case 5:
		return ctx.m_settings.RadarAutoTurret;
	case 6:
		return ctx.m_settings.RadarFlameTurret;
	case 7:
		return ctx.m_settings.RadarSamSite;
	case 8:
		return ctx.m_settings.RadarToolCupboard;
	case 9:
		return ctx.m_settings.RadarPlanter;
	case 10:
		return ctx.m_settings.RadarLargeBox;
	case 11:
		return ctx.m_settings.RadarWorkbench;
	case 12:
		return ctx.m_settings.RadarFurnace;
	case 13:
		return ctx.m_settings.RadarOilRefinery;
	case 14:
		return ctx.m_settings.RadarRecycler;
	case 15:
		return ctx.m_settings.RadarBattery;
	default:
		break;
	}
	return RadarDeployables();
}

std::vector<std::string> akSkins = {xorstr_("None"),xorstr_("Alien Red"),xorstr_("Victoria"),xorstr_("Glory"),xorstr_("Anubis"),xorstr_("Cold Hunter"),xorstr_("From Hell"),xorstr_("Tempered"),xorstr_("Phantom"),xorstr_("No Mercy")};
std::vector<std::string> lrSkins = {xorstr_("None"),xorstr_("Daemon strike"), xorstr_("Polymer"), xorstr_("Fury Ex Red"), xorstr_("Cold Hunter"), xorstr_("Survivor"), xorstr_("Deathcore"), xorstr_("Holographic"), xorstr_("Jungle Fighter"), xorstr_("Phantom"), xorstr_("No Mercy"), xorstr_("Tempered"), xorstr_("Redemption")};
std::vector<std::string> m2Skins = {xorstr_("None"),xorstr_("Bombshell"), xorstr_("Tempered"), xorstr_("Rainbow Pony"), xorstr_("The Beast"), xorstr_("Black Gold"), xorstr_("Redemption"), xorstr_("Cold Hunter")};
std::vector<std::string> m39Skins = {xorstr_("None"),xorstr_("Metalhead"), xorstr_("Desert Warrior"), xorstr_("Ornate"), xorstr_("Devourer"), xorstr_("Tempered")};
std::vector<std::string> l96Skins = {xorstr_("None"),xorstr_("Blackout"), xorstr_("Ghostflame"), xorstr_("Rainbow Pony"), xorstr_("Evil Snowman"), xorstr_("No Mercy"), xorstr_("Timmy")};
std::vector<std::string> mp5Skins = {xorstr_("None"),xorstr_("Tempered"), xorstr_("Military Camo"), xorstr_("Polymer"), xorstr_("Digital Camo"), xorstr_("Charitable Rust"), xorstr_("Cold Hunter"), xorstr_("Toy"), xorstr_("Phantom"), xorstr_("No Mercy")};
std::vector<std::string> semiRifleSkins = {xorstr_("None"), xorstr_("Glory"), xorstr_("Imp"), xorstr_("Comics"), xorstr_("Gold Phoenix"), xorstr_("Urban Light"), xorstr_("Sublime"), xorstr_("Tempered"), xorstr_("Cold Hunter"), xorstr_("No Mercy"), xorstr_("Redemption")};
std::vector<std::string> semiPistolSkins = {xorstr_("None"),xorstr_("Demonic Pistol"), xorstr_("Halloween Bat"), xorstr_("Direct Threat"), xorstr_("Polymer"), xorstr_("Golden Leaf"), xorstr_("Cold Forged"), xorstr_("Chrome"), xorstr_("Tempered"), xorstr_("Golden Dragon")};
std::vector<std::string> thompsonSkins = {xorstr_("None"),xorstr_("Morocco"), xorstr_("Blackout"), xorstr_("Cold Hunter"), xorstr_("Pearl"), xorstr_("Tempered"), xorstr_("Toy")};
std::vector<std::string> stonePickSkins = {xorstr_("None"),xorstr_("Dragon Glass"), xorstr_("Aztec"), xorstr_("Ancient Gods"), xorstr_("Vampire"), xorstr_("Ruby"), xorstr_("Bloody Tooth"), xorstr_("Silver Leaf")};
std::vector<std::string> stoneHatchetSkins = {xorstr_("None"),xorstr_("Dragon Glass"), xorstr_("Aztec Jade"), xorstr_("Ice Tooth"), xorstr_("Volcanic"), xorstr_("Ruby"), xorstr_("Geode"), xorstr_("Silver Leaf"), xorstr_("Vampire")};
std::vector<std::string> rocketLauncherSkins = {xorstr_("None"),xorstr_("Warhawk"), xorstr_("Beans Cannon"), xorstr_("Army Surplus"), xorstr_("Dragon Launcher"), xorstr_("Glory")};
std::vector<std::string> rockSkins = {xorstr_("None"), xorstr_("Gold Rock"), xorstr_("Ice"), xorstr_("Frozen Turkey"), xorstr_("Fresh Brain"), xorstr_("Big Burger"), xorstr_("Mother's Day"), xorstr_("Fish"), xorstr_("Pug"), xorstr_("Hamster"), xorstr_("Capybara")};
std::vector<std::string> pythonSkins = {xorstr_("None"),xorstr_("Morocco"), xorstr_("Blackout"), xorstr_("Cold Hunter"), xorstr_("Pearl"), xorstr_("Tempered"), xorstr_("Toy")};
std::vector<std::string> pickaxeSkins = {xorstr_("None"), xorstr_("Raventale"), xorstr_("Damascus"), xorstr_("Rainbwo Pony"), xorstr_("Ancient Amber"), xorstr_("Shinobi"), xorstr_("Montezuma"), xorstr_("Gold Digger"), xorstr_("Inflatable")};
std::vector<std::string> hatchetSkins = {xorstr_("None"),xorstr_("Raider Hatchet"), xorstr_("Red Chopper"), xorstr_("Firefighter"), xorstr_("Raventale"), xorstr_("Damascus"), xorstr_("Shinobi"), xorstr_("Amber"), xorstr_("Inflatable")};
std::vector<std::string> jackHammerSkins = {xorstr_("None"),xorstr_("Comics"), xorstr_("From Hell"), xorstr_("Toy"), xorstr_("Ghost"), xorstr_("Azul"), xorstr_("Carrot")};
std::vector<std::string> combatKnifeSkins = {xorstr_("None"),xorstr_("From Hell"), xorstr_("Emerald"), xorstr_("Dread Lord"), xorstr_("Thug"), xorstr_("Glory"), xorstr_("Carbon Elite")};
std::vector<std::string> salvagedIcepickSkins = {xorstr_("None"), xorstr_("Raven"), xorstr_("Dark Arts"), xorstr_("Stinger"), xorstr_("Lavapick"), xorstr_("Inferno")};
std::vector<std::string> eokaSkins = {xorstr_("None"),xorstr_("Banana Eoka"), xorstr_("Flintlock"), xorstr_("Last Blast"), xorstr_("Hotdog")};
std::vector<std::string> crossBowSkins = {xorstr_("None"),xorstr_("Gold Spirit"), xorstr_("Ronin"), xorstr_("Victoria"), xorstr_("Cold Hunter"), xorstr_("Tempered"), xorstr_("Chrome"), xorstr_("From Hell")};
std::vector<std::string> bowSkins = {xorstr_("None"),xorstr_("Blackout"), xorstr_("Gold Leaf"), xorstr_("From Hell"), xorstr_("Tempered"), xorstr_("Abyss")};
std::vector<std::string> customSmgSkins = {xorstr_("None"),xorstr_("Alien Relic"), xorstr_("Polymer"), xorstr_("Looters"), xorstr_("Dark Flame"), xorstr_("Tempered"), xorstr_("Phantom"), xorstr_("Toy")};
std::vector<std::string> revolverSkins = {xorstr_("None"),xorstr_("Bandito"), xorstr_("Phantom"), xorstr_("Hot Tempered"), xorstr_("Frostbitten"), xorstr_("Eightball"), xorstr_("Outlaw")};
std::vector<std::string> boltySkins = {xorstr_("None"),xorstr_("Army"),xorstr_("Polymer"),xorstr_("Glory"),xorstr_("Survivor"),xorstr_("From Hell"),xorstr_("No Mercy"),xorstr_("Glory"),xorstr_("Tundra"),xorstr_("Redemption"),xorstr_("Tempered"),xorstr_("Cold Hunter")};

void c_draw::Init(Fuser& fuse)
{
	Font::initializeFonts();

	this->fuser = fuse;
	//this->fuslie
}

bool KmboxNetInit = false;
char kmboxComPort[256] = {0};
char DeviceLocalIPl[256] = {0};
static char ip[256] = {0};
static char port[256] = {0};
static char mac[256] = {0};

void c_draw::Run()
{
	if (GetAsyncKeyState(ctx.m_settings.menu_menu_key_hotkey) & 0x1)
		ctx.m_settings.Toggle(&ctx.m_settings.menu_menu_key);

	if (!ctx.m_settings.menu_menu_key)
	{
		SetWindowLong(fuser.GetHWND(), GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
		return;
	}
	else
		SetWindowLong(fuser.GetHWND(), GWL_EXSTYLE, WS_EX_LAYERED);

	this->Begin();
	this->DisplayBrand("Moe Rust");
	this->End();

	this->BeginMenu();
	this->MoveWindow();
	this->DrawBackground();

	//display tab 
	this->DisplayTab({
		TabButtons("Visuals", ICON_ESP, 9.5f),
		TabButtons("Radar", ICON_RADAR, 9.5f),
		TabButtons("Aimbot", ICON_AIMBOT, 9.5f),
		TabButtons("Weapons", ICON_WEAPONS, 9.5f),
		TabButtons("Movement", ICON_WORLD, 9.5f),
		TabButtons("Skin Changer", ICON_RESOURCE, 9.5f),
		TabButtons("Miscellaneous", ICON_MISC, 9.5f),
		TabButtons("Important", ICON_WARNING, 9.5f)
	});

	this->DisplayConfigs();
	this->DisplayPadding();

	if (this->IsActive("Visuals"))
	{
		this->Visuals();
	}
	else if (this->IsActive("Radar"))
	{
		this->Radar();
	}
	else if (this->IsActive("Aimbot"))
	{
		this->Aimbot();
	}
	else if (this->IsActive("Weapons"))
	{
		this->Weapons();
	}
	else if (this->IsActive("Movement"))
	{
		this->Movement();
	}
	else if (this->IsActive("Skin Changer"))
	{
		this->SkinChanger();
	}
	else if (this->IsActive("Miscellaneous"))
	{
		this->Miscellaneous();
	}
	else if (this->IsActive("Important"))
	{
		this->Important();
	}
	this->DisplayModals();

	this->EndMenu();
}

int esp_index = 0;

int player_index = 0;
int animal_index = 0;
int vehicle_index = 0;
int current_entity_index = 0;

int current_resource_index = 0;
int ores_index = 0;
int collectables_index = 0;

int current_lootable_index = 0;
int lootables_index = 0;
int dropped_items_index = 0;

int other_index = 0;

int radar_index = 0;

int radar_player_index = 0;
int radar_animal_index = 0;
int radar_vehicle_index = 0;
int radar_current_entity_index = 0;

int radar_current_resource_index = 0;
int radar_ores_index = 0;
int radar_collectables_index = 0;

int radar_current_lootable_index = 0;
int radar_lootables_index = 0;
int radar_dropped_items_index = 0;

int radar_other_index = 0;

void c_draw::Radar()
{
	UII::Controls::beginSection("Radar Settings", ImVec2(265, 200));
	{
		UII::Controls::Toggle("Enable Radar", "Displays Radar", &ctx.m_settings.Radar_Enable);
		if (UII::Controls::Button("Reload Radar Map", "(F1) Renadermap Move It To: Documents/moe/maps/Rust/"))
		{
			Map::ReloadMap = true;
		}
		UII::Controls::Hotkey("Centre Radar", &ctx.m_settings.Radar_Centre_Key);
		if (UII::Controls::Button("Load Corrupted Map", "Loads c:\\map.png"))
		{
			Map::ReloadCorruptedMap = true;
		}
		UII::Controls::Hotkey("Zoom In", &ctx.m_settings.Radar_Centre_ZoomIn);
		UII::Controls::Hotkey("Zoom Out", &ctx.m_settings.Radar_Centre_ZoomOut);
	}
	UII::Controls::endSection();
	UII::Controls::sameSection();
	UII::Controls::beginSection("Radar Entities", ImVec2(265, 210));
	{
		UII::Controls::Combo("ESP Type", {"Entity", "Resource", "Lootable", "Other"}, radar_index);
		if (radar_index == 0)
		{
			//Player ESP
			UII::Controls::Combo("Entity Type", {"Players", "Animals", "Vehicles"}, radar_current_entity_index);
			if (radar_current_entity_index == 0)
			{
				UII::Controls::Combo("Players", {"Player", "Friend", "Scientist"}, radar_player_index);
			}
			else if (radar_current_entity_index == 1)
			{
				UII::Controls::Combo("Animals", {"Bear", "Polar Bear", "Boar", "Stag", "Wolf", "Horse", "Chicken", "Shark"}, radar_animal_index);
			}
			else
			{
				UII::Controls::Combo("Vehicles", {"Attack Helicopter", "Bradley", "MiniCopter", "Attack MiniCopter", "Scrap Helicopter", "Train", "Car", "TugBoat", "RowBoat", "RHIB","Bicycle","Motorbike","Motorbike Sidecar"}, radar_vehicle_index);
			}
		}
		else if (radar_index == 1)
		{
			//Resource ESP
			UII::Controls::Combo("Resource Type", {"Ores", "Collectables"}, radar_current_resource_index);
			if (radar_current_resource_index == 0)
			{
				UII::Controls::Combo("Ores", {"Stone Ore", "Metal Ore", "Sulphur Ore"}, radar_ores_index);
			}
			else
			{
				UII::Controls::Combo("Collectables", {"Wood", "Hemp", "Pumpkin", "Corn", "Mushroom", "Potato", "Red Berry", "White Berry", "Black Berry", "Yellow Berry", "Blue Berry", "Green Berry", "Stone Collectable", "Metal Collectable", "Sulphur Collectable", "Diesel", "Metal Detector"}, radar_collectables_index);
			}
		}
		else if (radar_index == 2)
		{
			//Lootable ESP
			UII::Controls::Combo("Lootable Type", {"Dropped Items", "Lootables"}, radar_current_lootable_index);
			if (radar_current_lootable_index == 0)
			{
				UII::Controls::Combo("Dropped Items", {"Other Items", "Gun Items", "Tool Items", "Armour Items", "Medical Items", "Resource Items", "Food Items", "Explosive Items", "Ammo Items", "Components Items"}, radar_dropped_items_index);
			}
			else
			{
				UII::Controls::Combo("Lootables", {"Barrel", "Fuel Barrel", "Corpse", "Locked Crate", "Supply Drop", "Elite Crate", "Normal Crate", "Military Crate", "Medical Crate", "Food Crate", "Mine Cart", "Tool Box", "Ammo Crate", "Fuel Crate", "Under Water Crate", "Helicopter Crate", "Bradley Crate", "Tech Crates"}, radar_lootables_index);
			}
		}
		else if (radar_index == 3)
		{
			//Other ESP
			UII::Controls::Combo("Other", {"Stash", "Sleeping Bag", "Land Mine", "BearTrap", "Shotgun Trap", "AutoTurret", "Flame Turret", "SamSite", "Tool Cupboard", "Planter", "LargeBox", "Workbench", "Furnace", "Oil Refinery", "Recycler", "Battery"}, radar_other_index);
		}
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	UII::Controls::beginSection("ESP Settings");
	{
		if (radar_index == 0)
		{
			//Player ESP
			if (radar_current_entity_index == 0)
			{
				RadarPlayers player = GetRadarPlayerEntityConfig(radar_player_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &player.Enable);
				UII::Controls::Toggle("Name", "Display the name", &player.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &player.Distance);
				UII::Controls::Toggle("Weapon", "Display the weapon", &player.Weapon);
			
				if (radar_player_index != 2)
					UII::Controls::Toggle("Sleeper", "Display sleepers", &player.DrawSleepers);
				UII::Controls::Toggle("YDistance", "Display height difference", &player.YDistance);
				UII::Controls::SliderInt("Max Distance", &player.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &player.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &player.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &player.Scale, 0.1f, 1.0f);
				if (radar_player_index == 2)
					UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, player.IconType);
				else
					UII::Controls::Combo("Icon Type", {"None", "Dot"}, player.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &player.TextColour);
				UII::Controls::ColorPicker("Dot Colour", "", &player.VisibleDotColour, &player.InvisibleDotColour);
				if (radar_player_index != 2)
					UII::Controls::ColorPicker("Sleeper Colour", "", &player.SleeperColour);
				if (radar_player_index == 0)
					UII::Controls::ColorPicker("LocalPlayer Colour", "", &player.LocalPlayerColour);
				
				switch (radar_player_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.PlayerRadar, player);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.FriendRadar, player);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.ScientistRadar, player);
					break;
				default:
					break;
				}
			//	UII::Controls::Toggle("Fast Update Players", "Improves Esp, Uses More Resources(Requires Restart)", &ctx.m_settings.Misc_FastUpdatePlayers);
			//	if (ctx.m_settings.Misc_FastUpdatePlayers.changed())
			//		ctx.m_settings.save("Default.conf");
			}
			else if (radar_current_entity_index == 1)
			{
				RadarAnimals animal = GetRadarAnimalEntityConfig(radar_animal_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &animal.Enable);
				UII::Controls::Toggle("Name", "Display the name", &animal.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &animal.Distance);
				UII::Controls::Toggle("YDistance", "Display height difference", &animal.YDistance);
				UII::Controls::SliderInt("Max Distance", &animal.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &animal.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &animal.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &animal.Scale, 0.1f, 1.0f);
				UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, animal.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &animal.TextColour);
				if (animal.IconType == 1)
					UII::Controls::ColorPicker("Dot Colour", "", &animal.DotColour);
				switch (radar_animal_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.RadarBear, animal);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.RadarPolarBear, animal);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.RadarBoar, animal);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.RadarStag, animal);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.RadarWolf, animal);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.RadarHorse, animal);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RadarChicken, animal);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.RadarShark, animal);
					break;
				default:
					break;
				}
			}
			else
			{
				RadarVehicles vehicle = GetRadarVehicleEntityConfig(radar_vehicle_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &vehicle.Enable);
				UII::Controls::Toggle("Name", "Display the name", &vehicle.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &vehicle.Distance);
				UII::Controls::Toggle("YDistance", "Display height difference", &vehicle.YDistance);
				UII::Controls::SliderInt("Max Distance", &vehicle.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &vehicle.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &vehicle.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &vehicle.Scale, 0.1f, 1.0f);
				UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, vehicle.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &vehicle.TextColour);
				if (vehicle.IconType == 1)
					UII::Controls::ColorPicker("Dot Colour", "", &vehicle.DotColour);
				switch (radar_vehicle_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.RadarPatrolHelicopter, vehicle);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.RadarBradley, vehicle);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.RadarMiniCopter, vehicle);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.RadarAttackMiniCopter, vehicle);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.RadarScrapHeli, vehicle);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.RadarTrain, vehicle);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RadarCar, vehicle);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.RadarTugBoat, vehicle);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.RadarRowBoat, vehicle);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.RadarRHIB, vehicle);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.RadarPedalbike, vehicle);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.RadarMotorbike, vehicle);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.RadarMotorbikeSidecar, vehicle);
					break;
			
				default:
					break;
				}
			}
		}
		else if (radar_index == 1)
		{
			//Resource ESP
			if (radar_current_resource_index == 0)
			{
				RadarOres ores = GetRadarOresEntityConfig(radar_ores_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &ores.Enable);
				UII::Controls::Toggle("Name", "Display the name", &ores.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &ores.Distance);
				UII::Controls::Toggle("YDistance", "Display height difference", &ores.YDistance);
				UII::Controls::SliderInt("Max Distance", &ores.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &ores.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &ores.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &ores.Scale, 0.1f, 1.0f);
				UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, ores.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &ores.TextColour);
				if (ores.IconType == 1)
					UII::Controls::ColorPicker("Dot Colour", "", &ores.DotColour);
				switch (radar_ores_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.RadarStone, ores);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.RadarMetal, ores);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.RadarSulphur, ores);
					break;
				default:
					break;
				}
			}
			else if (radar_current_resource_index == 1)
			{
				RadarCollectables collectables = GetRadarCollectablesEntityConfig(radar_collectables_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &collectables.Enable);
				UII::Controls::Toggle("Name", "Display the name", &collectables.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &collectables.Distance);
				UII::Controls::Toggle("YDistance", "Display height difference", &collectables.YDistance);
				UII::Controls::SliderInt("Max Distance", &collectables.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &collectables.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &collectables.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &collectables.Scale, 0.1f, 1.0f);
				UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, collectables.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &collectables.TextColour);
				if (collectables.IconType == 1)
					UII::Controls::ColorPicker("Dot Colour", "", &collectables.DotColour);
				switch (radar_collectables_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.RadarWood, collectables);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.RadarHemp, collectables);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.RadarPumpkin, collectables);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.RadarCorn, collectables);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.RadarMushroom, collectables);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.RadarPotato, collectables);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RadarRedBerry, collectables);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.RadarWhiteBerry, collectables);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.RadarBlackBerry, collectables);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.RadarYellowBerry, collectables);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.RadarBlueBerry, collectables);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.RadarGreenBerry, collectables);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.RadarStoneCollectable, collectables);
					break;
				case 13:
					ctx.m_settings.Set(&ctx.m_settings.RadarMetalCollectable, collectables);
					break;
				case 14:
					ctx.m_settings.Set(&ctx.m_settings.RadarSulphurCollectable, collectables);
					break;
				case 15:
					ctx.m_settings.Set(&ctx.m_settings.RadarDiesel, collectables);
					break;
				case 16:
					ctx.m_settings.Set(&ctx.m_settings.RadarMetalDetector, collectables);
					break;
				}
			}
		}
		else if (radar_index == 2)
		{
			//Lootable ESP
			if (radar_current_lootable_index == 0)
			{
				RadarDroppedItems droppedItems = GetRadarDroppedItemsEntityConfig(radar_dropped_items_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &droppedItems.Enable);
				UII::Controls::Toggle("Name", "Display the name", &droppedItems.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &droppedItems.Distance);
				UII::Controls::Toggle("YDistance", "Display height difference", &droppedItems.YDistance);
				UII::Controls::SliderInt("Max Distance", &droppedItems.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &droppedItems.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &droppedItems.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &droppedItems.Scale, 0.1f, 1.0f);
				UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, droppedItems.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &droppedItems.TextColour);
				if (droppedItems.IconType == 1)
					UII::Controls::ColorPicker("Dot Colour", "", &droppedItems.DotColour);
				switch (radar_dropped_items_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.RadarOtherItems, droppedItems);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.RadarGunItems, droppedItems);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.RadarToolItems, droppedItems);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.RadarArmourItems, droppedItems);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.RadarMedicalItems, droppedItems);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.RadarResourceItems, droppedItems);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RadarFoodItems, droppedItems);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.RadarExplosiveItems, droppedItems);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.RadarAmmoItems, droppedItems);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.RadarComponentsItems, droppedItems);
					break;
				default:
					break;
				}
			}
			else if (radar_current_lootable_index == 1)
			{
				RadarLootables lootables = GetRadarLootablesEntityConfig(radar_lootables_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &lootables.Enable);
				UII::Controls::Toggle("Name", "Display the name", &lootables.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &lootables.Distance);
				UII::Controls::Toggle("YDistance", "Display height difference", &lootables.YDistance);
				UII::Controls::SliderInt("Max Distance", &lootables.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Max Y Distance", &lootables.MaxYDistance, 0, 200);
				UII::Controls::SliderInt("Text Size", &lootables.FontSize, 3, 24);
				UII::Controls::Slider("Icon Scale", &lootables.Scale, 0.1f, 1.0f);
				UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, lootables.IconType);
				UII::Controls::ColorPicker("Text Colour", "", &lootables.TextColour);
				if (lootables.IconType == 1)
					UII::Controls::ColorPicker("Dot Colour", "", &lootables.DotColour);
				switch (radar_lootables_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.RadarBarrel, lootables);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.RadarFuelBarrel, lootables);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.RadarCorpse, lootables);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.RadarLockedCrate, lootables);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.RadarSupplyDrop, lootables);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.RadarEliteCrate, lootables);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RadarNormalCrate, lootables);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.RadarMilitaryCrate, lootables);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.RadarMedicalCrate, lootables);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.RadarFoodCrate, lootables);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.RadarMineCart, lootables);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.RadarToolBox, lootables);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.RadarAmmoCrate, lootables);
					break;
				case 13:
					ctx.m_settings.Set(&ctx.m_settings.RadarFuelCrate, lootables);
					break;
				case 14:
					ctx.m_settings.Set(&ctx.m_settings.RadarUnderWaterCrate, lootables);
					break;
				case 15:
					ctx.m_settings.Set(&ctx.m_settings.RadarHelicopterCrate, lootables);
					break;
				case 16:
					ctx.m_settings.Set(&ctx.m_settings.RadarBradleyCrate, lootables);
					break;
				case 17:
					ctx.m_settings.Set(&ctx.m_settings.RadarTechCrate, lootables);
					break;
				}
			}
		}
		else if (radar_index == 3)
		{
			//Other ESP
			RadarDeployables deployables = GetRadarDeployablesEntityConfig(radar_other_index);
			UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &deployables.Enable);
			UII::Controls::Toggle("Name", "Display the name", &deployables.Name);
			UII::Controls::Toggle("Distance", "Display the distance", &deployables.Distance);
			UII::Controls::Toggle("YDistance", "Display height difference", &deployables.YDistance);
			UII::Controls::SliderInt("Max Distance", &deployables.MaxDistance, 0, 2000);
			UII::Controls::SliderInt("Max Y Distance", &deployables.MaxYDistance, 0, 200);
			UII::Controls::SliderInt("Text Size", &deployables.FontSize, 3, 24);
			UII::Controls::Slider("Icon Scale", &deployables.Scale, 0.1f, 1.0f);
			UII::Controls::Combo("Icon Type", {"None", "Dot", "Image"}, deployables.IconType);
			UII::Controls::ColorPicker("Text Colour", "", &deployables.TextColour);
			if (deployables.IconType == 1)
				UII::Controls::ColorPicker("Dot Colour", "", &deployables.DotColour);
			switch (radar_other_index)
			{
			case 0:
				ctx.m_settings.Set(&ctx.m_settings.RadarStash, deployables);
				break;
			case 1:
				ctx.m_settings.Set(&ctx.m_settings.RadarSleepingBag, deployables);
				break;
			case 2:
				ctx.m_settings.Set(&ctx.m_settings.RadarLandMine, deployables);
				break;
			case 3:
				ctx.m_settings.Set(&ctx.m_settings.RadarBearTrap, deployables);
				break;
			case 4:
				ctx.m_settings.Set(&ctx.m_settings.RadarShotgunTrap, deployables);
				break;
			case 5:
				ctx.m_settings.Set(&ctx.m_settings.RadarAutoTurret, deployables);
				break;
			case 6:
				ctx.m_settings.Set(&ctx.m_settings.RadarFlameTurret, deployables);
				break;
			case 7:
				ctx.m_settings.Set(&ctx.m_settings.RadarSamSite, deployables);
				break;
			case 8:
				ctx.m_settings.Set(&ctx.m_settings.RadarToolCupboard, deployables);
				break;
			case 9:
				ctx.m_settings.Set(&ctx.m_settings.RadarPlanter, deployables);
				break;
			case 10:
				ctx.m_settings.Set(&ctx.m_settings.RadarLargeBox, deployables);
				break;
			case 11:
				ctx.m_settings.Set(&ctx.m_settings.RadarWorkbench, deployables);
				break;
			case 12:
				ctx.m_settings.Set(&ctx.m_settings.RadarFurnace, deployables);
				break;
			case 13:
				ctx.m_settings.Set(&ctx.m_settings.RadarOilRefinery, deployables);
				break;
			case 14:
				ctx.m_settings.Set(&ctx.m_settings.RadarRecycler, deployables);
				break;
			case 15:
				ctx.m_settings.Set(&ctx.m_settings.RadarBattery, deployables);
				break;
			}
		}
	}
	UII::Controls::endSection();
}

void c_draw::Visuals()
{
	UII::Controls::beginSection("ESP", ImVec2(265, 210));
	{
		UII::Controls::Combo("ESP Type", {"Entity", "Resource", "Lootable", "Other"}, esp_index);
		if (esp_index == 0)
		{
			//Player ESP
			UII::Controls::Combo("Entity Type", {"Players", "Animals", "Vehicles"}, current_entity_index);
			if (current_entity_index == 0)
			{
				UII::Controls::Combo("Players", {"Player", "Friend", "Scientist"}, player_index);
			}
			else if (current_entity_index == 1)
			{
				UII::Controls::Combo("Animals", {"Bear", "Polar Bear", "Boar", "Stag", "Wolf", "Horse", "Chicken", "Shark"}, animal_index);
			}
			else
			{
				UII::Controls::Combo("Vehicles", {"Attack Helicopter", "Bradley", "MiniCopter", "Attack MiniCopter", "Scrap Helicopter", "Train", "Car", "TugBoat", "RowBoat", "RHIB","Bicycle","Motorbike","Motorbike Sidecar"}, vehicle_index);
			}
		}
		else if (esp_index == 1)
		{
			//Resource ESP
			UII::Controls::Combo("Resource Type", {"Ores", "Collectables"}, current_resource_index);
			if (current_resource_index == 0)
			{
				UII::Controls::Combo("Ores", {"Stone Ore", "Metal Ore", "Sulphur Ore"}, ores_index);
			}
			else
			{
				UII::Controls::Combo("Collectables", {"Wood", "Hemp", "Pumpkin", "Corn", "Mushroom", "Potato", "Red Berry", "White Berry", "Black Berry", "Yellow Berry", "Blue Berry", "Green Berry", "Stone Collectable", "Metal Collectable", "Sulphur Collectable", "Diesel", "Metal Detector"}, collectables_index);
			}
		}
		else if (esp_index == 2)
		{
			//Lootable ESP
			UII::Controls::Combo("Lootable Type", {"Dropped Items", "Lootables"}, current_lootable_index);
			if (current_lootable_index == 0)
			{
				UII::Controls::Combo("Dropped Items", {"Other Items", "Gun Items", "Tool Items", "Armour Items", "Medical Items", "Resource Items", "Food Items", "Explosive Items", "Ammo Items", "Components Items"}, dropped_items_index);
			}
			else
			{
				UII::Controls::Combo("Lootables", {"Barrel", "Fuel Barrel", "Corpse", "Locked Crate", "Supply Drop", "Elite Crate", "Normal Crate", "Military Crate", "Medical Crate", "Food Crate", "Mine Cart", "Tool Box", "Ammo Crate", "Fuel Crate", "Under Water Crate", "Helicopter Crate", "Bradley Crate", "Tech Crates"}, lootables_index);
			}
		}
		else if (esp_index == 3)
		{
			//Other ESP
			UII::Controls::Combo("Other", {"Stash", "Sleeping Bag", "Land Mine", "BearTrap", "Shotgun Trap", "AutoTurret", "Flame Turret", "SamSite", "Tool Cupboard", "Planter", "LargeBox", "Workbench", "Furnace", "Oil Refinery", "Recycler", "Battery"}, other_index);
		}
	}
	UII::Controls::endSection();

	UII::Controls::sameSection();

	UII::Controls::beginSection("ESP Colors", ImVec2(265, 231));
	{
		//Player ESP
		if (esp_index == 0)
		{
			if (current_entity_index == 0)
			{
				Players player = GetPlayerEntityConfig(player_index);
				UII::Controls::ColorPicker("Text Colour", "Text Color editer", &player.TextVisibleColour, &player.TextInvisibleColour);
				UII::Controls::ColorPicker("Box Colour", "Box ESP Color Editor", &player.BoxVisibleColour, &player.BoxInvisibleColour);
				UII::Controls::ColorPicker("Skeleton Colour", "Skeleton ESP Color Editor", &player.SkeletonVisibleColour, &player.SkeletonInvisibleColour);
				UII::Controls::ColorPicker("Sleeper Colour", "Sleeper Color Editor", &player.SleeperColour);
				switch (player_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Player, player);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Friend, player);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Scientist, player);
					break;
				default:
					break;
				}
			}
			else if (current_entity_index == 1)
			{
				Animals animal = GetAnimalEntityConfig(animal_index);
				UII::Controls::ColorPicker("Text Color", "Text Color editer", &animal.TextColour);
				switch (animal_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Bear, animal);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.PolarBear, animal);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Boar, animal);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.Stag, animal);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.Wolf, animal);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.Horse, animal);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.Chicken, animal);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.Shark, animal);
					break;
				default:
					break;
				}
			}
			else
			{
				Vehicles vehicle = GetVehicleEntityConfig(vehicle_index);
				UII::Controls::ColorPicker("Text Color", "Text Color editer", &vehicle.TextColour);
				switch (vehicle_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.PatrolHelicopter, vehicle);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Bradley, vehicle);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.MiniCopter, vehicle);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.AttackMiniCopter, vehicle);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.ScrapHeli, vehicle);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.Train, vehicle);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.Car, vehicle);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.TugBoat, vehicle);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.RowBoat, vehicle);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.RHIB, vehicle);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.Pedalbike, vehicle);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.Motorbike, vehicle);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.MotorbikeSidecar, vehicle);
					break;
				default:
					break;
				}
			}
		}
		//Resource ESP
		else if (esp_index == 1)
		{
			if (current_resource_index == 0)
			{
				Ores ores = GetOresEntityConfig(ores_index);
				UII::Controls::ColorPicker("Text Color", "Text Color editer", &ores.TextColour);
				switch (ores_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Stone, ores);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Metal, ores);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Sulphur, ores);
					break;
				default:
					break;
				}
			}
			else
			{
				Collectables collectables = GetCollectablesEntityConfig(collectables_index);
				UII::Controls::ColorPicker("Text Color", "Text Color editer", &collectables.TextColour);
				switch (collectables_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Wood, collectables);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Hemp, collectables);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Pumpkin, collectables);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.Corn, collectables);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.Mushroom, collectables);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.Potato, collectables);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RedBerry, collectables);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.WhiteBerry, collectables);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.BlackBerry, collectables);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.YellowBerry, collectables);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.BlueBerry, collectables);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.GreenBerry, collectables);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.StoneCollectable, collectables);
					break;
				case 13:
					ctx.m_settings.Set(&ctx.m_settings.MetalCollectable, collectables);
					break;
				case 14:
					ctx.m_settings.Set(&ctx.m_settings.SulphurCollectable, collectables);
					break;
				case 15:
					ctx.m_settings.Set(&ctx.m_settings.Diesel, collectables);
					break;
				case 16:
					ctx.m_settings.Set(&ctx.m_settings.MetalDetector, collectables);
					break;
				}
			}
		}
		//Lootable ESP
		else if (esp_index == 2)
		{
			if (current_lootable_index == 0)
			{
				DroppedItems droppedItems = GetDroppedItemsEntityConfig(dropped_items_index);
				UII::Controls::ColorPicker("Text Color", "Text Color editer", &droppedItems.TextColour);
				switch (dropped_items_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.OtherItems, droppedItems);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.GunItems, droppedItems);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.ToolItems, droppedItems);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.ArmourItems, droppedItems);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.MedicalItems, droppedItems);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.ResourceItems, droppedItems);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.FoodItems, droppedItems);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.ExplosiveItems, droppedItems);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.AmmoItems, droppedItems);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.ComponentsItems, droppedItems);
					break;
				default:
					break;
				}
			}
			else
			{
				Lootables lootables = GetLootablesEntityConfig(lootables_index);
				UII::Controls::ColorPicker("Text Color", "Text Color editer", &lootables.TextColour);
				switch (lootables_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Barrel, lootables);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.FuelBarrel, lootables);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Corpse, lootables);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.LockedCrate, lootables);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.SupplyDrop, lootables);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.EliteCrate, lootables);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.NormalCrate, lootables);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.MilitaryCrate, lootables);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.MedicalCrate, lootables);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.FoodCrate, lootables);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.MineCart, lootables);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.ToolBox, lootables);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.AmmoCrate, lootables);
					break;
				case 13:
					ctx.m_settings.Set(&ctx.m_settings.FuelCrate, lootables);
					break;
				case 14:
					ctx.m_settings.Set(&ctx.m_settings.UnderWaterCrate, lootables);
					break;
				case 15:
					ctx.m_settings.Set(&ctx.m_settings.HelicopterCrate, lootables);
					break;
				case 16:
					ctx.m_settings.Set(&ctx.m_settings.BradleyCrate, lootables);
					break;
				case 17:
					ctx.m_settings.Set(&ctx.m_settings.TechCrate, lootables);
					break;
				}
			}
		}
		else if (esp_index == 3)
		{
			Deployables deployables = GetDeployablesEntityConfig(other_index);
			UII::Controls::ColorPicker("Text Color", "Text Color editer", &deployables.TextColour);
			switch (other_index)
			{
			case 0:
				ctx.m_settings.Set(&ctx.m_settings.Stash, deployables);
				break;
			case 1:
				ctx.m_settings.Set(&ctx.m_settings.SleepingBag, deployables);
				break;
			case 2:
				ctx.m_settings.Set(&ctx.m_settings.LandMine, deployables);
				break;
			case 3:
				ctx.m_settings.Set(&ctx.m_settings.BearTrap, deployables);
				break;
			case 4:
				ctx.m_settings.Set(&ctx.m_settings.ShotgunTrap, deployables);
				break;
			case 5:
				ctx.m_settings.Set(&ctx.m_settings.AutoTurret, deployables);
				break;
			case 6:
				ctx.m_settings.Set(&ctx.m_settings.FlameTurret, deployables);
				break;
			case 7:
				ctx.m_settings.Set(&ctx.m_settings.SamSite, deployables);
				break;
			case 8:
				ctx.m_settings.Set(&ctx.m_settings.ToolCupboard, deployables);
				break;
			case 9:
				ctx.m_settings.Set(&ctx.m_settings.Planter, deployables);
				break;
			case 10:
				ctx.m_settings.Set(&ctx.m_settings.LargeBox, deployables);
				break;
			case 11:
				ctx.m_settings.Set(&ctx.m_settings.Workbench, deployables);
				break;
			case 12:
				ctx.m_settings.Set(&ctx.m_settings.Furnace, deployables);
				break;
			case 13:
				ctx.m_settings.Set(&ctx.m_settings.OilRefinery, deployables);
				break;
			case 14:
				ctx.m_settings.Set(&ctx.m_settings.Recycler, deployables);
				break;
			case 15:
				ctx.m_settings.Set(&ctx.m_settings.Battery, deployables);
				break;
			}
		}
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	UII::Controls::beginSection("ESP Settings");
	{
		if (esp_index == 0)
		{
			//Player ESP
			if (current_entity_index == 0)
			{
				Players player = GetPlayerEntityConfig(player_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &player.Enable);
				UII::Controls::Toggle("Name", "Display the name", &player.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &player.Distance);
				UII::Controls::Toggle("Weapon", "Display the weapon", &player.Weapon);
				if (player_index != 2 && player.Weapon)
					UII::Controls::Toggle("Tool", "Display player's tools", &player.ShowTools);
				UII::Controls::Toggle("Seperate Text Line For Distance", "Draws Distance Below Weapon", &player.DistanceSeperateLine);
				if (player_index == 0)
					UII::Controls::Toggle("Team ID", "Shows player teamids", &player.Team);
				UII::Controls::Toggle("Box ESP", "Display a box", &player.Box);
				UII::Controls::Toggle("Filled Box ESP", "Display a filled box", &player.FillBox);
				UII::Controls::Toggle("Skeleton", "", &player.Skeleton);
				UII::Controls::Toggle("Sleeper", "Display sleepers", &player.DrawSleepers);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &player.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &player.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &player.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, player.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, player.TextCulling);

				switch (player_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Player, player);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Friend, player);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Scientist, player);
					break;
				default:
					break;
				}
			}
			else if (current_entity_index == 1)
			{
				Animals animal = GetAnimalEntityConfig(animal_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &animal.Enable);
				UII::Controls::Toggle("Name", "Display the name", &animal.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &animal.Distance);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &animal.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &animal.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &animal.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, animal.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, animal.TextCulling);
				switch (animal_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Bear, animal);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.PolarBear, animal);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Boar, animal);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.Stag, animal);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.Wolf, animal);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.Horse, animal);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.Chicken, animal);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.Shark, animal);
					break;
				default:
					break;
				}
			}
			else
			{
				Vehicles vehicle = GetVehicleEntityConfig(vehicle_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &vehicle.Enable);
				UII::Controls::Toggle("Name", "Display the name", &vehicle.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &vehicle.Distance);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &vehicle.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &vehicle.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &vehicle.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, vehicle.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, vehicle.TextCulling);
				switch (vehicle_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.PatrolHelicopter, vehicle);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Bradley, vehicle);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.MiniCopter, vehicle);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.AttackMiniCopter, vehicle);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.ScrapHeli, vehicle);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.Train, vehicle);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.Car, vehicle);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.TugBoat, vehicle);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.RowBoat, vehicle);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.RHIB, vehicle);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.Pedalbike, vehicle);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.Motorbike, vehicle);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.MotorbikeSidecar, vehicle);
					break;
				default:
					break;
				}
			}
		}
		else if (esp_index == 1)
		{
			//Resource ESP
			if (current_resource_index == 0)
			{
				Ores ores = GetOresEntityConfig(ores_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &ores.Enable);
				UII::Controls::Toggle("Name", "Display the name", &ores.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &ores.Distance);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &ores.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &ores.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &ores.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, ores.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, ores.TextCulling);
				switch (ores_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Stone, ores);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Metal, ores);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Sulphur, ores);
					break;
				default:
					break;
				}
			}
			else if (current_resource_index == 1)
			{
				Collectables collectables = GetCollectablesEntityConfig(collectables_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &collectables.Enable);
				UII::Controls::Toggle("Name", "Display the name", &collectables.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &collectables.Distance);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &collectables.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &collectables.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &collectables.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, collectables.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, collectables.TextCulling);
				switch (collectables_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Wood, collectables);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.Hemp, collectables);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Pumpkin, collectables);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.Corn, collectables);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.Mushroom, collectables);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.Potato, collectables);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.RedBerry, collectables);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.WhiteBerry, collectables);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.BlackBerry, collectables);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.YellowBerry, collectables);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.BlueBerry, collectables);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.GreenBerry, collectables);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.StoneCollectable, collectables);
					break;
				case 13:
					ctx.m_settings.Set(&ctx.m_settings.MetalCollectable, collectables);
					break;
				case 14:
					ctx.m_settings.Set(&ctx.m_settings.SulphurCollectable, collectables);
					break;
				case 15:
					ctx.m_settings.Set(&ctx.m_settings.Diesel, collectables);
					break;
				case 16:
					ctx.m_settings.Set(&ctx.m_settings.MetalDetector, collectables);
					break;
				}
			}
		}
		else if (esp_index == 2)
		{
			//Lootable ESP
			if (current_lootable_index == 0)
			{
				DroppedItems droppedItems = GetDroppedItemsEntityConfig(dropped_items_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &droppedItems.Enable);
				UII::Controls::Toggle("Name", "Display the name", &droppedItems.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &droppedItems.Distance);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &droppedItems.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &droppedItems.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &droppedItems.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, droppedItems.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, droppedItems.TextCulling);
				switch (dropped_items_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.OtherItems, droppedItems);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.GunItems, droppedItems);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.ToolItems, droppedItems);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.ArmourItems, droppedItems);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.MedicalItems, droppedItems);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.ResourceItems, droppedItems);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.FoodItems, droppedItems);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.ExplosiveItems, droppedItems);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.AmmoItems, droppedItems);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.ComponentsItems, droppedItems);
					break;
				default:
					break;
				}
			}
			else if (current_lootable_index == 1)
			{
				Lootables lootables = GetLootablesEntityConfig(lootables_index);
				UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &lootables.Enable);
				UII::Controls::Toggle("Name", "Display the name", &lootables.Name);
				UII::Controls::Toggle("Distance", "Display the distance", &lootables.Distance);
				UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &lootables.EnableInBattleMode);
				UII::Controls::SliderInt("Max Distance", &lootables.MaxDistance, 0, 2000);
				UII::Controls::SliderInt("Text Size", &lootables.FontSize, 3, 24);
				UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, lootables.OpacityCulling);
				UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, lootables.TextCulling);
				switch (lootables_index)
				{
				case 0:
					ctx.m_settings.Set(&ctx.m_settings.Barrel, lootables);
					break;
				case 1:
					ctx.m_settings.Set(&ctx.m_settings.FuelBarrel, lootables);
					break;
				case 2:
					ctx.m_settings.Set(&ctx.m_settings.Corpse, lootables);
					break;
				case 3:
					ctx.m_settings.Set(&ctx.m_settings.LockedCrate, lootables);
					break;
				case 4:
					ctx.m_settings.Set(&ctx.m_settings.SupplyDrop, lootables);
					break;
				case 5:
					ctx.m_settings.Set(&ctx.m_settings.EliteCrate, lootables);
					break;
				case 6:
					ctx.m_settings.Set(&ctx.m_settings.NormalCrate, lootables);
					break;
				case 7:
					ctx.m_settings.Set(&ctx.m_settings.MilitaryCrate, lootables);
					break;
				case 8:
					ctx.m_settings.Set(&ctx.m_settings.MedicalCrate, lootables);
					break;
				case 9:
					ctx.m_settings.Set(&ctx.m_settings.FoodCrate, lootables);
					break;
				case 10:
					ctx.m_settings.Set(&ctx.m_settings.MineCart, lootables);
					break;
				case 11:
					ctx.m_settings.Set(&ctx.m_settings.ToolBox, lootables);
					break;
				case 12:
					ctx.m_settings.Set(&ctx.m_settings.AmmoCrate, lootables);
					break;
				case 13:
					ctx.m_settings.Set(&ctx.m_settings.FuelCrate, lootables);
					break;
				case 14:
					ctx.m_settings.Set(&ctx.m_settings.UnderWaterCrate, lootables);
					break;
				case 15:
					ctx.m_settings.Set(&ctx.m_settings.HelicopterCrate, lootables);
					break;
				case 16:
					ctx.m_settings.Set(&ctx.m_settings.BradleyCrate, lootables);
					break;
				case 17:
					ctx.m_settings.Set(&ctx.m_settings.TechCrate, lootables);
					break;
				}
			}
		}
		else if (esp_index == 3)
		{
			//Other ESP
			Deployables deployables = GetDeployablesEntityConfig(other_index);
			UII::Controls::Toggle("Enable", "Turn on / Off the ESP", &deployables.Enable);
			UII::Controls::Toggle("Name", "Display the name", &deployables.Name);
			UII::Controls::Toggle("Distance", "Display the distance", &deployables.Distance);
			UII::Controls::Toggle("Enable In BattleMode", "Enable in BattleMode", &deployables.EnableInBattleMode);
			UII::Controls::SliderInt("Max Distance", &deployables.MaxDistance, 0, 2000);
			UII::Controls::SliderInt("Text Size", &deployables.FontSize, 3, 24);
			UII::Controls::Combo("Opacity Culling Mode", {"None", "Enabled"}, deployables.OpacityCulling);
			UII::Controls::Combo("Text Culling Mode", {"None", "Enabled"}, deployables.TextCulling);
			switch (other_index)
			{
			case 0:
				ctx.m_settings.Set(&ctx.m_settings.Stash, deployables);
				break;
			case 1:
				ctx.m_settings.Set(&ctx.m_settings.SleepingBag, deployables);
				break;
			case 2:
				ctx.m_settings.Set(&ctx.m_settings.LandMine, deployables);
				break;
			case 3:
				ctx.m_settings.Set(&ctx.m_settings.BearTrap, deployables);
				break;
			case 4:
				ctx.m_settings.Set(&ctx.m_settings.ShotgunTrap, deployables);
				break;
			case 5:
				ctx.m_settings.Set(&ctx.m_settings.AutoTurret, deployables);
				break;
			case 6:
				ctx.m_settings.Set(&ctx.m_settings.FlameTurret, deployables);
				break;
			case 7:
				ctx.m_settings.Set(&ctx.m_settings.SamSite, deployables);
				break;
			case 8:
				ctx.m_settings.Set(&ctx.m_settings.ToolCupboard, deployables);
				break;
			case 9:
				ctx.m_settings.Set(&ctx.m_settings.Planter, deployables);
				break;
			case 10:
				ctx.m_settings.Set(&ctx.m_settings.LargeBox, deployables);
				break;
			case 11:
				ctx.m_settings.Set(&ctx.m_settings.Workbench, deployables);
				break;
			case 12:
				ctx.m_settings.Set(&ctx.m_settings.Furnace, deployables);
				break;
			case 13:
				ctx.m_settings.Set(&ctx.m_settings.OilRefinery, deployables);
				break;
			case 14:
				ctx.m_settings.Set(&ctx.m_settings.Recycler, deployables);
				break;
			case 15:
				ctx.m_settings.Set(&ctx.m_settings.Battery, deployables);
				break;
			}
		}
	}
	UII::Controls::endSection();
}

void c_draw::Aimbot()
{
	UII::Controls::beginSection("Aimbot");
	{
		UII::Controls::Toggle("Enable", "Turn on / Off", &ctx.m_settings.Aimbot_Enable);
		UII::Controls::Toggle("Target Players", "Should target Players", &ctx.m_settings.Aimbot_TargetPlayer);
		UII::Controls::Toggle("Target Scientists", "Should target Scientists", &ctx.m_settings.Aimbot_TargetScientist);
		UII::Controls::Toggle("Draw FOV", "Draws FOV Circle", &ctx.m_settings.Aimbot_DrawFOV);
		UII::Controls::Toggle("Visible Only", "Target only Visible", &ctx.m_settings.Aimbot_VisibleOnly);
		UII::Controls::Toggle("Target Attack Helicopter", "Targets Patrol/Attack Helicopter", &ctx.m_settings.Aimbot_TargetHeli);
		UII::Controls::Toggle("Target Bradley", "Targets Bradley APC", &ctx.m_settings.Aimbot_TargetBradley);
		UII::Controls::Toggle("Short Prediction Algorithm", "Don't Use Unless You REALLY NEED TO/ Are Poor", &ctx.m_settings.Aimbot_ShortAlgo);
		UII::Controls::Combo("Target Mode", {"Closest To Player", "Closest To Crosshair", "Both"}, ctx.m_settings.Aimbot_TargetPriority);
		UII::Controls::SliderInt("Retargeting Time MS (Requires Sticky Targeting)", &ctx.m_settings.Aimbot_RetargetTime,0,1000);
		UII::Controls::Toggle("Sticky Targeting", "Maintain a constant target", &ctx.m_settings.Aimbot_StickyAim);
		if (ctx.m_settings.Aimbot_StickyAim)
		{
			UII::Controls::Toggle("Sticky Ignore FOV", "Allows Aimbot To Work With Bows Aiming Outside FOV", &ctx.m_settings.Aimbot_StickyAimIgnoreFOV);
			UII::Controls::Toggle("Sticky Ignore Visibility", "Allows Aimbot To Target Players Covered By Your Body/Gun", &ctx.m_settings.Aimbot_StickyAimIgnoreVisibility);
		}
		UII::Controls::Toggle("Targetting Line", "Draws Line To Target. For Silent Aim Users", &ctx.m_settings.Aimbot_DrawTarget);
		if (ctx.m_settings.Aimbot_DrawTarget)
		{
			UII::Controls::ColorPicker("Target Line Colour", "Change Target Line Colour", &ctx.m_settings.Aimbot_DrawTarget_Colour);
		}
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	ImVec2 sectionSize = ImVec2(265, 450);
	if (ctx.m_settings.Aimbot_AimType == 2 || ctx.m_settings.Aimbot_AimType == 3 || ctx.m_settings.Aimbot_AimType == 4)
		sectionSize = ImVec2(265, 273 - 5);

	UII::Controls::beginSection("Aimbot Settings", sectionSize);
	{
		UII::Controls::Combo("Aim Type", {"View Angle", "Silent Aim", "PicoW", "Kmbox", "Kmbox Net"}, ctx.m_settings.Aimbot_AimType);
		UII::Controls::Combo("Bone", {"Head", "Neck", "Chest", "Pelvis","Closest"}, ctx.m_settings.Aimbot_AimBone);
		if (ctx.m_settings.Aimbot_Hybrid)
		{
			UII::Controls::Combo("Hybrid Bone", {"Head", "Neck", "Chest", "Pelvis"}, ctx.m_settings.Aimbot_Hybrid_AimBone);
		}
		UII::Controls::Combo("Smooth Type", {"Linear", "Humanized"}, ctx.m_settings.Aimbot_SmoothingType);
		UII::Controls::Hotkey("Aim Key", &ctx.m_settings.Aimbot_AimKey);
		if (ctx.m_settings.Aimbot_AimType != 1)
			UII::Controls::Toggle("Silent Hybrid", "Uses Current Aim Mode With Silent Aim", &ctx.m_settings.Aimbot_Hybrid);

		UII::Controls::SliderInt("Smooth X", &ctx.m_settings.Aimbot_SmoothX, 1, 100);
		UII::Controls::SliderInt("Smooth Y", &ctx.m_settings.Aimbot_SmoothY, 1, 100);
		UII::Controls::Toggle("Alternative Slow Projectile Smoothing", "Individual Smoothing For Bows/Crossbows", &ctx.m_settings.Aimbot_SlowMovingProjectile_Smooth);
		if (ctx.m_settings.Aimbot_SlowMovingProjectile_Smooth)
		{
			UII::Controls::SliderInt("Slow Projectile Smooth X", &ctx.m_settings.Aimbot_SlowMovingProjectile_SmoothX, 1, 100);
			UII::Controls::SliderInt("Slow Projectile Y", &ctx.m_settings.Aimbot_SlowMovingProjectile_SmoothY, 1, 100);
		}
		UII::Controls::SliderInt("Max Distance", &ctx.m_settings.Aimbot_MaxDistance, 0, 1000);
		if (ctx.m_settings.Aimbot_TargetHeli || ctx.m_settings.Aimbot_TargetBradley)
		{
			UII::Controls::SliderInt("Vehicle Max Distance", &ctx.m_settings.Aimbot_VehicleMaxDistance, 1, 1000);
		}
		UII::Controls::SliderInt("FOV", &ctx.m_settings.Aimbot_FOV, 0, 1000);
		if (ctx.m_settings.Aimbot_Hybrid)
		{
			UII::Controls::SliderInt("Hybrid FOV", &ctx.m_settings.Aimbot_Hybrid_FOV, 0, ctx.m_settings.Aimbot_FOV);
			UII::Controls::SliderInt("Hybrid CutOut FOV", &ctx.m_settings.Aimbot_Hybrid_CutOut_FOV, 0, ctx.m_settings.Aimbot_Hybrid_FOV);
		}
		if (ctx.m_settings.Aimbot_DrawFOV)
			UII::Controls::ColorPicker("FOV Colour", "FOV Colour Editor", &ctx.m_settings.Aimbot_FOVColour);
		UII::Controls::Toggle("Movement Prediction", "Predicts Movement", &ctx.m_settings.Aimbot_Movement_Prediction);
		if (ctx.m_settings.Aimbot_Movement_Prediction)
		{
			UII::Controls::Slider("Movement Prediction Modifier", &ctx.m_settings.Aimbot_Movement_Prediction_Modifier, 0, 1.5f);
			UII::Controls::SliderInt("Movement Prediction Samples", &ctx.m_settings.Aimbot_Prediction_SampleSize, 0, 24);
		}
		UII::Controls::Toggle("Localplayer Movement Prediction", "Predicts LocalPlayer Movement", &ctx.m_settings.Aimbot_Movement_LocalPlayerPrediction);
	}
	UII::Controls::endSection();
	if (ctx.m_settings.Aimbot_AimType == 2 || ctx.m_settings.Aimbot_AimType == 3 || ctx.m_settings.Aimbot_AimType == 4)
	{
		UII::Controls::sameSection();

		UII::Controls::beginSection("Misc", ImVec2(265, 175));
		{
			
			 if (ctx.m_settings.Aimbot_AimType == 3)
			{
				//ctx.m_settings.Set<const char*>(&ctx.m_settings.Aimbot_KmboxComPort, kmbox::find_port("USB-SERIAL CH340").c_str());

				//	UII::Controls::Input("KMbox Comport", kmboxComPort, 256);
				UII::Controls::SliderInt("KMBox Aim Delay", &ctx.m_settings.Aimbot_KmboxDelay, 0, 100);
				UII::Controls::InputInt("KMbox Baudrate", &ctx.m_settings.Aimbot_KmboxBaudRate);
				if (UII::Controls::Button("Connect", "Initializes Mouse Hardware"))
				{
					std::string com = std::string(ctx.m_settings.Aimbot_KmboxComPort);
					kmbox::KmboxInitialize(com.c_str());
				}
				ctx.m_settings.Set<const char*>(&ctx.m_settings.Aimbot_KmboxComPort, kmboxComPort);
				//kmbox
			}
			else if (ctx.m_settings.Aimbot_AimType == 4)
			{
				UII::Controls::Input("KMboxNET IP", ip, 256);
				UII::Controls::Input("KMboxNET Port", port, 256);
				UII::Controls::Input("KMboxNET MAC", mac, 256);
				UII::Controls::SliderInt("KMBoxNET Aim Delay", &ctx.m_settings.Aimbot_KmboxDelay, 0, 100);
				if (UII::Controls::Button("Connect", "Initializes Mouse Hardware"))
				{
					KmBoxMgr.InitDevice(ip, (WORD)std::stoi(port), mac);
				}
				ctx.m_settings.Set<const char*>(&ctx.m_settings.Aimbot_NetworkIP, ip);
				ctx.m_settings.Set<const char*>(&ctx.m_settings.Aimbot_NetworkPort, port);
				ctx.m_settings.Set<const char*>(&ctx.m_settings.Aimbot_NetworkMac, mac);
				//kmbox net
			}

			/*if (UII::Controls::Button("Crosshair", "Modify your Crosshair"))
			{
				UII::ModalController::crosshairEditor = true;
			}
			UII::Controls::Toggle("Crosshair", "Toggle Crosshair", &ctx.m_settings.aimbot_crosshair);*/
		}
		UII::Controls::endSection();
	}
}

void c_draw::Weapons()
{
	UII::Controls::beginSection("Weapons");
	{
		UII::Controls::Toggle("No Recoil", "Turn on / Off", &ctx.m_settings.Weapon_Misc_NoRecoil);
		if (ctx.m_settings.Weapon_Misc_NoRecoil)
		{
			UII::Controls::SliderInt("Recoil Amount X", &ctx.m_settings.Weapon_Misc_RecoilAmountx, 1, 100);
			UII::Controls::SliderInt("Recoil Amount Y", &ctx.m_settings.Weapon_Misc_RecoilAmounty, 1, 100);
			UII::Controls::Toggle("No Recoil Disabliser", "Turn Off Recoil On Key", &ctx.m_settings.Weapon_Misc_NoRecoilDisabler);
			if (ctx.m_settings.Weapon_Misc_NoRecoilDisabler)
				UII::Controls::Hotkey("No Recoil Disabliser Key", &ctx.m_settings.Weapon_Misc_NoRecoilDisablerKey);
		}
		UII::Controls::Toggle("No Spread", "Turn on / Off", &ctx.m_settings.Weapon_Misc_NoAimCone);
		if (ctx.m_settings.Weapon_Misc_NoAimCone)
			UII::Controls::SliderInt("AimCone Amount", &ctx.m_settings.Weapon_Misc_AimConeAmount, 1, 100);
		UII::Controls::Toggle("Scope Sway Reduction", "Tun on / Off", &ctx.m_settings.Weapon_Misc_NoSway);
		if (ctx.m_settings.Weapon_Misc_NoSway)
			UII::Controls::SliderInt("Sway Amount", &ctx.m_settings.Weapon_Misc_SwayAmount, 1, 100);
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	UII::Controls::beginSection("Weapons Misc");
	{
		UII::Controls::Toggle("Automatic", "Changes Weapons To Be Automatic", &ctx.m_settings.Weapon_Misc_ForceAutomatic);
		UII::Controls::Toggle("Instant Eoka", "Instantly fires Eoka", &ctx.m_settings.Weapon_Misc_InstantEoka);
		UII::Controls::Toggle("Instant Compound Bow", "Instantly fires Compound Bow", &ctx.m_settings.Weapon_Misc_InstantCompound);
		UII::Controls::Toggle("Long Tools", "YOU GOT LONG TOOLS BOI", &ctx.m_settings.Weapon_Misc_AttackDistance);
		UII::Controls::Toggle("No Melee Sprint Block", "RUN FAST", &ctx.m_settings.Weapon_Misc_AttackableSprinting);
		UII::Controls::Toggle("Fast Hammer", "Attack With Hammer Faster", &ctx.m_settings.Weapon_Misc_InstantHammer);
		UII::Controls::Toggle("Building Plan Rotation", "Rotate Buildings With Arrow Keys", &ctx.m_settings.Weapon_Misc_FreeRotation);
		UII::Controls::Slider("Building Plan Rotation Degree", &ctx.m_settings.Weapon_Misc_FreeRotation_Degrees, 0.001f, 5.f);
	}
	UII::Controls::endSection();
}

void c_draw::Movement()
{
	UII::Controls::beginSection("Movement");
	{
		//UII::Controls::Toggle("SpiderMan", "Climb Shit", &ctx.m_settings.Movement_Misc_Spiderman);
		UII::Controls::Toggle("No Fall Damage", "Take No Fall Damage", &ctx.m_settings.Movement_Misc_NoFallDamage);
		UII::Controls::Toggle("Jump And Shoot", "Shoot while jumping", &ctx.m_settings.Movement_Misc_AlwaysShoot);
	//	UII::Controls::Toggle("Capsule Bypass", "Jump against a Wall", &ctx.m_settings.Movement_Misc_CapsuleBypass);
		UII::Controls::Toggle("No Sprint Delays", "No Sprint Delays", &ctx.m_settings.Movement_Misc_NoSprintDelays);
		UII::Controls::Toggle("Infinite jumps", "Jump Boi", &ctx.m_settings.Movement_Misc_InfiniteJump);
		UII::Controls::Toggle("FlyHack", "", &ctx.m_settings.Movement_Misc_FlyHack);
		if (ctx.m_settings.Movement_Misc_FlyHack)
		{
			UII::Controls::Hotkey("FlyHack Key", &ctx.m_settings.Movement_Misc_FlyHack_Key);
			UII::Controls::Toggle("Remove FlyHack Ladder", "", &ctx.m_settings.Movement_Misc_FlyHack_NoLadder);
		}
		UII::Controls::Toggle("SpeedHack", "Only For A Few Servers", &ctx.m_settings.Movement_Misc_SpeedHack);
		if (ctx.m_settings.Movement_Misc_SpeedHack)
		{
			UII::Controls::Toggle("Use Key For Speed Hack", "", &ctx.m_settings.Movement_Misc_SpeedHack_UseKey);
			if (ctx.m_settings.Movement_Misc_SpeedHack_UseKey)
				UII::Controls::Hotkey("SpeedHack Key", &ctx.m_settings.Movement_Misc_SpeedHack_Key);
			UII::Controls::SliderInt("SpeedHack Amount", &ctx.m_settings.Movement_Misc_SpeedHack_Speed, 1, 100);
		}
		UII::Controls::Toggle("Change Gravity", "Turn on / Off", &ctx.m_settings.Movement_Misc_GravityModifier);
		if (ctx.m_settings.Movement_Misc_GravityModifier)
			UII::Controls::Slider("Gravity Amount", &ctx.m_settings.Movement_Misc_GravityModifierAmount, 0.f, 2.5f);
	//	UII::Controls::Toggle("Fake Lag", "Lag", &ctx.m_settings.Movement_Misc_FakeLag);
	//	if (ctx.m_settings.Movement_Misc_FakeLag)
		//	UII::Controls::SliderInt("Fake Lag Delay (MS)", &ctx.m_settings.Movement_Misc_FakeLagDelay, 0, 1000);
	//	UII::Controls::Toggle("Interactive Debug Camera", "Debug Cam", &ctx.m_settings.Movement_Misc_InteractiveDebug);
		//	UII::Controls::Toggle("View Angle Manipulation", "Change Look Position", &ctx.m_settings.Movement_Misc_Manipulation);
		UII::Controls::Toggle("Omni Sprint", "Fast Cards Only", &ctx.m_settings.Movement_Misc_OmniSprint);
		UII::Controls::Toggle("Anti Aim", "Enables Spinbot", &ctx.m_settings.Movement_Misc_AntiAim);
		UII::Controls::Hotkey("Anti Aim Key", &ctx.m_settings.Movement_Misc_AntiAim_Hotkey);
		UII::Controls::Combo("Anti Aim Type", {xorstr_("Random"),xorstr_("Manual")}, ctx.m_settings.Movement_Misc_AntiAim_Mode);
		UII::Controls::ColorPicker("Anti Aim Indicator Color", "Anti Aim Color Editor", &ctx.m_settings.Movement_Misc_AntiAim_Indicator_Colour);
		if (ctx.m_settings.Movement_Misc_AntiAim_Mode == 1)
		{
			UII::Controls::Hotkey("Anti Aim Right", &ctx.m_settings.Movement_Misc_AntiAim_Right);
			UII::Controls::Hotkey("Anti Aim Left", &ctx.m_settings.Movement_Misc_AntiAim_Left);
			UII::Controls::Hotkey("Anti Aim Back", &ctx.m_settings.Movement_Misc_AntiAim_Back);
		}
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	/*UII::Controls::beginSection("Debug Controls", ImVec2(250, 283 - 5));
	{
		UII::Controls::Hotkey("Debug Camera Key", &ctx.m_settings.Movement_Misc_DebugCamKey);
		UII::Controls::Hotkey("Forward Key", &ctx.m_settings.Movement_Misc_DebugForward);
		UII::Controls::Hotkey("Back Key", &ctx.m_settings.Movement_Misc_DebugBackwards);
		UII::Controls::Hotkey("Right Key", &ctx.m_settings.Movement_Misc_DebugRight);
		UII::Controls::Hotkey("Left Key", &ctx.m_settings.Movement_Misc_DebugLeft);
		UII::Controls::Hotkey("Upward Key", &ctx.m_settings.Movement_Misc_DebugUp);
		UII::Controls::Hotkey("Downward Key", &ctx.m_settings.Movement_Misc_DebugDown);
	}
	UII::Controls::endSection();

	UII::Controls::sameSection();
	*/


	UII::Controls::beginSection("Other Misc", ImVec2(250, 163));
	{
		UII::Controls::Toggle("Instant Interaction", "Fast Pickup Doors/ Revive/ Backpacks", &ctx.m_settings.Misc_InstantInteract);
	}
	UII::Controls::endSection();
}

//25
void c_draw::SkinChanger()
{
	UpdateSkinConfig();
	UII::Controls::beginSection("SkinChanger");
	{
		UII::Controls::Toggle("Enable", "Turn on / Off", &ctx.m_settings.Skins_Enable);
		UII::Controls::Combo("AK47", akSkins, ctx.m_settings.Skins_AkSkin);
		UII::Controls::Combo("LR300", lrSkins, ctx.m_settings.Skins_LR300Skin);
		UII::Controls::Combo("M249", m2Skins, ctx.m_settings.Skins_M249Skin);
		UII::Controls::Combo("M39", m39Skins, ctx.m_settings.Skins_M39Skin);
		UII::Controls::Combo("L96", l96Skins, ctx.m_settings.Skins_L96Skin);
		UII::Controls::Combo("MP5", mp5Skins, ctx.m_settings.Skins_MP5Skin);
		UII::Controls::Combo("Bolt", boltySkins, ctx.m_settings.Skins_BoltSkin);
		UII::Controls::Combo("Semi Rifle", semiRifleSkins, ctx.m_settings.Skins_SemiAutoRifleSkin);
		UII::Controls::Combo("Semi Pistol", semiPistolSkins, ctx.m_settings.Skins_SemiAutoPistolSkin);
		UII::Controls::Combo("Thompson", thompsonSkins, ctx.m_settings.Skins_ThompsonSkin);
		UII::Controls::Combo("Revolver", revolverSkins, ctx.m_settings.Skins_RevolverSkin);
		UII::Controls::Combo("Custom SMG", customSmgSkins, ctx.m_settings.Skins_CustomSMGSkin);
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	UII::Controls::beginSection("SkinChanger2");
	{
		UII::Controls::Combo("Eoka", eokaSkins, ctx.m_settings.Skins_EokaSkin);
		UII::Controls::Combo("Bow", bowSkins, ctx.m_settings.Skins_BowSkin);
		UII::Controls::Combo("Crossbow", crossBowSkins, ctx.m_settings.Skins_CrossbowSkin);
		UII::Controls::Combo("Rock", rockSkins, ctx.m_settings.Skins_RockSkin);
		UII::Controls::Combo("Rocket Launcher", rocketLauncherSkins, ctx.m_settings.Skins_RocketLauncherSkin);
		UII::Controls::Combo("Hatchet", hatchetSkins, ctx.m_settings.Skins_HatchetSkin);
		UII::Controls::Combo("Pickaxe", pickaxeSkins, ctx.m_settings.Skins_PickaxeSkin);
		UII::Controls::Combo("Salvaged Icepick", salvagedIcepickSkins, ctx.m_settings.Skins_SalvagedIcepickSkin);
		UII::Controls::Combo("Combat Knife", combatKnifeSkins, ctx.m_settings.Skins_CombatKnifeSkin);
		UII::Controls::Combo("Stone Pickaxe", stonePickSkins, ctx.m_settings.Skins_StonePickaxeSkin);
		UII::Controls::Combo("Stone Hatchet", stoneHatchetSkins, ctx.m_settings.Skins_StoneHatchetSkin);
		UII::Controls::Combo("Jack Hammer", jackHammerSkins, ctx.m_settings.Skins_JackHammerSkin);
	}
	UII::Controls::endSection();
}

void c_draw::Miscellaneous()
{
	UII::Controls::beginSection("Visuals");
	{
		/*UII::Controls::Toggle("Debug Camera", "Turn on / Off", &ctx.m_settings.Visuals_Misc_DebugCam);
		if (ctx.m_settings.Visuals_Misc_DebugCam)
		{
	//		UII::Controls::Hotkey("Key", &ctx.m_settings.Visuals_Misc_DebugCamKey);
	//		UII::Controls::Button("Info!", "Bind DebugCamera To This Key In Game. (Bind M Debug.DebugCamera)");
		}*/
	/*	if (UII::Controls::Toggle("Admin ESP", "Turn on / Off", &ctx.m_settings.Visuals_Misc_AdminEsp))
		{
			std::shared_ptr<OcclusionCulling> occlusionculling = std::make_shared<OcclusionCulling>();
			if (ctx.m_settings.Visuals_Misc_AdminEsp)
			{
				occlusionculling->WriteDebugSettings(DebugFilter::Dynamic);
				occlusionculling->WriteLayerMask(131072);
			}
			else
			{
				occlusionculling->WriteDebugSettings(DebugFilter::Off);
				occlusionculling->WriteLayerMask(0);
			}
		}*/
	/*if (UII::Controls::Toggle("Change FOV", "Turn on / Off", &ctx.m_settings.Visuals_Misc_ChangeFOV))
		{
			std::shared_ptr<ConvarGraphics> convargraphics = std::make_shared<ConvarGraphics>();
			if (ctx.m_settings.Visuals_Misc_ChangeFOV)
				convargraphics->WriteFOV(ctx.m_settings.Visuals_Misc_FOV);
			else
				convargraphics->WriteFOV(90);
		}
		if (ctx.m_settings.Visuals_Misc_ChangeFOV)
		{
			if (UII::Controls::SliderInt("FOV", &ctx.m_settings.Visuals_Misc_FOV, 1, 180))
			{
				std::shared_ptr<ConvarGraphics> convargraphics = std::make_shared<ConvarGraphics>();
				convargraphics->WriteFOV(ctx.m_settings.Visuals_Misc_FOV);
			}
		}*/
		
	/*	if (UII::Controls::Toggle("Change Time", "Turn on / Off", &ctx.m_settings.Visuals_Misc_ChangeTime))
		{
			std::shared_ptr<ConvarAdmin> convaradmin = std::make_shared<ConvarAdmin>();
			if (ctx.m_settings.Visuals_Misc_ChangeTime)
			{
				convaradmin->SetAdminTime(ctx.m_settings.Visuals_Misc_Time);
			}
			else
			{
				convaradmin->SetAdminTime(-1);
			}
		}
		if (ctx.m_settings.Visuals_Misc_ChangeTime)
		{
			if (UII::Controls::SliderInt("Time", &ctx.m_settings.Visuals_Misc_Time, 0, 24))
			{
				std::shared_ptr<ConvarAdmin> convaradmin = std::make_shared<ConvarAdmin>();
				convaradmin->SetAdminTime(ctx.m_settings.Visuals_Misc_Time);
			}
		}*/
	//	UII::Controls::Toggle("Clear Vision in water", "Provides Clear vision underwater", &ctx.m_settings.Visuals_Misc_ClearWaterVision);
		UII::Controls::Toggle("Force Admin", "Use this to change the time of day on latest version", &ctx.m_settings.Visuals_Misc_ForceAdminFlag);
		UII::Controls::Toggle("Bright Nights", "Spam Turn On And Off Till It Works", &ctx.m_settings.Visuals_Misc_BrightNight);
		UII::Controls::Toggle("Bright Cave", "Bright Caves!", &ctx.m_settings.Visuals_Misc_BrightCave);
		UII::Controls::Toggle("Clothing ADS Blocker", "Disables Pot Armour Prenting You Aim", &ctx.m_settings.Visuals_Misc_ADSBlocker);

		//did this get removed?
		//UII::Controls::Toggle("Admin Flag", "(Will break noclip in build servers)", &ctx.m_settings.Visuals_Misc_AdminFlag);
	/*	UII::Controls::Toggle("Zoom", "zoomies", &ctx.m_settings.Visuals_Misc_Zoom);
		if (ctx.m_settings.Visuals_Misc_Zoom)
		{
			UII::Controls::SliderInt("Zoom Amount", &ctx.m_settings.Visuals_Misc_ZoomAmount, 1, 100);
			UII::Controls::Hotkey("Zoom Key", &ctx.m_settings.Visuals_Misc_ZoomKey);
		}*/

		UII::Controls::Toggle("Hotbar HUD", "Draws Target Hotbar", &ctx.m_settings.Visuals_Misc_TargetHotbar);
		UII::Controls::Toggle("Clothes HUD", "Draws Target Clothes", &ctx.m_settings.Visuals_Misc_TargetClothes);
		UII::Controls::Toggle("Team HUD", "Draws Target Team", &ctx.m_settings.Visuals_Misc_TargetTeam);
		UII::Controls::Toggle("World HUD", "Draws World Information", &ctx.m_settings.Visuals_Misc_WorldHud);
		UII::Controls::ColorPicker("Hud Text Colour", "", &ctx.m_settings.Visuals_Misc_Hud_TextColour);
		UII::Controls::Toggle("HUD Background", "Draws A Back Ground Around Huds", &ctx.m_settings.Visuals_Misc_HudBackground);
		if(ctx.m_settings.Visuals_Misc_HudBackground)
		UII::Controls::ColorPicker("Hud Background Colour", "", &ctx.m_settings.Visuals_Misc_Hud_BackgroundColour);
	}
	UII::Controls::endSection();

	UII::Controls::nextSection();

	UII::Controls::beginSection("Overlay", ImVec2(250, 283 - 5));
	{
		UII::Controls::Button("Info!", "Shift+WinKey+ArrowKey To Change Overlay Window!");
		UII::Controls::Button("Info!", "If ESP Shakes, Disable Nvidia DLSS In Settings!");
		UII::Controls::Toggle("Override W2S Resolution", "Overrides W2S Resolution", &ctx.m_settings.Overlay_OverRideResolution);
		UII::Controls::InputInt("Screen Width", &ctx.m_settings.Overlay_ScreenWidth);
		UII::Controls::InputInt("Screen Height", &ctx.m_settings.Overlay_ScreenHeight);
		UII::Controls::Toggle("Transparent", "Sets Overlay to transparent. DON'T 1PC THE TRANSPARENT", &ctx.m_settings.Overlay_Transparent);
		if (ctx.m_settings.Overlay_Transparent)
			UII::Controls::Toggle("DON'T USE TRANSPARENT ON MAIN PC!!!", "DON'T USE THIS ON YOUR GAME PC!!!!!!", &ctx.m_settings.Overlay_Transparent);
		UII::Controls::Toggle("Crosshair", "Draws Cricle In Centre Of Screen", &ctx.m_settings.Visuals_Misc_Crosshair);
		if (ctx.m_settings.Visuals_Misc_Crosshair)
		{
			UII::Controls::InputInt("Crosshair Size", &ctx.m_settings.Visuals_Misc_Crosshair_Size);
			UII::Controls::ColorPicker("Crosshair Colour", "Crosshair Color editer", &ctx.m_settings.Visuals_Misc_Crosshair_Colour);
		}
		UII::Controls::Toggle("Bold Fonts", "Change Fuser font", &ctx.m_settings.Overlay_BoldFont);
		UII::Controls::Toggle("VSync", "Toggle VSync", &ctx.m_settings.Overlay_Vsync);
		
	
		//UII::Controls::Toggle("Improved VM Update", "Requires Restart! Makes Esp More Sticky", &ctx.m_settings.Misc_MultiThreadedViewUpdate);
		//if (ctx.m_settings.Misc_MultiThreadedViewUpdate.changed())
	//		ctx.m_settings.save("Default.conf");
		UII::Controls::Hotkey("Revert Transparency Key", &ctx.m_settings.Overlay_RevertKey);
	}
	UII::Controls::endSection();

	UII::Controls::sameSection();

	UII::Controls::beginSection("Battle Mode", ImVec2(250, 163));
	{
		UII::Controls::Toggle("Enable", "Turn on / Off", &ctx.m_settings.Battlemode_Enable);
		UII::Controls::Toggle("Battlemode Resource Saver", "Prevents None Player Esp Updating/Caching When Battlemode Is On", &ctx.m_settings.Battlemode_ResourceSaver);
		UII::Controls::Hotkey("Toggle Key", &ctx.m_settings.Battlemode_key);
		UII::Controls::Toggle("BattleMode Indicator", "Toggle The Indicator", &ctx.m_settings.Battlemode_Indicator);
	}
	UII::Controls::endSection();
}

void c_draw::Important()
{
	UII::Controls::beginSection("Important");
	{
		UII::Controls::Toggle("Exception Catching", "Requires Restart! Toggle Off To Prevent Players Not Showing", &ctx.m_settings.Misc_ExceptionCatching);
		if (ctx.m_settings.Misc_ExceptionCatching.changed())
			ctx.m_settings.save("Default.conf");

		
			mem.WriteHeapCheck(ctx.m_settings.Misc_HeapMemoryCheck);
		UII::Controls::Toggle("Write Only To Heaped Memory Check", "Wont Work For Some Cards. Extra Write Memory Check", &ctx.m_settings.Misc_HeapMemoryCheck);
		if (ctx.m_settings.Misc_HeapMemoryCheck.changed())
			ctx.m_settings.save("Default.conf");

		UII::Controls::Toggle("Disable ESP", "Stops Fuser ESP Drawing", &ctx.m_settings.Misc_DisableESP);
		UII::Controls::Toggle("Cpu Saver", "Reduces Activity Of Cheat To Save CPU Usage", &ctx.m_settings.Misc_PerformanceSleep);
		if(ctx.m_settings.Misc_PerformanceSleep)
			UII::Controls::SliderInt("Saver Amount", &ctx.m_settings.Misc_SleepAmount, 1, 10);
	}
	UII::Controls::endSection();
	UII::Controls::nextSection();
	UII::Controls::beginSection("Other");
	{
		UII::Controls::Combo("Font", { "Verdana","Tahoma","Arial","Comic",}, ctx.m_settings.Misc_Font);
		if (ctx.m_settings.Misc_Font.changed())
		{
			ctx.m_settings.save("Default.conf");
		//	Font::initializeFonts();
		}
		UII::Controls::Button("Restart Cheat For Font To Change", "");

		UII::Controls::Toggle("Font Size Override", "Sets All Font Sizes To Set Size", &ctx.m_settings.Misc_Font_Override);
		UII::Controls::SliderInt("Font Size", &ctx.m_settings.Misc_Font_Override_Size, 1, 100);
		
	}
	UII::Controls::endSection();
}

void c_draw::DrawString(std::string text, ImVec2 pos, ImColor color, int fontSize, FontAlignment alignment)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	if (fontSize == 0)
		fontSize = ImGui::GetFontSize();
	ImFont* font = Font::overlayFont;
	if (ctx.m_settings.Overlay_BoldFont)
		font = Font::overlayFontBold;

	ImVec2 txtSize = ImGui::CustomCalcTextSize(text.c_str(), 0, false, -1, fontSize, font);
	// set the position
	switch (alignment)
	{
	case FontAlignment::Centre:
		pos.x -= (txtSize.x / 2);
		break;
	case FontAlignment::Right:
		pos.x += txtSize.x;
		break;
	case FontAlignment::Left:
		pos.x -= txtSize.x;
		break;
	case FontAlignment::None:
		break;
	case FontAlignment::CentreCentre:
		pos.x -= (txtSize.x / 2);
		pos.y -= (txtSize.y / 2);
		break;
	case FontAlignment::CentreLeft:
		pos.x -= txtSize.x;
		pos.y += (txtSize.y / 2);
		break;
	case FontAlignment::CentreRight:
		pos.x += txtSize.x;
		pos.y -= (txtSize.y / 2);
		break;
	}

	draw_list->AddText(font, fontSize, pos, color, text.c_str());
}

void c_draw::DrawRect(ImVec2 min, int width, int height, float thickness, ImColor color)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddRect(min, min + ImVec2(width, height), color, 0.0f, 0, thickness);
}

void c_draw::DrawFilledRect(ImVec2 min, int width, int height, ImColor color)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddRectFilled(min, min + ImVec2(width, height), color, 0.0f, 0);
}

void c_draw::DrawLine(ImVec2 p1, ImVec2 p2, ImColor color, float thickness)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddLine(p1, p2, color, thickness);
}

void c_draw::DrawCircle(ImVec2 center, float raduis, ImColor color, int segments, float thickness)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddCircle(center, raduis, color, segments, thickness);
}

void c_draw::DrawFilledCircle(ImVec2 center, float raduis, ImColor color, int segments)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddCircleFilled(center, raduis, color, segments);
}
