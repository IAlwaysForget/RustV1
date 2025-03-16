#include "Pch.h"
#include "Networkable.h"
#include "Globals.h"
// we did this to save reads, in reality i think reading the actual name would be more optimal
std::unordered_map<std::string, std::string> ItemResolvedNames = {
	{xorstr_("pistol.revolver"),xorstr_("Revolver")},
	{xorstr_("shotgun.waterpipe"),xorstr_("WaterPipe Shotgun")},
	{xorstr_("rifle.ak.diver"),xorstr_("Assault Rifle")},
	{xorstr_("rifle.ak.ice"),xorstr_("Assault Rifle")},
	{xorstr_("rifle.bolt"),xorstr_("Bolt Action Rifle")},
	{xorstr_("rocket.launcher"),xorstr_("Rocket Launcher")},
	{xorstr_("bow.compound"),xorstr_("Compound Bow")},
	{xorstr_("pistol.prototype17"),xorstr_("ProtoType17")},
	{xorstr_("smg.2"),xorstr_("Custom SMG")},
	{xorstr_("shotgun.double"),xorstr_("Double Barrel Shotgun")},
	{xorstr_("pistol.Eoka"),xorstr_("Eoka")},
	{xorstr_("multiplegrenadelauncher"),xorstr_("Grenade Launcher")},
	{xorstr_("smg.mp5"),xorstr_("MP5")},
	{xorstr_("rifle.sks"),xorstr_("SKS")},
	{xorstr_("pistol.m92"),xorstr_("M92")},
	{xorstr_("shotgun.m4"),xorstr_("M4")},
	{xorstr_("rifle.m39"),xorstr_("M39")},
	{xorstr_("lmg.m249"),xorstr_("M249")},
	{xorstr_("rifle.lr300"),xorstr_("LR300")},
	{xorstr_("rifle.l96"),xorstr_("L96")},
	{xorstr_("bow.hunting"),xorstr_("Bow")},
	{xorstr_("legacy bow"),xorstr_("Bow")},
	{xorstr_("hmlmg"),xorstr_("HMLMG")},
	{xorstr_("pistol.nailgun"),xorstr_("NailGun")},
	{xorstr_("shotgun.pump"),xorstr_("Pump Shotgun")},
	{xorstr_("pistol.python"),xorstr_("Python")},
	{xorstr_("pistol.semiauto"),xorstr_("Semi Pistol")},
	{xorstr_("rifle.ak"),xorstr_("Assault Rifle")},
	{xorstr_("shotgun.spas12"),xorstr_("Spas12")},
	{xorstr_("smg.thompson"),xorstr_("Thompson")},
	{xorstr_("rifle.semiauto"),xorstr_("Semi Rifle")},
	{xorstr_("metal.refined"),xorstr_("High Quality Metal")},
	{xorstr_("lowgradefuel"),xorstr_("Low Grade Fuel")},
	{xorstr_("diverhatchet"),xorstr_("Pickaxe")},
	{xorstr_("diverpickaxe"),xorstr_("Hatchet")},
	{xorstr_("lumberjack.pickaxe"),xorstr_("Pickaxe")},
	{xorstr_("lumberjack.hatchet"),xorstr_("Hatchet")},
	{xorstr_("concretehatchet"),xorstr_("Stone Pickaxe")},
	{xorstr_("concretepickaxe"),xorstr_("Stone Hatchet")},
	{xorstr_("stonehatchet"),xorstr_("Stone Hatchet")},
	{xorstr_("largemedkit"),xorstr_("MedKit")},
	{xorstr_("syringe.medical"),xorstr_("Medical Syringe")},
	{xorstr_("grenadelauncher.smoke"),xorstr_("40mm Smoke Grenade")},
	{xorstr_("ammo.rocket.basic"),xorstr_("Rocket")},
	{xorstr_("ammo.rocket.seeker"),xorstr_("Homing Rocket")},
	{xorstr_("ammo.rocket.fire"),xorstr_("Incendiary Rocket")},
	{xorstr_("ammo.grenadelauncher.he"),xorstr_("40MM HE Grenade")},
	{xorstr_("ammo.rocket.mlrs"),xorstr_("MLRS Rocket")},
	{xorstr_("ammo.rocket.hv"),xorstr_("HV Rocket")},
	{xorstr_("arrow.hv"),xorstr_("HV Arrow")},
	{xorstr_("arrow.bone"),xorstr_("Bone Arrow")},
	{xorstr_("arrow.wooden"),xorstr_("Wooden Arrow")},
	{xorstr_("arrow.fire"),xorstr_("Fire Arrow")},
	{xorstr_("rifle"),xorstr_("Rifle Ammo")},
	{xorstr_("pistol"),xorstr_("Pistol Ammo")},
	{xorstr_("shotgun.slug"),xorstr_("Slug")},
	{xorstr_("shotgun"),xorstr_("Buckshot")},
	{xorstr_("shotgun.fire"),xorstr_("Incendiary Shot")},
	{xorstr_("rifle.incendiary"),xorstr_("Incendiary Rifle Ammo")},
	{xorstr_("pistol.hv"),xorstr_("HV Pistol Ammo")},
	{xorstr_("rifle.hv"),xorstr_("HV Rifle Ammo")},
	{xorstr_("pistol.fire"),xorstr_("Incendiary Pistol Ammo")},
	{xorstr_("ammo.rocket.sam"),xorstr_("Sam Site Rocket")},
	{xorstr_("explosive.timed"),xorstr_("C4 Explosive")},
	{xorstr_("explosive.satchel"),xorstr_("Satchel")},
	{xorstr_("hazmatsuit.arcticsuit"),xorstr_("Artic Hazmat")},
	{xorstr_("attire.ninja.suit"),xorstr_("Ninja Suit")},
	{xorstr_("hazmatsuit.nomadsuit"),xorstr_("Hazmat")},
	{xorstr_("halloween.mummysuit"),xorstr_("Mummy Suit")},
	{xorstr_("attire.banditguard"),xorstr_("Hazmat")},
	{xorstr_("hazmatsuittwitch"),xorstr_("Hazmat")},
	{xorstr_("halloween.surgeonsuit"),xorstr_("Surgeon Suit")},
	{xorstr_("nightvisiongoggles"),xorstr_("Night Vision")},
	{xorstr_("hazmatsuit_scientist_nvgm"),xorstr_("Hazmat")},
	{xorstr_("hazmatsuit_scientist"),xorstr_("Hazmat")},
	{xorstr_("hazmatsuit_scientist_peacekeeper"),xorstr_("Hazmat")},
	{xorstr_("hazmatsuit"),xorstr_("Hazmat")},
	{xorstr_("hazmatsuit.lumberjack"),xorstr_("Hazmat")},
	{xorstr_("lumberjack hoodie"),xorstr_("Hoodie")},
	{xorstr_("metal.facemask.icemask"),xorstr_("Metal Facemask")},
	{xorstr_("metal.plate.torso"),xorstr_("Metal Chest Plate")},
	{xorstr_("metal.plate.torso.icevest"),xorstr_("Metal Chest Plate")},
	{xorstr_("draculacape"),xorstr_("Dracula Cape")},
	{xorstr_("deer.skull.mask"),xorstr_("Bone Mask")},
	{xorstr_("hazmatsuit.diver"),xorstr_("Hazmat")},
	{xorstr_("attire.egg.suit"),xorstr_("Egg Suit")},
	{xorstr_("shoes.boots"),xorstr_("Boots")},
	{xorstr_("hazmatsuit_scientist_arctic"),xorstr_("Hazmat")},
	{xorstr_("hat.wolf"),xorstr_("Wolf Headdress")},
	{xorstr_("sewingkit"),xorstr_("Sewing Kit")},
	{xorstr_("techparts"),xorstr_("Tech Trash")},
	{xorstr_("propanetank"),xorstr_("Propane Tank")},
	{xorstr_("riflebody"),xorstr_("Rifle Body")},
	{xorstr_("sheetmetal"),xorstr_("Sheet Metal")},
	{xorstr_("roadsigns"),xorstr_("Road Signs")},
	{xorstr_("metalspring"),xorstr_("Metal Springs")},
	{xorstr_("smgbody"),xorstr_("SMG Body")},
	{xorstr_("metalpipe"),xorstr_("Metal Pipe")},
	{xorstr_("metalblade"),xorstr_("Metal Blades")},
	{xorstr_("semibody"),xorstr_("Semi Body")},
	{ xorstr_("smallbackpack"),xorstr_("Small Backpack") },
	{ xorstr_("largebackpack"),xorstr_("Large Backpack") },
};
std::unordered_map<ObjectType, std::string> ObjectNames = {
	{ObjectType::Stone,xorstr_("Stone")},
	{ObjectType::Metal,xorstr_("Metal")},
	{ObjectType::Sulphur,xorstr_("Sulphur")},
	{ObjectType::StoneCollectable,xorstr_("Stone Collectable")},
	{ObjectType::MetalCollectable,xorstr_("Metal Collectable")},
	{ObjectType::SulphurCollectable,xorstr_("Sulphur Collectable")},
	{ObjectType::Wood,xorstr_("Wood")},
	{ObjectType::Hemp,xorstr_("Hemp")},
	{ObjectType::Pumpkin,xorstr_("Pumpkin")},
	{ObjectType::Corn,xorstr_("Corn")},
	{ObjectType::Mushroom,xorstr_("Mushroom")},
	{ObjectType::Potato,xorstr_("Potato")},
	{ObjectType::RedBerry,xorstr_("Red Berry")},
	{ObjectType::WhiteBerry,xorstr_("White Berry")},
	{ObjectType::BlackBerry,xorstr_("Black Berry")},
	{ObjectType::YellowBerry,xorstr_("Yellow Berry")},
	{ObjectType::BlueBerry,xorstr_("Blue Berry")},
	{ObjectType::GreenBerry,xorstr_("Green Berry")},
	{ObjectType::Barrel,xorstr_("Barrel")},
	{ObjectType::FuelBarrel,xorstr_("Fuel Barrel")},
	{ObjectType::Stash,xorstr_("Stash")},
	{ObjectType::Corpse,xorstr_("Corpse")},
	{ObjectType::LockedCrate,xorstr_("Locked Crate")},
	{ObjectType::SupplyDrop,xorstr_("Supply Drop")},
	{ObjectType::ToolCupboard,xorstr_("Tool Cupboard")},
	{ObjectType::EliteCrate,xorstr_("Elite Crate")},
	{ObjectType::NormalCrate,xorstr_("Normal Crate")},
	{ObjectType::MilitaryCrate,xorstr_("Military Crate")},
	{ObjectType::MedicalCrate,xorstr_("Medical Crate")},
	{ObjectType::FoodCrate,xorstr_("Food Crate")},
	{ObjectType::MineCart,xorstr_("Mine Cart")},
	{ObjectType::ToolBox,xorstr_("Tool Box")},
	{ObjectType::AmmoCrate,xorstr_("Ammo Crate")},
	{ObjectType::FuelCrate,xorstr_("Fuel Crate")},
	{ObjectType::Diesel,xorstr_("Diesel Fuel")},
	{ObjectType::UnderWaterCrate,xorstr_("Under Water Crate")},
	{ObjectType::HelicopterCrate,xorstr_("Helicopter Crate")},
	{ObjectType::BradleyCrate,xorstr_("Bradley Crate")},
	{ObjectType::SleepingBag,xorstr_("Sleeping Bag")},
	{ObjectType::LandMine,xorstr_("Land Mine")},
	{ObjectType::BearTrap,xorstr_("Bear Trap")},
	{ObjectType::ShotgunTrap,xorstr_("Shotgun Trap")},
	{ObjectType::AutoTurret,xorstr_("Auto Turret")},
	{ObjectType::FlameTurret,xorstr_("Flame Turret")},
	{ObjectType::SamSite,xorstr_("Sam Site")},
	{ObjectType::Planter,xorstr_("Planter")},
	{ObjectType::LargeBox,xorstr_("Large Box")},
	{ObjectType::Workbench1,xorstr_("Workbench 1")},
	{ObjectType::Workbench2,xorstr_("Workbench 2")},
	{ObjectType::Workbench3,xorstr_("Workbench 3")},
	{ObjectType::Furnace,xorstr_("Furnace")},
	{ObjectType::LargeFurnace,xorstr_("Large Furnace")},
	{ObjectType::OilRefinery,xorstr_("Oil Refinery")},
	{ObjectType::Recycler,xorstr_("Recycler")},
	{ObjectType::Battery,xorstr_("Battery")},
	{ObjectType::Bradley,xorstr_("Bradley")},
	{ObjectType::AttackHelicopter,xorstr_("Attack Helicopter")},
	{ObjectType::Bear,xorstr_("Bear")},
	{ObjectType::PolarBear,xorstr_("Polar Bear")},
	{ObjectType::Boar,xorstr_("Boar")},
	{ObjectType::Chicken,xorstr_("Chimken")},
	{ObjectType::Stag,xorstr_("Stag")},
	{ObjectType::Horse,xorstr_("Horse")},
	{ObjectType::Wolf,xorstr_("Doggo")},
	{ObjectType::Shark,xorstr_("Shark")},
	{ObjectType::MiniCopter,xorstr_("MiniCopter")},
	{ObjectType::AttackMiniCopter,xorstr_("Attack MiniCopter")},
	{ObjectType::ScrapHeli,xorstr_("Scrap Helicopter")},
	{ObjectType::Train,xorstr_("Train")},
	{ObjectType::Car,xorstr_("Car")},
	{ObjectType::TugBoat,xorstr_("Tug Boat")},
	{ObjectType::RowBoat,xorstr_("Row Boat")},
	{ObjectType::RHIB,xorstr_("RHIB")},
	{ObjectType::DetectorSource,xorstr_("Treasure")},
	{ObjectType::SnowMobile,xorstr_("Snowmobile")},
	{ObjectType::LargeSubmarine,xorstr_("Large Submarine")},
	{ObjectType::SmallSubmarine,xorstr_("Small Submarine")},
	{ObjectType::TechCrate,xorstr_("Tech Crate")},
		{ObjectType::PedalBike,xorstr_("Bicycle")},
	{ObjectType::Motorbike,xorstr_("Motorbike")},
	{ObjectType::MotorbikeSidecar,xorstr_("Motorbike(sidecar)")},
};
std::unordered_map<std::string, bool> ResourceNames = {
	{xorstr_("gunpowder"), true},
	{xorstr_("cloth"), true},
	{xorstr_("charcoal"), true},
	{xorstr_("metal.refined"), true},
	{xorstr_("metal.fragments"), true},
	{xorstr_("fat.animal"), true},
	{xorstr_("hq.metal.ore"), true},
	{xorstr_("sulfur"), true},
	{xorstr_("metal.ore"), true},
	{xorstr_("stones"), true},
	{xorstr_("scrap"), true},
	{xorstr_("wood"), true},
	{xorstr_("sulfur.ore"), true},
	{xorstr_("lowgradefuel"), true},
	{xorstr_("diesel_barrel"), true},
	{xorstr_("crude.oil"), true},
	{xorstr_("fertilizer"), true}
};

