#include "Pch.h"
#include "BaseNetworkable.h"

#include "Globals.h"
#include "Networkable.h"
#include "Item.h"
#include "xorstr.hpp"
#include "Misc/vector.h"
#include "context.h"
#include "Encryption.h"
inline int FailCount;

BaseNetworkable::BaseNetworkable()
{
	/*uint64_t networkable = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
	printf("[BaseNetworkable] BaseNetworkable: 0x%llX\n", networkable);
	this->StaticField = mem.Read<uint64_t>(networkable + StaticFieldOffset); // Set Static Padding
	printf("[BaseNetworkable] Static Fields: 0x%llX\n", StaticField);
	this->ListParent = mem.Read<uint64_t>(StaticField + ListParent); // Set Static Padding
	printf("[BaseNetworkable] ListParent: 0x%llX\n", ListParent);
	this->DecryptedListParent = Encryption.load()->DecryptBaseNetworkable(ListParent);
	printf("[BaseNetworkable] DecryptedListParent: 0x%llX\n", DecryptedListParent);
	this->EntityList = Encryption.load()->DecryptBaseNetworkable(mem.Read<uint64_t>(DecryptedListParent + EntityListOffset));
	printf("[BaseNetworkable] EntityList: 0x%llX\n", EntityList);*/
/*	printf("[BaseNetworkable] Static Fields: 0x%llX\n", StaticField);
	this->ClientEntities = mem.Read<uint64_t>(StaticField + ClientEntitiesOffset);
	printf("[BaseNetworkable] ClientEntities: 0x%llX\n", ClientEntities);
	this->EntityList = mem.Read<uint64_t>(ClientEntities + EntityListOffset); // entity realm
	printf("[BaseNetworkable] EntityList: 0x%llX\n", EntityList);
	this->BufferList = mem.Read<uint64_t>(EntityList + BufferListOffset);
	printf("[BaseNetworkable] BufferList: 0x%llX\n", BufferList);
	ItterateEntities();*/
	BufferList = mem.Read<uint64_t>(Encryption.load()->GetBaseNetworkableList() + 0x28);
	this->ObjectListSize = mem.Read<uint32_t>(BufferList + ObjectListSizeOffset);
	this->ObjectList = mem.Read<uint64_t>(BufferList + ObjectListOffset);

	//printf("[BaseNetworkable] ObjectList: 0x%llX\n", ObjectList);
	//printf("[BaseNetworkable] ObjectListSize: %d\n", ObjectListSize);
	ItterateEntities();
}

struct PrefabName
{
	char name[256];
};

std::vector<std::shared_ptr<NetworkableObject>> BaseNetworkable::GetNetworkables()
{
	return Networkables;
}

int BaseNetworkable::GetListSize()
{
	return ListSize;
}

std::unordered_map<uint64_t, std::string> DroppedItemNames;

