#pragma once
#include "BaseMovement.h"
#include "Config.h"
#include "Memory.h"
#include "Item.h"
#include "xorstr.hpp"
#include "Misc/vector.h"

static enum PlayerFlags : int
{
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Incapacitated = 524288,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
	VoiceRangeBoost = 8388608,
	ModifyClan = 16777216,
	LoadingAfterTransfer = 33554432,
	NoRespawnZone = 67108864
};

enum Bones : int
{
	LeftHip = 1,
	LeftLnee = 2,
	LeftFoot = 3,
	Pelvis = 6,
	RightHip = 13,
	RightKnee = 14,
	RightFoot = 15,
	Spine1 = 18,
	Spine2 = 20,
	Spine3 = 21,
	Spine4 = 22,
	LeftUpperarm = 24,
	LeftForearm = 25,
	LeftHand = 26,
	Neck = 46,
	Head = 47,
	RightUpperarm = 55,
	RightForearm = 56,
	RightHand = 57,
};

static enum StanceFlags : int
{
	Ducked = 1,
	Jumped = 2,
	OnGround = 4,
	Sleeper = 8,
	Sprinting = 16,
	OnLadder = 32,
	Flying = 64,
	Aim = 128,
	Prone = 256,
	Mounted = 512,
	Relax = 1024,
	OnPhone = 2048,
	Crawling = 4096
};

extern std::unordered_map<uint32_t, std::string> ItemNames;
extern std::unordered_map<uint64_t, std::vector<std::wstring>> TeamMembers;
extern std::unordered_map<uint64_t, std::wstring> TeamName;
void ResetBasePlayerCaches();

class BasePlayer
{
	uint64_t FoundLocalPlayer = 0x1;
	uint64_t Class = 0;
	uint64_t PlayerFlag = 0xA68; // 	public global::BasePlayer.PlayerFlags playerFlags;
	uint64_t LifeStateOffset = 0x274; // 	public BaseCombatEntity.LifeState lifestate;
	uint64_t BaseMovementOffset = 0xBA8; // public BaseMovement movement;
	uint64_t PlayerInventory = 0xA90; // public global::PlayerInventory inventory;
	uint64_t ModelStateOffset = 0xBE0; // public ModelState modelState;
	uint64_t CurrentTeam = 0x978; // public ulong currentTeam;
	uint64_t PlayerEyes = 0xBA0; // public PlayerEyes eyes;
	uint64_t PlayerInput = 0xB20; // public PlayerInput input;
	uint64_t DestroyedOffset = 0x36; // basentworkable ->private bool <IsDestroyed>k__BackingField; // above networkable, below a string
	uint64_t PlayerModel = 0xB98; //public PlayerModel playerModel;
	uint64_t InputState = 0x28; // PlayerInput -> private InputState state;
	uint64_t CurrentMessage = 0x10; // InputState -> private InputMessage current;
	uint64_t AimAngles = 0x10; // InputMessage -> public Vector3 aimAngles;
	uint64_t DisplayName = 0xB70; // protected string _displayName;
	uint64_t ClothingBlocked = 0xBE4; // public bool clothingBlocksAiming;
	bool Destroyed = false;
	int* LifeState = nullptr;
	std::shared_ptr<BaseMovement> BaseMovementInstance;
	uint64_t TeamID = 0;

	uint32_t ActiveItemID = 0;

	// these are offsets outside of baseplayer. I just don't want the hassle of 1000 classes. The Class is before the "->"
	uint64_t ContainerBelt = 0x68; // PlayerInventory -> public global::ItemContainer containerBelt;
	uint64_t ContainerWear = 0x50; // PlayerInventory -> public global::ItemContainer containerWear;
	uint64_t ItemListOffset = 0x38; // ItemContainer -> 	public List<global::Item> itemList;
	uint64_t ContainerBeltItemList = 0x0;
	uint64_t ContainerWearItemList = 0x0;
	uint64_t ItemListContents = 0x10; // ItemList + 0x10 is the actual contents of the c# list
	uint64_t ItemListSize = 0x18; // ItemList + 0x18 is the size of a c# list

	std::vector<std::shared_ptr<Item>> BeltContainerList;
	std::vector<std::shared_ptr<Item>> WearContainerList;