std::unordered_map<std::string, bool> ComponentNames = {
	{xorstr_("riflebody"), true},
	{xorstr_("sheetmetal"), true},
	{xorstr_("cctv.camera"), true},
	{xorstr_("roadsigns"), true},
	{xorstr_("metalspring"), true},
	{xorstr_("smgbody"), true},
	{xorstr_("fuse"), true},
	{xorstr_("propanetank"), true},
	{xorstr_("metalpipe"), true},
	{xorstr_("tarp"), true},
	{xorstr_("techparts"), true},
	{xorstr_("metalblade"), true},
	{xorstr_("gears"), true},
	{xorstr_("rope"), true},
	{xorstr_("semibody"), true},
	{xorstr_("targeting.computer"), true},
	{xorstr_("sewingkit"), true}
};
std::unordered_map<std::string, bool> ToolNames = {
	{xorstr_("diverhatchet"), true},
	{xorstr_("diverpickaxe"), true},
	{xorstr_("hatchet"), true},
	{xorstr_("lumberjack.pickaxe"), true},
	{xorstr_("axe.salvaged"), true},
	{xorstr_("icepick.salvaged"), true},
	{xorstr_("hammer.salvaged"), true},
	{xorstr_("fishingrod.handmade"), true},
	{xorstr_("lumberjack.hatchet"), true},
	{xorstr_("chainsaw"), true},
	{xorstr_("concretehatchet"), true},
	{xorstr_("pickaxe"), true},
	{xorstr_("jackhammer"), true},
	{xorstr_("concretepickaxe"), true},
	{xorstr_("stone.pickaxe"), true},
	{xorstr_("stonehatchet"), true}
};

