#pragma once
#include <DMALibrary/libs/vmmdll.h>

#include "Memory.h"
#include "xorstr.hpp"
#include "Misc/vector.h"

enum class ObjectType
{
	Stone,
	Metal,
	Sulphur,
	StoneCollectable,
	MetalCollectable,
	SulphurCollectable,
	Wood,
	Hemp,
	Pumpkin,
	Corn,
	Mushroom,
	Potato,
	RedBerry,
	WhiteBerry,
	BlackBerry,
	YellowBerry,
	BlueBerry,
	GreenBerry,
	DroppedItem,
	DetectorSource,
	Barrel,
	FuelBarrel,
	Corpse,
	LockedCrate,
	SupplyDrop,
	Diesel,
	EliteCrate,
	NormalCrate,
	MilitaryCrate,
	MedicalCrate,
	FoodCrate,
	MineCart,
	ToolBox,
	AmmoCrate,
	FuelCrate,
	UnderWaterCrate,
	HelicopterCrate,
	BradleyCrate,
	Stash,
	SleepingBag,
	LandMine,
	BearTrap,
	ShotgunTrap,
	AutoTurret,
	FlameTurret,
	SamSite,
	ToolCupboard,
	Planter,
	LargeBox,
	Workbench1,
	Workbench2,
	Workbench3,
	Furnace,
	LargeFurnace,
	OilRefinery,
	Recycler,
	Battery,
	Bradley,
	AttackHelicopter,
	ScrapHeli,
	MiniCopter,
	AttackMiniCopter,
	Bear,
	PolarBear,
	Horse,
	Boar,
	Chicken,
	Wolf,
	Stag,
	Shark,
	Chinook,
	Train,
	Car,
	TugBoat,
	RowBoat,
	RHIB,
	SmallSubmarine,
	LargeSubmarine,
	SnowMobile,
	TechCrate,
	PedalBike,
	Motorbike,
	MotorbikeSidecar,
};

enum class ItemType
{
	None,
	Gun,
	Tool,
	Armour,
	Medical,
	Resources,
	Food,
	Explosives,
	Ammo,
	Components,
	Other
};

class NetworkableObject
{
private:
	uint64_t Class = 0x0;
	uint64_t ObjectOffset = 0x30;
	uint64_t Object = 0x30;
	uint64_t TransformHost = 0x8;
	uint64_t TransformHostOffset = 0x8;
	uint64_t Transform = 0x38;
	uint64_t TransformOffset = 0x38;
	uint64_t TransformPosition = 0x90;
	Vector3 Position;
	ObjectType Type;
	std::string Name;
	ItemType Item;
	void SetItemType();
	Vector3 LastPosition = Vector3(0, 0, 0);
	std::chrono::steady_clock::time_point LastPositionUpdate;
	std::vector<Vector3> Velocities;
	std::string PrefabName;
public:
	NetworkableObject(uint64_t address, ObjectType type, VMMDLL_SCATTER_HANDLE handle, std::string name = xorstr_(""));
	ObjectType GetType();
	void SetUp1(VMMDLL_SCATTER_HANDLE handle);
	void SetUp2(VMMDLL_SCATTER_HANDLE handle);
	void SetUp3(VMMDLL_SCATTER_HANDLE handle);
	bool SetUp1Valid();
	bool SetUp2Valid();
	bool SetUp3Valid();
	std::string GetPrefabName();
	bool IsValid();
	uint64_t GetClass();
	void SetPosition(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetPosition();
	std::string GetName();
	std::string GetItemName();
	void SetName(std::string name);
	uint64_t DescriminatableClass;
	ItemType GetItemType();
	void SetItemType(ItemType type);
	std::string ResolveItemName();

	void SetLastPositionUpdate();
	Vector3 GetLastPosition();
	void SetLastPosition(Vector3 position);
	std::chrono::steady_clock::time_point GetLastPositionUpdate();
	void AddNewVelocity(Vector3 velocity);
	Vector3 GetAverageVelocity();
};