	uint64_t VisiblePlayerList;
	uint64_t VisiblePlayerListBuffer = 0x18; // list value 
	uint32_t VisiblePlayerListSize = 0x10; // list size

	uint32_t IsNPCOffset = 0x2C2; // 	PlayerModel -> private bool <IsNpc>k__BackingField; // very top of the private bools in the class
	uint64_t Visible = 0x23C; // PlayerModel -> internal bool visible;, next to a private transform and internal uint
	uint64_t NewVelocity = 0x1EC; // PlayerModel -> private Vector3 newVelocity; below a few 	internal Quaternion
	bool IsVisible = false;
	bool IsNPC = false;
	uint64_t Position = 0x1C8; // PlayerModel -> internal Vector3 position; // first vector3 below modelstate
	bool Initialized = false;
	Vector3 TransformPosition = Vector3(0, 0, 0);


	std::string Name = xorstr_("");
	wchar_t PlayerName[36] = {'\0'};
	std::string ActiveItemName = xorstr_("");

	uint64_t BaseModel = 0xA8; // BaseEntity -> public Model model;
	uint64_t BoneTransforms = 0x50; // Model -> public Transform[] boneTransforms;	
	std::mutex BoneMutex;
	std::unordered_map<Bones, uint64_t> BoneList;
	std::unordered_map<Bones, uint64_t> BoneTransformList;
	std::unordered_map<Bones, Vector3> BonePositionList;
	std::unordered_map<Bones, uint64_t> TransformIndex;
	std::unordered_map<Bones, uint64_t> TransformData;
	std::unordered_map<Bones, uint64_t[2]> BoneTransformPosition;
	std::unordered_map<Bones, PVOID> BoneMatrices = {};
	std::unordered_map<Bones, uint64_t> BoneMatricesSizes = {};
	std::unordered_map<Bones, PVOID> BoneIndices = {};

	uint64_t ContainerListItems = 0;
	uint32_t ContainerListItemsSize = 0;
	std::vector<uint64_t> ContainerBeltPointers;
	std::vector<std::shared_ptr<Item>> TempContainerBeltList;

	uint64_t ContainerWearListItems = 0;
	uint32_t ContainerWearListItemsSize = 0;
	std::vector<uint64_t> ContainerWearPointers;
	std::vector<std::shared_ptr<Item>> TempContainerWearList;

	std::unordered_map<uint64_t, std::shared_ptr<BasePlayer>> LastPlayerList;

	PlayerFlags ActiveFlag;
	int InActiveFlag = 0;
	uint64_t PoseType = 0x20; // ModelState -> public int flags;
	StanceFlags StanceFlag;
	bool IsPlayerFriendly = false;
	uint64_t ModelState = 0;
	uint64_t ViewOffset = 0x60; // PlayerEyes -> private Vector3 viewOffset;
	Vector3 EyePosition = Vector3(0, 0, 0);
	Vector2 BodyRotation;
	uint64_t ViewAngles = 0x44; // Playerinput->	private Vector3 bodyAngles;
	uint64_t EyeBodyRotation = 0x50; // PlayerEyes -> private Quaternion <bodyRotation>k__BackingField;
	