std::unordered_map<std::string, bool> MedicalNames = {
	{xorstr_("syringe.medical"), true},
	{xorstr_("bandage"), true},
	{xorstr_("largemedkit"), true},
};

std::unordered_map<std::string, bool> ExplosiveNames = {
	{xorstr_("ammo.grenadelauncher.he"), true},
	{xorstr_("explosive.timed"), true},
	{xorstr_("explosives"), true},
	{xorstr_("explosive.satchel"), true},
	{xorstr_("supply.signal"), true},
	{xorstr_("grenade.beancan"), true},
	{xorstr_("grenade.f1"), true},
	{xorstr_("rifle.explosive"), true}

};
std::unordered_map<std::string, bool> FoodNames = {
	{xorstr_("pumpkin"), true},
	{xorstr_("wolfmeat.cooked"), true},
	{xorstr_("meat.pork.cooked"), true},
	{xorstr_("waterjug"), true},
	{xorstr_("chocolate"), true},
	{xorstr_("apple"), true},
	{xorstr_("jar.pickle"), true},
	{xorstr_("mushroom"), true},
	{xorstr_("horsemeat.raw"), true},
	{xorstr_("meat.boar"), true},
	{xorstr_("can.tuna"), true},
	{xorstr_("deermeat.raw"), true},
	{xorstr_("fish.raw"), true},
	{xorstr_("bearmeat.cooked"), true},
	{xorstr_("chicken.cooked"), true},
	{xorstr_("wolfmeat.raw"), true},
	{xorstr_("chicken.raw"), true},
	{xorstr_("bearmeat"), true},
	{xorstr_("humanmeat.cooked"), true},
	{xorstr_("horsemeat.cooked"), true},
	{xorstr_("deermeat.cooked"), true},
	{xorstr_("fish.cooked"), true},
	{xorstr_("can.beans"), true},
	{xorstr_("potato"), true},
	{xorstr_("fish.minnows"), true},
	{xorstr_("granolabar"), true},
	{xorstr_("corn"), true},
	{xorstr_("black.raspberries"), true},
	{xorstr_("blueberries"), true},
	{xorstr_("cactusflesh"), true},
	{xorstr_("black.berry"), true},
	{xorstr_("green.berry"), true},
	{xorstr_("red.berry"), true},
	{xorstr_("blue.berry"), true},
	{xorstr_("fish.yellowperch"), true},
	{xorstr_("fish.salmon"), true},
	{xorstr_("fish.sardine"), true},
	{xorstr_("fish.smallshark"), true},
	{xorstr_("fish.troutsmall"), true},
	{xorstr_("fish.anchovy"), true},
	{xorstr_("yellow.berry"), true},
	{xorstr_("white.berry"), true},
	{xorstr_("scraptea.pure"), true},
	{xorstr_("radiationremovetea.pure"), true},
	{xorstr_("oretea.pure"), true},
	{xorstr_("maxhealthtea.pure"), true},
	{xorstr_("healingtea.pure"), true},
	{xorstr_("oretea"), true},
	{xorstr_("radiationremovetea.advanced"), true},
	{xorstr_("woodtea.pure"), true},
	{xorstr_("radiationresisttea.advanced"), true},
	{xorstr_("woodtea.advanced"), true},
	{xorstr_("radiationremovetea"), true},
	{xorstr_("scraptea.advanced"), true},
	{xorstr_("scraptea"), true},
	{xorstr_("radiationresisttea.pure"), true},
	{xorstr_("woodtea"), true},
	{xorstr_("maxhealthtea"), true},
	{xorstr_("oretea.advanced"), true},
	{xorstr_("maxhealthtea.advanced"), true},
	{xorstr_("healingtea"), true},
	{xorstr_("radiationresisttea"), true},
	{xorstr_("healingtea.advanced"), true},
};

