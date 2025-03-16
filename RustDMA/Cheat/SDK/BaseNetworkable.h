#pragma once
#include "BasePlayer.h"
#include "Networkable.h"
#include <mutex>

class BaseNetworkable
{
	// So we use the static basegamemode__C in order to then access basenetworkable through the inheritance chain as basenetworkable itself is in a guarded region
	// Not sure about these ones, I copied from a chain
	uint64_t BufferList = 0;
	uint64_t BufferListOffset = 0x28;
	uint64_t ObjectList = 0x0;
	uint64_t ObjectListOffset = 0x18;
	uint32_t ObjectListSize = 0;
	uint32_t ObjectListSizeOffset = 0x10;

	std::vector<std::shared_ptr<NetworkableObject>> Networkables;
	std::unordered_map<uint64_t, std::shared_ptr<NetworkableObject>> NetworkableMap;
	int ListSize = 0;
public:
	BaseNetworkable();
	void ItterateEntities();
	void UpdateNearbyEntities();
	void UpdateAnimalPositions(VMMDLL_SCATTER_HANDLE handle);
	void UpdateVehiclePositions(VMMDLL_SCATTER_HANDLE handle);
	std::vector<std::shared_ptr<NetworkableObject>> GetNetworkables();
	std::vector<std::shared_ptr<NetworkableObject>> OresList;
	std::mutex OreMutex;
	std::vector<std::shared_ptr<NetworkableObject>> CollectableList;
	std::mutex CollectableMutex;
	std::vector<std::shared_ptr<NetworkableObject>> DroppedItems;
	std::mutex DroppedItemsMutex;
	std::vector<std::shared_ptr<NetworkableObject>> LootableList;
	std::mutex LootableMutex;
	std::vector<std::shared_ptr<NetworkableObject>> DeployableList;
	std::mutex DeployableMutex;
	std::vector<std::shared_ptr<NetworkableObject>> AnimalList;
	std::mutex AnimalMutex;
	std::vector<std::shared_ptr<NetworkableObject>> AttackEntitiesList;
	std::mutex AttackEntitiesMutex;
	std::vector<std::shared_ptr<NetworkableObject>> VehicleList;
	std::mutex VehicleMutex;
	int GetListSize();
	uint64_t LocalPlayerOffset = 0x0;
};