	Vector3 Velocity = Vector3(0, 0, 0);
	Vector3 LastPosition = Vector3(0, 0, 0);
	std::chrono::steady_clock::time_point LastPositionUpdate;
	std::vector<Vector3> Velocities;
	std::atomic<int> ListSize = 0;
	Vector4 EyeRotation;
	Vector4 ModelRotation;
	bool BonesSetup = false;
	bool SetAsLocalPlayer = false;
	
public:
	void WriteClothingAimBlock(VMMDLL_SCATTER_HANDLE handle, bool value);
	int GetListSize();
	Players GetPlayerConfig();
	BasePlayer(uint64_t address, VMMDLL_SCATTER_HANDLE handle, bool localplayer = false);
	int InitializeClasses(VMMDLL_SCATTER_HANDLE handle);
	~BasePlayer();
	void CachePlayerList();
	void InitializePlayerList();
	void SetPlayerFlag(VMMDLL_SCATTER_HANDLE handle);
	void SetIntPlayerFlag(VMMDLL_SCATTER_HANDLE handle);
	PlayerFlags GetPlayerFlag();
	int GetPlayerFlagInt();
	void WritePlayerFlag(VMMDLL_SCATTER_HANDLE handle, int flag);
	void WritePlayerFlag(VMMDLL_SCATTER_HANDLE handle, PlayerFlags flag);
	void WritePlayerFlag(PlayerFlags flag);
	void SetLifeState(VMMDLL_SCATTER_HANDLE handle);
	bool GetLifeState();
	std::shared_ptr<BaseMovement> GetBaseMovement();
	Vector3 GetBonePosition(Bones bone); 
	Vector3 GetBonePositionByAimbone(int index, bool fullcheck);
	uint32_t GetActiveItemID();
	std::shared_ptr<Item> GetActiveItem();
	void SetName(VMMDLL_SCATTER_HANDLE vmm);
	void SetNPC(VMMDLL_SCATTER_HANDLE vmm);
	bool IsPlayerValid();
	bool IsPlayerValid1();
	void SetupBeltContainerList();
	bool IsLocalPlayer();
	bool IsSleeper();
	std::vector<std::shared_ptr<BasePlayer>> PlayerList;
	std::unordered_map<uint64_t, std::shared_ptr<BasePlayer>> PlayerMap;
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetPosition();
	std::string GetName();
	std::wstring GetWName();
	std::wstring GetRawName();
	bool GetIsNPC();
	uint64_t GetClass();
	void SetActiveItemName(std::string name);
	std::string GetActiveItemName();
	std::vector<std::shared_ptr<Item>> GetBeltContainerList();
	std::vector<std::shared_ptr<Item>> GetWearContainerList();
	void UpdateSkinType(VMMDLL_SCATTER_HANDLE handle);
	void SetBoneTransforms(VMMDLL_SCATTER_HANDLE handle);
	void SetBoneTransforms1(VMMDLL_SCATTER_HANDLE handle);
	void SetBoneTransforms2(VMMDLL_SCATTER_HANDLE handle);
	void SetBoneTransforms3(VMMDLL_SCATTER_HANDLE handle);
	void SetBonePosition(VMMDLL_SCATTER_HANDLE handle);
	void SetBonePosition1();
	void SetBeltContainer1(VMMDLL_SCATTER_HANDLE);
	void SetBeltContainer2(VMMDLL_SCATTER_HANDLE);
	void SetBeltContainer3(VMMDLL_SCATTER_HANDLE);
	void SetBeltContainer4(VMMDLL_SCATTER_HANDLE);
	void UpdateVisibility(VMMDLL_SCATTER_HANDLE handle);
	bool IsPlayerVisible();
	StanceFlags GetPoseType();
	void WritePoseType(VMMDLL_SCATTER_HANDLE handle, StanceFlags stance);
	void WritePoseType(StanceFlags stance);
	void UpdatePoseType(VMMDLL_SCATTER_HANDLE handle);
	void UpdateModelState(VMMDLL_SCATTER_HANDLE handle);
	void WriteFrozen(VMMDLL_SCATTER_HANDLE handle, bool value);
	void WriteEyesPosition(VMMDLL_SCATTER_HANDLE handle, Vector3 position);
	void SetEyesPosition(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetEyesPosition();
	void WriteViewAngles(VMMDLL_SCATTER_HANDLE handle, Vector2 angles);
	bool IsFriendly();
	Vector2 GetBodyRotation();
	void SetBodyRotation(VMMDLL_SCATTER_HANDLE handle);
	void WriteEyeAngles(VMMDLL_SCATTER_HANDLE handle, Vector4 direction);
	void SetEyeAngles(VMMDLL_SCATTER_HANDLE handle);
	Vector4 GetEyeAngles();
	void SetVelocity(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetVelocity();
	void SetTeam(VMMDLL_SCATTER_HANDLE handle);
	bool IsDestroyed();
	void SetDestroyed(VMMDLL_SCATTER_HANDLE handle);
	void SetLastPositionUpdate();
	Vector3 GetLastPosition();
	void SetLastPosition(Vector3 position);
	std::chrono::steady_clock::time_point GetLastPositionUpdate();
	void AddNewVelocity(Vector3 velocity);
	Vector3 GetAverageVelocity();
	uint64_t GetTeamID();
	RadarPlayers GetPlayerRadarConfig();
	void WriteNetworkedAngles(VMMDLL_SCATTER_HANDLE handle, Vector3 angles);
	void DecryptActiveItem();
	std::string GetMostLethalWeapon();
};