std::unordered_map<std::string, bool> ArmourNames = {
	{xorstr_("hazmatsuit.arcticsuit"), true},
	{xorstr_("attire.banditguard"), true},
	{xorstr_("attire.ninja.suit"), true},
	{xorstr_("pants"), true},
	{xorstr_("hazmatsuit.nomadsuit"), true},
	{xorstr_("roadsign.jacket"), true},
	{xorstr_("riot.helmet"), true},
	{xorstr_("diving.mask"), true},
	{xorstr_("halloween.mummysuit"), true},
	{xorstr_("tshirt"), true},
	{xorstr_("jacket.snow"), true},
	{xorstr_("attire.snowman.helmet"), true},
	{xorstr_("hazmatsuittwitch"), true},
	{xorstr_("diving.wetsuit"), true},
	{xorstr_("tactical.gloves"), true},
	{xorstr_("halloween.surgeonsuit"), true},
	{xorstr_("scarecrow.suit"), true},
	{xorstr_("wood.armor.pants"), true},
	{xorstr_("scarecrowhead"), true},
	{xorstr_("wood.armor.helmet"), true},
	{xorstr_("wood.armor.jacket"), true},
	{xorstr_("roadsign.kilt"), true},
	{xorstr_("roadsign.gloves"), true},
	{xorstr_("nightvisiongoggles"), true},
	{xorstr_("hazmatsuit_scientist_nvgm"), true},
	{xorstr_("hat.wolf"), true},
	{xorstr_("hazmatsuit_scientist"), true},
	{xorstr_("hazmatsuit_scientist_peacekeeper"), true},
	{xorstr_("hazmatsuit"), true},
	{xorstr_("metal.plate.torso"), true},
	{xorstr_("heavy.plate.pants"), true},
	{xorstr_("hazmatsuit.lumberjack"), true},
	{xorstr_("scientistsuit_heavy"), true},
	{xorstr_("attire.hide.boots"), true},
	{xorstr_("attire.hide.poncho"), true},
	{xorstr_("lumberjack hoodie"), true},
	{xorstr_("heavy.plate.helmet"), true},
	{xorstr_("hoodie"), true},
	{xorstr_("metal.facemask"), true},
	{xorstr_("heavy.plate.jacket"), true},
	{xorstr_("metal.facemask.icemask"), true},
	{xorstr_("metal.plate.torso.icevest"), true},
	{xorstr_("jacket"), true},
	{xorstr_("twitch.headset"), true},
	{xorstr_("mask.bandana"), true},
	{xorstr_("bone.armor.suit"), true},
	{xorstr_("jumpsuit.suit.blue"), true},
	{xorstr_("diving.fins"), true},
	{xorstr_("coffeecan.helmet"), true},
	{xorstr_("draculacape"), true},
	{xorstr_("hazmatsuit.diver"), true},
	{xorstr_("deer.skull.mask"), true},
	{xorstr_("attire.egg.suit"), true},
	{xorstr_("shoes.boots"), true},
	{xorstr_("hazmatsuit_scientist_arctic"), true},
	{xorstr_("bucket.helmet"), true},
	{xorstr_("ghostsheet"), true},
	{xorstr_("gingerbreadsuit"), true},
	{xorstr_("smallbackpack"), true},
	{xorstr_("largebackpack"), true},

};