void BaseNetworkable::ItterateEntities()
{
	if (ctx.m_settings.Battlemode_Enable && ctx.m_settings.Battlemode_ResourceSaver)
		return;

	FailCount = 0;
	if (BaseLocalPlayer.load() == nullptr)
	{
		ListSize = 0;
		return;
	}
	std::vector<uint64_t> basenetworkableobjects;

	this->ObjectListSize = mem.Read<uint32_t>(BufferList + ObjectListSizeOffset);
	//printf("[BaseNetworkable] ObjectListSize: %d\n", ObjectListSize);
	this->ObjectList = mem.Read<uint64_t>(BufferList + ObjectListOffset);
	//printf("[BaseNetworkable] ObjectList: 0x%llX\n", ObjectList);

	ListSize = ObjectListSize;
	//printf("[BaseNetworkable] ObjectList: 0x%llX\n", ObjectList);

	auto handle = mem.CreateScatterHandle();
	if (ObjectListSize > 150000)
	{
		OreMutex.lock();
		CollectableMutex.lock();
		DroppedItemsMutex.lock();
		LootableMutex.lock();
		DeployableMutex.lock();
		AnimalMutex.lock();
		AttackEntitiesMutex.lock();
		VehicleMutex.lock();
		Encryption.load()->Update();
		BufferList = mem.Read<uint64_t>(Encryption.load()->GetBaseNetworkableList() + 0x28);
		this->ObjectListSize = mem.Read<uint32_t>(BufferList + ObjectListSizeOffset);
		this->ObjectList = mem.Read<uint64_t>(BufferList + ObjectListOffset);
		ListSize = ObjectListSize;

		OreMutex.unlock();
		CollectableMutex.unlock();
		DroppedItemsMutex.unlock();
		LootableMutex.unlock();
		DeployableMutex.unlock();
		AnimalMutex.unlock();
		AttackEntitiesMutex.unlock();
		VehicleMutex.unlock();
	}
	basenetworkableobjects.resize(ObjectListSize);
	std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(ObjectListSize);
	mem.Read(ObjectList + 0x20, object_raw_ptr.get(), ObjectListSize * sizeof(uint64_t));
	for (size_t i = 0; i < ObjectListSize; i++)
	{
		basenetworkableobjects[i] = object_raw_ptr[i];
	}

	std::vector<uint64_t> prefablist;
	prefablist.resize(ObjectListSize);
	std::vector<PrefabName> namelist;
	namelist.resize(ObjectListSize);
	std::vector<uint64_t> objectlist;
	objectlist.resize(ObjectListSize);
	std::vector<WORD> taglist;
	taglist.resize(ObjectListSize);
	std::vector<uint64_t> baseobjectlist;
	baseobjectlist.resize(ObjectListSize);
	//std::cout << "[BaseNetworkable] Reading Object Addresses" << std::endl;

	for (int i = 0; i < ObjectListSize; i++)
	{
		if (basenetworkableobjects[i] == 0)
			continue;
		uint64_t currentobject = basenetworkableobjects[i];
		if (currentobject == 0)
			continue;
		mem.AddScatterReadRequest(handle, currentobject + 0x10, reinterpret_cast<void*>(&baseobjectlist[i]), sizeof(uint64_t));
	}
	mem.ExecuteReadScatter(handle);
	for (int i = 0; i < ObjectListSize; i++)
	{
		if (basenetworkableobjects[i] == 0)
			continue;
		uint64_t currentobject = basenetworkableobjects[i];
		if (currentobject == 0)
			continue;
		mem.AddScatterReadRequest(handle, baseobjectlist[i] + 0x30, reinterpret_cast<void*>(&objectlist[i]), sizeof(uint64_t));
	}
	mem.ExecuteReadScatter(handle);
	//std::cout << "[BaseNetworkable] Reading Prefab Addresses" << std::endl;
	for (int i = 0; i < ObjectListSize; i++)
	{
		if (objectlist[i] == 0)
			continue;
		mem.AddScatterReadRequest(handle, objectlist[i] + 0x60, reinterpret_cast<void*>(&prefablist[i]), sizeof(uint64_t));
		mem.AddScatterReadRequest(handle, objectlist[i] + 0x54, reinterpret_cast<void*>(&taglist[i]), sizeof(WORD));
	}
	mem.ExecuteReadScatter(handle);

	//std::cout << "[BaseNetworkable] Reading Prefab Names" << std::endl;
	for (int i = 0; i < ObjectListSize; i++)
	{
		uint64_t prefab = prefablist[i];
		if (prefab == 0)
			continue;
		mem.AddScatterReadRequest(handle, prefab, reinterpret_cast<void*>(&namelist[i].name), 256);
	}
	mem.ExecuteReadScatter(handle);
	std::vector<std::shared_ptr<NetworkableObject>> networkables;
//	LocalPlayerOffset = objectlist[0];
	//printf(xorstr_("LocalPlayerOffset: 0x%llX\n"), LocalPlayerOffset);
	for (int i = 0; i < ObjectListSize; i++)
	{
		
		std::string prefabname = namelist[i].name;
		
		if (prefabname.size() == 0)
			continue;

	//if(prefabname == xorstr_("LocalPlayer"))
	//LocalPlayerOffset = objectlist[i];
		//int tag = taglist[i];
	//	if (tag == 6)
			//printf("[BaseNetworkable] LocalPlayer: %s\n", prefabname.c_str());
			//continue;
		//if (NetworkableMap.contains(objectlist[i]))
		//	{
		//		networkables.push_back(NetworkableMap[objectlist[i]]); // skip some reads
		//	continue;
		//}
		// ore

		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores/metal-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Metal, handle, xorstr_("MetalNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores/sulfur-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Sulphur, handle, xorstr_("SulphurNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores/stone-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Stone, handle, xorstr_("StoneNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores_sand/metal-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Metal, handle, xorstr_("MetalNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores_sand/sulfur-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Sulphur, handle, xorstr_("SulphurNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores_sand/stone-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Stone, handle, xorstr_("StoneNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores_snow/metal-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Metal, handle, xorstr_("MetalNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores_snow/sulfur-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Sulphur, handle, xorstr_("SulphurNode")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/ores_snow/stone-ore.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Stone, handle, xorstr_("StoneNode")));
			continue;
		}
		// collectable
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/stone/metal-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MetalCollectable, handle,xorstr_("metal.ore")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/stone/sulfur-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SulphurCollectable, handle, xorstr_("sulfur.ore")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/stone/stone-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::StoneCollectable, handle, xorstr_("Stones")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Hemp, handle, xorstr_("clone.hemp")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/wood/wood-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Wood, handle, xorstr_("Wood")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/hemp/hemp.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Hemp, handle, xorstr_("clone.hemp")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/berry-yellow/berry-yellow-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::YellowBerry, handle, xorstr_("clone.yellow.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/berry-blue/berry-blue-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::BlueBerry, handle, xorstr_("clone.blue.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/berry-red/berry-red-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::RedBerry, handle, xorstr_("clone.red.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/potato/potato-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Potato, handle, xorstr_("potato")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/berry-green/berry-green-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::GreenBerry, handle, xorstr_("clone.green.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/berry-white/berry-white-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::WhiteBerry, handle, xorstr_("clone.white.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/berry-black/berry-black-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::BlackBerry, handle, xorstr_("clone.black.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/berrry/black/black_berry.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::BlackBerry, handle, xorstr_("clone.black.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/berrry/red/red_berry.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::RedBerry, handle, xorstr_("clone.red.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/berrry/white/white_berry.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::WhiteBerry, handle, xorstr_("clone.white.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/berrry/blue/blue_berry.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::BlueBerry, handle, xorstr_("clone.blue.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/berrry/green/green_berry.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::GreenBerry, handle, xorstr_("clone.green.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/berrry/yellow/yellow_berry.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::YellowBerry, handle, xorstr_("clone.yellow.berry")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/potato/potato.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Potato, handle, xorstr_("potato")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/pumpkin/pumpkin.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Pumpkin, handle, xorstr_("pumpkin")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/plants/corn/corn.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Corn, handle, xorstr_("corn")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/corn/corn-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Corn, handle, xorstr_("corn")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Pumpkin, handle, xorstr_("pumpkin")));
			continue;
		}
		if (prefabname.find(xorstr_("mushroom-cluster")) != std::string::npos)
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Mushroom, handle, xorstr_("mushroom")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/loot/loot-barrel-1.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Barrel, handle, xorstr_("bluebarrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/autospawn/resource/loot/loot-barrel-2.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Barrel, handle, xorstr_("sandbarrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/loot_barrel_2.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Barrel, handle, xorstr_("sandbarrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/loot_barrel_1.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Barrel, handle, xorstr_("bluebarrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/oil_barrel.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FuelBarrel, handle, xorstr_("redbarrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/oil_barrel.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FuelBarrel, handle, xorstr_("redbarrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_normal_2.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::NormalCrate, handle, xorstr_("cratecostume")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_normal_2.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::NormalCrate, handle, xorstr_("cratecostume")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_normal.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MilitaryCrate, handle, xorstr_("MilitaryCrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_normal.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MilitaryCrate, handle, xorstr_("MilitaryCrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_elite.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::EliteCrate, handle, xorstr_("elitecrate"))); 
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_normal_2_medical.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MedicalCrate, handle, xorstr_("MedicalCrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_normal_2_food.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FoodCrate, handle, xorstr_("FoodBox")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/foodbox.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FoodCrate, handle, xorstr_("FoodBox")));
			//	printf("[BaseNetworkable] DroppedItem: %s\n", prefabname.c_str());
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/minecart.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MineCart, handle, xorstr_("minecart.planter")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_tools.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::ToolBox, handle, xorstr_("ToolBox")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_tools.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::ToolBox, handle, xorstr_("ToolBox"))); 
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_medical.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MedicalCrate, handle, xorstr_("MedicalCrate"))); 
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_food_2.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FoodCrate, handle, xorstr_("FoodBox")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_food_1.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FoodCrate, handle,xorstr_("FoodBox")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_elite.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::EliteCrate, handle, xorstr_("elitecrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_ammunition.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::AmmoCrate, handle)); 
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/crate_fuel.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FuelCrate, handle));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_underwater_advanced.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::UnderWaterCrate, handle, xorstr_("box.wooden.large")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/crate_underwater_basic.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::UnderWaterCrate, handle, xorstr_("UnderwaterCrate"))); 
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/chinooklockedcrate/codelockedhackablecrate.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LockedCrate, handle,xorstr_("LockedCrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/chinooklockedcrate/codelockedhackablecrate_oilrig.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LockedCrate, handle, xorstr_("LockedCrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/patrol helicopter/heli_crate.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::HelicopterCrate, handle, xorstr_("elitecrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/m2bradley/bradley_crate.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::BradleyCrate, handle, xorstr_("elitecrate")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/small stash/small_stash_deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Stash, handle, xorstr_("stash.small")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/supply drop/supply_drop.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SupplyDrop, handle, "SupplyDrop"));
			continue;
		}
		if (prefabname == xorstr_("assets/content/structures/excavator/prefabs/diesel_collectable.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Diesel, handle, xorstr_("diesel_barrel")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/player/player_corpse_new.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Corpse, handle, xorstr_("Head Skull")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/player/player.corpse.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Corpse, handle, xorstr_("Head Skull")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/item drop/item_drop_backpack.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Corpse, handle, xorstr_("Head Skull")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/playerioents/electricfurnace/electricfurnace.io.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Furnace, handle, xorstr_("electric.furnace")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/furnace/furnace.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Furnace, handle, xorstr_("furnace")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/legacyfurnace/legacy_furnace.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Furnace, handle, xorstr_("furnace.legacy.skin")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/furnace.large/furnace.large.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LargeFurnace, handle, xorstr_("furnace.large")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/decor_dlc/storagebarrel/storage_barrel_c.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LargeBox, handle, xorstr_("storage_barrel_c")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/decor_dlc/storagebarrel/storage_barrel_b.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LargeBox, handle, xorstr_("storage_barrel_b")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/large wood storage/box.wooden.large.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LargeBox, handle, xorstr_("box.wooden.large")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/halloween/coffin/coffinstorage.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LargeBox, handle, xorstr_("coffin.storage")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/flame turret/flameturret.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::FlameTurret, handle, xorstr_("flameturret")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/sam_site_turret/sam_site_turret_deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SamSite, handle, xorstr_("samsite")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/bear trap/beartrap.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::BearTrap, handle, xorstr_("trap.bear")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/single shot trap/guntrap.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::ShotgunTrap, handle, xorstr_("guntrap")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/landmine/landmine.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LandMine, handle, xorstr_("trap.landmine")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/playerioents/generators/generator.small.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Battery, handle, xorstr_("electric.generator.small")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/playerioents/batteries/large/large.rechargable.battery.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Battery, handle, xorstr_("electric.battery.rechargable.large")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/playerioents/batteries/smallrechargablebattery.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Battery, handle, xorstr_("electric.battery.rechargable.small")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/playerioents/batteries/medium/medium.rechargable.battery.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Battery, handle, xorstr_("electric.battery.rechargable.medium")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/playerioents/generators/fuel generator/small_fuel_generator.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Battery, handle, xorstr_("electric.fuelgenerator.small")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/tool cupboard/cupboard.tool.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::ToolCupboard, handle, xorstr_("cupboard.tool")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/tool cupboard/retro/cupboard.tool.retro.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::ToolCupboard, handle, xorstr_("cupboard.tool")));
			continue;
		}
		if (prefabname.find(xorstr_("deployable/sleeping bag")) != std::string::npos)
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SleepingBag, handle, xorstr_("sleepingbag")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/bed/bed_deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SleepingBag, handle, xorstr_("bed")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/tier 1 workbench/workbench1.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Workbench1, handle, xorstr_("workbench1")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/tier 2 workbench/workbench2.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Workbench2, handle, xorstr_("workbench2")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/tier 3 workbench/workbench3.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Workbench3, handle, xorstr_("workbench3")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/oil refinery/refinery_small_deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::OilRefinery, handle, xorstr_("small.oil.refinery")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/static/recycler_static.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Recycler, handle, xorstr_("Recycler")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/decor_dlc/minecart planter/minecart.planter.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Planter, handle, xorstr_("minecart.planter")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/misc/decor_dlc/bath tub planter/bathtub.planter.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Planter, handle, xorstr_("bathtub.planter")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/planters/planter.large.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Planter, handle, xorstr_("planter.large")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/deployable/planters/planter.small.deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Planter, handle, xorstr_("planter.small")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/boar/boar.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Boar, handle, xorstr_("Boar")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/chicken/chicken.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Chicken, handle, xorstr_("Chicken")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/horse/horse.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Horse, handle, xorstr_("Horse")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/nextai/testridablehorse.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Horse, handle, xorstr_("Horse")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/stag/stag.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Stag, handle, xorstr_("Stag")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/wolf/wolf.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Wolf, handle, xorstr_("Wolf")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/bear/bear.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Bear, handle, xorstr_("Bear")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/bear/polarbear.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::PolarBear, handle, xorstr_("PolarBear")));
			continue;
		}
		if (prefabname == xorstr_("assets/rust.ai/agents/fish/simpleshark.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Shark, handle, xorstr_("Shark")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/m2bradley/bradleyapc.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Bradley, handle, xorstr_("Bradley")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::AttackHelicopter, handle, xorstr_("AttackHeli")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/trains/workcart/workcart.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Train, handle, xorstr_("WorkCart")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/trains/workcart/subents/workcart_fuel_storage.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Train, handle, xorstr_("WorkCart")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/trains/workcart/workcart_aboveground2.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Train, handle, xorstr_("WorkCart")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/trains/workcart/workcart_aboveground.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Train, handle, xorstr_("WorkCart")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/trains/locomotive/locomotive.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Train, handle, xorstr_("Locomotive")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/attackhelicopter/attackhelicopter.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::AttackMiniCopter, handle, xorstr_("AttackMinicopter")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/scrap heli carrier/scraptransporthelicopter.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::ScrapHeli, handle, xorstr_("ScrapHeli")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/minicopter/minicopter.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MiniCopter, handle, xorstr_("MiniCopter")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/vehicle/seats/modularcardriverseat.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Car, handle, xorstr_("Car")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/boats/tugboat/tugboat.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::TugBoat, handle, xorstr_("TugBoat")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/submarine/submarinesolo.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SmallSubmarine, handle, xorstr_("SmallSubmarine")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/submarine/submarineduo.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::LargeSubmarine, handle, xorstr_("LargeSubmarine")));

			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/boats/rowboat/rowboat.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::RowBoat, handle, xorstr_("Rowboat")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/boats/rhib/rhib.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::RHIB, handle, xorstr_("RHIB")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/snowmobiles/snowmobile.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::SnowMobile, handle,xorstr_("Snowmobile")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/tools/metaldetector/metal_detector_source.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::DetectorSource, handle, xorstr_("metal.detector")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/tools/metaldetector/metal_detector_flag.entity.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::DetectorSource, handle, xorstr_("metal.detector")));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/npc/autoturret/autoturret_deployed.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::AutoTurret, handle, xorstr_("autoturret")));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/tech_parts_2.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::TechCrate, handle));
			continue;
		}
		if (prefabname == xorstr_("assets/bundled/prefabs/radtown/underwater_labs/tech_parts_1.prefab"))
		{
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::TechCrate, handle));
			continue;
		}
		if (prefabname == xorstr_("assets/prefabs/vehicle/seats/bikedriverseat.prefab"))
		{

			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::PedalBike, handle, xorstr_("bicycle")));
			continue;
		}
	
		//assets/content/vehicles/bikes/motorbike_sidecar.prefab
		//assets/content/vehicles/bikes/motorbike.prefab
		if (prefabname == xorstr_("assets/content/vehicles/bikes/motorbike_sidecar.prefab"))
		{

			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::MotorbikeSidecar, handle, xorstr_("motorbike_sidecar")));
			continue;
		}
		if (prefabname == xorstr_("assets/content/vehicles/bikes/motorbike.prefab"))
		{

			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::Motorbike, handle, xorstr_("motorbike")));
			continue;
		}
		if (prefabname.find(xorstr_("(world)")) != std::string::npos)
		{
			std::string::size_type worldpos = prefabname.find(xorstr_("(world)"));
			if (worldpos != std::string::npos)
				prefabname.erase(worldpos - 1, 8);
			networkables.push_back(std::make_shared<NetworkableObject>(objectlist[i], ObjectType::DroppedItem, handle, prefabname));
		//	printf("[BaseNetworkable] DroppedItem: %s\n", prefabname.c_str());
			continue;
		}
		//printf("[BaseNetworkable] DroppedItem: %s\n", prefabname.c_str());
			//	std::cout << "[BaseNetworkable] Tag: " << tag << std::endl;
	}
	mem.ExecuteReadScatter(handle);
	for (std::shared_ptr<NetworkableObject> networkable : networkables)
	{
		if (!networkable->SetUp1Valid())
			continue;
		networkable->SetUp2(handle);
	}
	mem.ExecuteReadScatter(handle);

	for (std::shared_ptr<NetworkableObject> networkable : networkables)
	{
		if (!networkable->SetUp2Valid())
			continue;
		networkable->SetUp3(handle);
	}
	mem.ExecuteReadScatter(handle);
	for (std::shared_ptr<NetworkableObject> networkable : networkables)
	{
		if (!networkable->IsValid()) // add another check here for moving entities
			continue;
		networkable->SetPosition(handle);
	}
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);
	std::unordered_map<uint64_t, std::shared_ptr<NetworkableObject>> newmap;
	std::vector<std::shared_ptr<NetworkableObject>> oreslist;
	std::vector<std::shared_ptr<NetworkableObject>> collectablelist;
	std::vector<std::shared_ptr<NetworkableObject>> droppeditems;
	std::vector<std::shared_ptr<NetworkableObject>> lootableitems;
	std::vector<std::shared_ptr<NetworkableObject>> deployablelist;
	std::vector<std::shared_ptr<NetworkableObject>> animallist;
	std::vector<std::shared_ptr<NetworkableObject>> vehiclelist;
	for (std::shared_ptr<NetworkableObject> networkable : networkables)
	{
		if (networkable->IsValid() && networkable->GetPosition() != Vector3::Zero())
		{
			//	NetworkableMap[Networkable.load()->GetClass()] = networkable;
			ObjectType type = networkable->GetType();
			if (type == ObjectType::Stone || type == ObjectType::Metal || type == ObjectType::Sulphur)
			{
				oreslist.push_back(networkable);
			}
			if (type == ObjectType::StoneCollectable || type == ObjectType::MetalCollectable || type == ObjectType::SulphurCollectable || type == ObjectType::Wood
				|| type == ObjectType::Hemp || type == ObjectType::Pumpkin || type == ObjectType::Corn || type == ObjectType::Potato || type == ObjectType::BlackBerry
				|| type == ObjectType::BlueBerry || type == ObjectType::GreenBerry || type == ObjectType::RedBerry || type == ObjectType::WhiteBerry || type == ObjectType::BlackBerry
				|| type == ObjectType::YellowBerry|| type == ObjectType::Diesel || type == ObjectType::DetectorSource)
			{
				collectablelist.push_back(networkable);
			}
			if (type == ObjectType::DroppedItem)
			{
				droppeditems.push_back(networkable);
			}
			if (type == ObjectType::EliteCrate || type == ObjectType::NormalCrate || type == ObjectType::MilitaryCrate || type == ObjectType::MedicalCrate
				|| type == ObjectType::FoodCrate || type == ObjectType::ToolBox || type == ObjectType::AmmoCrate || type == ObjectType::FuelCrate || type == ObjectType::UnderWaterCrate
				|| type == ObjectType::BradleyCrate || type == ObjectType::HelicopterCrate || type == ObjectType::LockedCrate || type == ObjectType::SupplyDrop || type == ObjectType::Barrel
				|| type == ObjectType::FuelBarrel || type == ObjectType::Corpse || type == ObjectType::TechCrate)
			{
				lootableitems.push_back(networkable);
				//	printf("[BaseNetworkable] Lootable Item: %s\n", Networkable.load()->GetName().c_str());
			}
			if (type == ObjectType::Stash || type == ObjectType::SamSite || type == ObjectType::ToolCupboard || type == ObjectType::AutoTurret || type == ObjectType::Planter || type == ObjectType::LargeBox
				|| type == ObjectType::LargeFurnace || type == ObjectType::Workbench1 || type == ObjectType::Workbench2 || type == ObjectType::Workbench3 || type == ObjectType::ShotgunTrap || type == ObjectType::BearTrap
				|| type == ObjectType::FlameTurret || type == ObjectType::OilRefinery || type == ObjectType::Recycler || type == ObjectType::Battery || type == ObjectType::Furnace || type == ObjectType::SleepingBag
				|| type == ObjectType::LandMine)
			{
				deployablelist.push_back(networkable);
			}
			if (type == ObjectType::Boar || type == ObjectType::Horse || type == ObjectType::Stag || type == ObjectType::Boar || type == ObjectType::Chicken || type == ObjectType::Bear || type == ObjectType::PolarBear
				|| type == ObjectType::Shark || type == ObjectType::Wolf)
			{
				animallist.push_back(networkable);
			}

			if (type == ObjectType::AttackHelicopter || type == ObjectType::Bradley || type == ObjectType::Car || type == ObjectType::Train || type == ObjectType::MiniCopter || type == ObjectType::AttackMiniCopter
				|| type == ObjectType::ScrapHeli || type == ObjectType::TugBoat || type == ObjectType::RowBoat || type == ObjectType::RHIB || type == ObjectType::SnowMobile || type == ObjectType::LargeSubmarine || type == ObjectType::SmallSubmarine
				|| type == ObjectType::PedalBike || type == ObjectType::Motorbike|| type == ObjectType::MotorbikeSidecar)
			{
				vehiclelist.push_back(networkable);
			}
		}
	}
	//Networkables = networkables;

	std::lock_guard<std::mutex> lock(OreMutex);
	{
		OresList = oreslist;
	}
	std::lock_guard<std::mutex> lock1(CollectableMutex);
	{
		CollectableList = collectablelist;
	}
	std::lock_guard<std::mutex> lock2(DroppedItemsMutex);
	{
		DroppedItems = droppeditems;
	}
	std::lock_guard<std::mutex> lock3(LootableMutex);
	{
		LootableList = lootableitems;
	}
	std::lock_guard<std::mutex> lock4(DeployableMutex);
	{
		DeployableList = deployablelist;
	}
	std::lock_guard<std::mutex> lock5(AnimalMutex);
	{
		AnimalList = animallist;
	}
	std::lock_guard<std::mutex> lock6(VehicleMutex);
	{
		VehicleList = vehiclelist;
	}
}