ObjectType NetworkableObject::GetType()
{
	return Type;
}

void NetworkableObject::SetItemType()
{
	if (Name.size() == 0)
	{
		Item = ItemType::None;
		return;
	} // arrow.
	if ((Name.find(xorstr_("ammo.")) != std::string::npos || Name.find(xorstr_("arrow.")) != std::string::npos) && Name.find(xorstr_("rocket.")) == std::string::npos && Name != xorstr_("ammo.grenadelauncher.he") && Name != xorstr_("rifle.explosive"))
	{
		Item = ItemType::Ammo;
		Name.erase(0, 5); // remove the ammo
		return;
	}
	if ((Name.find(xorstr_("ammo.")) != std::string::npos && Name.find(xorstr_("rocket.")) != std::string::npos) || ExplosiveNames.contains(Name))
	{
		Item = ItemType::Explosives;
		return;
	}
	if (Name.find(xorstr_("pistol.")) != std::string::npos)
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name.find(xorstr_("rifle.")) != std::string::npos)
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name.find(xorstr_("shotgun.")) != std::string::npos)
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name.find(xorstr_("smg.")) != std::string::npos)
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name.find(xorstr_("lmg.")) != std::string::npos)
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name == xorstr_("rocket.launcher"))
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name == xorstr_("military flamethrower") || Name == xorstr_("minigun"))
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name == xorstr_("multiplegrenadelauncher"))
	{
		Item = ItemType::Gun;
		return;
	}
	if (Name == xorstr_("hmlmg"))
	{
	
		Item = ItemType::Gun;
		return;
	}
	if (ResourceNames.contains(Name))
	{
		Item = ItemType::Resources;
		return;
	}
	if (ToolNames.contains(Name))
	{
		Item = ItemType::Tool;
		return;
	}
	if (MedicalNames.contains(Name))
	{
		Item = ItemType::Medical;
		return;
	}
	if (FoodNames.contains(Name))
	{
		Item = ItemType::Food;
		return;
	}
	if (ArmourNames.contains(Name))
	{
		Item = ItemType::Armour;
		return;
	}
	if (ComponentNames.contains(Name))
	{
		Item = ItemType::Components;
		return;
	}
	Item = ItemType::Other;
}
std::string NetworkableObject::GetPrefabName()
{
	return PrefabName;
}
NetworkableObject::NetworkableObject(uint64_t address, ObjectType type, VMMDLL_SCATTER_HANDLE handle, std::string name)
{
	this->Class = address;
	DescriminatableClass = Class;
	Type = type;
	SetUp1(handle);
	PrefabName = name;
	Name = name;
	SetItemType();
	if (Item != ItemType::None)
	{
		Name = ResolveItemName();
	}
	
}