void BaseNetworkable::UpdateAnimalPositions(VMMDLL_SCATTER_HANDLE handle)
{
	if (ctx.m_settings.Battlemode_Enable && ctx.m_settings.Battlemode_ResourceSaver)
		return;
	AnimalMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : AnimalList)
	{
		if (!networkable->IsValid()) // add another check here for moving entities
			continue;
		networkable->SetPosition(handle);
	}
	AnimalMutex.unlock();
}

void BaseNetworkable::UpdateVehiclePositions(VMMDLL_SCATTER_HANDLE handle)
{
	if (ctx.m_settings.Battlemode_Enable && ctx.m_settings.Battlemode_ResourceSaver)
		return;

	auto currenttime = std::chrono::steady_clock::now();

	VehicleMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : VehicleList)
	{
		if (!networkable->IsValid()) // add another check here for moving entities
			continue;
		// print position
		networkable->SetLastPosition(networkable->GetPosition());
		networkable->SetLastPositionUpdate();
		networkable->SetPosition(handle);
	}

	VehicleMutex.unlock();
}

void BaseNetworkable::UpdateNearbyEntities()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (ObjectListSize == 0)
		return;
	if (ctx.m_settings.Battlemode_Enable && ctx.m_settings.Battlemode_ResourceSaver)
		return;
	//printf("[BaseNetworkable] Updating Nearby Entities\n");
	std::vector<uint64_t> newobjects;
	this->ObjectListSize = mem.Read<uint32_t>(BufferList + ObjectListSizeOffset);
	//printf("[BaseNetworkable] ObjectListSize: %d\n", ObjectListSize);
	newobjects.resize(ObjectListSize);
	auto handle = mem.CreateScatterHandle();

	std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(ObjectListSize);
	mem.Read(ObjectList + 0x20, object_raw_ptr.get(), ObjectListSize * sizeof(uint64_t));
	for (size_t i = 0; i < ObjectListSize; i++)
	{
		newobjects[i] = object_raw_ptr[i];
	}

	std::vector<uint64_t> baseobjectlist;
	baseobjectlist.resize(ObjectListSize);

	for (int i = 0; i < ObjectListSize; i++)
	{
		if (newobjects[i] == 0)
			continue;
		uint64_t currentobject = newobjects[i];
		if (currentobject == 0)
			continue;
		mem.AddScatterReadRequest(handle, currentobject + 0x10, reinterpret_cast<void*>(&baseobjectlist[i]), sizeof(uint64_t));
	}
	mem.ExecuteReadScatter(handle);

	std::vector<uint64_t> objectlist;
	objectlist.resize(ObjectListSize);
	for (int i = 0; i < ObjectListSize; i++)
	{
		if (newobjects[i] == 0)
			continue;
		uint64_t currentobject = newobjects[i];
		if (currentobject == 0)
			continue;
		mem.AddScatterReadRequest(handle, baseobjectlist[i] + 0x30, reinterpret_cast<void*>(&objectlist[i]), sizeof(uint64_t));
	}
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);

	std::vector<std::shared_ptr<NetworkableObject>> neworelist;
	OreMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : OresList)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			neworelist.push_back(networkable);
		}
	}

	OresList = neworelist;
	OreMutex.unlock();
	std::vector<std::shared_ptr<NetworkableObject>> collectablelist;
	CollectableMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : CollectableList)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			collectablelist.push_back(networkable);
		}
	}

	CollectableList = collectablelist;
	CollectableMutex.unlock();
	std::vector<std::shared_ptr<NetworkableObject>> droppeditemlist;
	DroppedItemsMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : DroppedItems)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			droppeditemlist.push_back(networkable);
		}
	}

	DroppedItems = droppeditemlist;
	DroppedItemsMutex.unlock();
	std::vector<std::shared_ptr<NetworkableObject>> lootableitems;
	LootableMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : LootableList)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			lootableitems.push_back(networkable);
		}
	}

	LootableList = lootableitems;
	LootableMutex.unlock();
	std::vector<std::shared_ptr<NetworkableObject>> deployablelist;
	DeployableMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : DeployableList)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			deployablelist.push_back(networkable);
		}
	}

	DeployableList = deployablelist;
	DeployableMutex.unlock();
	std::vector<std::shared_ptr<NetworkableObject>> animallist;
	AnimalMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : AnimalList)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			animallist.push_back(networkable);
		}
	}

	AnimalList = animallist;
	AnimalMutex.unlock();
	std::vector<std::shared_ptr<NetworkableObject>> vehiclelist;
	VehicleMutex.lock();
	for (std::shared_ptr<NetworkableObject> networkable : VehicleList)
	{
		if (std::find(objectlist.begin(), objectlist.end(), networkable->GetClass()) != objectlist.end())
		{
			//	printf("[BaseNetworkable] Removing Networkable: 0x%llX\n", Networkable.load()->GetClass());
			vehiclelist.push_back(networkable);
		}
	}

	VehicleList = vehiclelist;
	VehicleMutex.unlock();
}