ItemType NetworkableObject::GetItemType()
{
	return Item;
}

void NetworkableObject::SetItemType(ItemType type)
{
	Item = type;
}

void capitalise(std::string& str)
{
	if (!str.empty())
	{
		str[0] = std::toupper(str[0]);
	}
}

std::string NetworkableObject::ResolveItemName()
{
	try
	{
		if (ItemResolvedNames.contains(Name))
		{
			return ItemResolvedNames[Name];
		}
		else
		{
			if (!Name.empty())
			{
				for (char& c : Name)
				{
					if (c == '_' || c == '.')
						c = ' ';
				}

				std::istringstream iss(Name);
				std::string word;
				std::string normalized;

				while (iss >> word)
				{
					capitalise(word);
					normalized += word + " ";
				}
				if (!normalized.empty())
					normalized.pop_back();

				return normalized;
			}
		}
	}
	catch (...)
	{
		return Name;
	}
}

void NetworkableObject::SetUp1(VMMDLL_SCATTER_HANDLE handle)
{
	if (Type == ObjectType::DroppedItem)
	{
		mem.Read(Class + ObjectOffset, reinterpret_cast<void*>(&Object), sizeof(uint64_t)); // fixes release mode
	}
	else
	{
		mem.AddScatterReadRequest(handle, Class + ObjectOffset, reinterpret_cast<void*>(&Object), sizeof(uint64_t));
	}
}

void NetworkableObject::SetName(std::string name)
{
	Name = name;
}

bool NetworkableObject::SetUp1Valid()
{
	return Object != 0;
}

bool NetworkableObject::SetUp2Valid()
{
	return TransformHost != 0;
}

void NetworkableObject::SetUp2(VMMDLL_SCATTER_HANDLE handle)
{
	mem.AddScatterReadRequest(handle, Object + TransformHostOffset, reinterpret_cast<void*>(&TransformHost), sizeof(uint64_t));
}

void NetworkableObject::SetUp3(VMMDLL_SCATTER_HANDLE handle)
{
	mem.AddScatterReadRequest(handle, TransformHost + TransformOffset, reinterpret_cast<void*>(&Transform), sizeof(uint64_t));
}

void NetworkableObject::SetPosition(VMMDLL_SCATTER_HANDLE handle)
{
	mem.AddScatterReadRequest(handle, Transform + TransformPosition, reinterpret_cast<void*>(&Position), sizeof(Vector3));
}

Vector3 NetworkableObject::GetPosition()
{
	return Position;
}

bool NetworkableObject::IsValid()
{
	return Class != 0x0 && TransformHost != 0 && Transform != 0;
}

uint64_t NetworkableObject::GetClass()
{
	return Class;
}

std::string NetworkableObject::GetName()
{
	return ObjectNames[Type];
}

std::string NetworkableObject::GetItemName()
{
	return Name; // This is already processed and resolved when the item is created
}

void NetworkableObject::SetLastPositionUpdate()
{
	LastPositionUpdate = std::chrono::high_resolution_clock::now();
}

Vector3 NetworkableObject::GetLastPosition()
{
	return LastPosition;
}

void NetworkableObject::SetLastPosition(Vector3 position)
{
	LastPosition = position;
}

std::chrono::time_point<std::chrono::high_resolution_clock> NetworkableObject::GetLastPositionUpdate()
{
	return LastPositionUpdate;
}

void NetworkableObject::AddNewVelocity(Vector3 velocity)
{
	if (velocity != Vector3::Zero() && Velocities.size() < 20)
		Velocities.push_back(velocity);
	else
		Velocities.clear();
}

Vector3 NetworkableObject::GetAverageVelocity()
{
	if (Velocities.size() == 0)
		return Vector3::Zero();
	Vector3 total;
	std::vector<Vector3> velocities = Velocities;
	for (Vector3 vec : velocities)
	{
		total = total + vec;
	}
	Vector3 average = total / velocities.size();
	return average;
}
