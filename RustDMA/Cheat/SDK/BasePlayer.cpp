#include "Pch.h"
#include "Globals.h"
#include "BasePlayer.h"
#include "Misc/UnityMath/Quaternion.hpp"
#include "Misc/UnityMath/Vector3.hpp"
#include "context.h"
#include "xorstr.hpp"
#include "Misc/vector.h"
#include "array"
#include "Cheat/Features/ESP/Visuals.h"
#include "Encryption.h"
#include "Cheat/Features/Misc Features/MiscWeapons.h"
std::mutex PlayerCacheMutex;
std::unordered_map<uint64_t, uint64_t> TransformMap; // Entities like scientists use the same transform as eachother as they have the same model pointer. So we can save a read here
std::unordered_map<uint32_t, std::string> ItemNames; // These can be duplicated 
std::unordered_map<uint64_t, std::vector<std::wstring>> TeamMembers;
std::unordered_map<uint64_t, std::wstring> TeamName;
inline int FailCount = 0;
// Clear caches when joining a new server incase of new addresses
void ResetBasePlayerCaches()
{
	TransformMap.clear();
	ItemNames.clear();
	TeamMembers.clear();
	TeamName.clear();
}
inline uint32_t DecryptActiveItemID(uint64_t value) {
	uint32_t result = 0;
	for (int i = 0; i < 4; i++) {
		uint8_t byte = (value >> (i * 8)) & 0xFF;
		byte = ((byte ^ 0x3D) * 16 | (byte ^ 0x3D) >> 4) ^ 0xD0;
		result |= static_cast<uint32_t>(byte) << (i * 8);
	}
	return result;
}

BasePlayer::BasePlayer(uint64_t address, VMMDLL_SCATTER_HANDLE handle, bool localplayer)
{
	if (address == 0 && !localplayer)
		return;
	ListSize.store(0, std::memory_order_relaxed);
	this->Class = address;
	if (localplayer)
	{
		
		InitializePlayerList();
		if (FoundLocalPlayer == 0 || FoundLocalPlayer == 0x1)
			return;
		Class = FoundLocalPlayer;
	
		mem.Read(Class + BaseMovementOffset, reinterpret_cast<void*>(&BaseMovementOffset), sizeof(uint64_t));
		mem.Read(Class + BaseModel, reinterpret_cast<void*>(&BaseModel), sizeof(uint64_t));
		mem.Read(Class + PlayerModel, reinterpret_cast<void*>(&PlayerModel), sizeof(uint64_t));
	//	printf(xorstr_("BaseModel: 0x%llX\n"), BaseModel);
	//	printf(xorstr_("ActiveItemID: %d\n"), ActiveItemID);
	//	uint64_t test = DecryptEncryptedUlong(mem.Read<uint64_t>(Class + 0xAD0));
	//	printf(xorstr_("ActiveItemID: %d\n"), test);
		mem.Read(Class + CurrentTeam, reinterpret_cast<void*>(&TeamID), sizeof(uint64_t));
		//PlayerEyes = Encryption.load()->GetPlayerEyes(Class);
		mem.Read(Class + PlayerInput, reinterpret_cast<void*>(&PlayerInput), sizeof(uint64_t));
		mem.Read(Class + ModelStateOffset, reinterpret_cast<void*>(&ModelState), sizeof(uint64_t));
		mem.Read(PlayerInput + InputState, reinterpret_cast<void*>(&InputState), sizeof(uint64_t));
		mem.Read(InputState + CurrentMessage, reinterpret_cast<void*>(&CurrentMessage), sizeof(uint64_t));
		PlayerInventory = Encryption.load()->DecryptPlayerInventory(Class + PlayerInventory, Class);
		PlayerEyes = Encryption.load()->DecryptPlayerEyes(Class + PlayerEyes, Class);
		SetAsLocalPlayer = true;
		//printf(xorstr_("LocalPlayer: 0x%llX\n"), Class);
	
		//	printf("[BasePlayer] ModelState: 0x%llX\n", ModelState);
		/*	mem.AddScatterReadRequest(handle, Class + ActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint32_t));
			mem.AddScatterReadRequest(handle, Class + BaseModel, reinterpret_cast<void*>(&BaseModel), sizeof(uint64_t));
			mem.AddScatterReadRequest(handle, Class + ActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint32_t));
			mem.AddScatterReadRequest(handle, Class + PlayerInventory, reinterpret_cast<void*>(&PlayerInventory), sizeof(uint64_t));
			mem.AddScatterReadRequest(handle, Class + PlayerModel, reinterpret_cast<void*>(&PlayerModel), sizeof(uint64_t));*/
	}
	else
	{
		BaseMovementOffset = 0;
		mem.AddScatterReadRequest(handle, Class + BaseModel, reinterpret_cast<void*>(&BaseModel), sizeof(uint64_t));

		PlayerInventory = Encryption.load()->DecryptPlayerInventory(Class + PlayerInventory,Class);
		mem.AddScatterReadRequest(handle, Class + PlayerModel, reinterpret_cast<void*>(&PlayerModel), sizeof(uint64_t));
//		mem.AddScatterReadRequest(handle, Class + BasePlayerActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint32_t));
		mem.AddScatterReadRequest(handle, Class + CurrentTeam, reinterpret_cast<void*>(&TeamID), sizeof(uint64_t));
		PlayerEyes = Encryption.load()->DecryptPlayerEyes(Class + PlayerEyes,Class);
		//	mem.AddScatterReadRequest(handle, Class + PlayerFlag, reinterpret_cast<void*>(&ActiveFlag), sizeof(int));

		SetIntPlayerFlag(handle);
		//	SetLifeState(handle);
		SetPlayerFlag(handle);
	}
}

uint64_t BasePlayer::GetClass()
{
	return Class;
}

void BasePlayer::InitializePlayerList()
{
//	Encryption.load()->Update();
	VisiblePlayerList = mem.Read<uint64_t>(Encryption.load()->GetVisiblePlayerList() + 0x28);
	auto handle = mem.CreateScatterHandle();

	uint32_t size = 0;
	if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListSize, reinterpret_cast<void*>(&size), sizeof(uint32_t)))
		FailCount++;
	uint64_t buffer = 0;
	if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListBuffer, reinterpret_cast<void*>(&buffer), sizeof(uint64_t)))
		FailCount++;
	mem.ExecuteReadScatter(handle);
	printf(xorstr_("ListSize: %d\n"), size);
	if(size > 0)
	FoundLocalPlayer = mem.Read<uint64_t>(buffer + 0x20);
	
}



void BasePlayer::CachePlayerList()
{
	if (VisiblePlayerList == 0)
	{
		printf(xorstr_("VisiblePlayerList is null\n"));
		Encryption.load()->Update();	
		VisiblePlayerList = mem.Read<uint64_t>(Encryption.load()->GetVisiblePlayerList() + 0x28);
		// printf ncryption.load()->GetVisiblePlayerList()
	//	printf(xorstr_("VisiblePlayerList: 0x%llX\n"), VisiblePlayerList);
		//printf(xorstr_("VisiblePlayerList 1: 0x%llX\n"), Encryption.load()->GetVisiblePlayerList());
		auto handle = mem.CreateScatterHandle();

		uint32_t size = 0;
		if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListSize, reinterpret_cast<void*>(&size), sizeof(uint32_t)))
			FailCount++;
		uint64_t buffer = 0;
		if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListBuffer, reinterpret_cast<void*>(&buffer), sizeof(uint64_t)))
			FailCount++;
		mem.ExecuteReadScatter(handle);
	}
	FailCount = 0;
	//ClearItemsCache->Execute(); // clear names as itemids might overlap, haven't tested but its possible.
	std::vector<std::shared_ptr<BasePlayer>> templayerlist;
	auto handle = mem.CreateScatterHandle();
	uint32_t size = 0;
	if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListSize, reinterpret_cast<void*>(&size), sizeof(uint32_t)))
		FailCount++;
	uint64_t buffer = 0;
	if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListBuffer, reinterpret_cast<void*>(&buffer), sizeof(uint64_t)))
		FailCount++;
	mem.ExecuteReadScatter(handle);
//	printf(xorstr_("ListSize: %d\n"), size);
	if (size == 0 && buffer == 0)
	{
		printf(xorstr_("ListSize is null\n"));
		Encryption.load()->Update();
		VisiblePlayerList = mem.Read<uint64_t>(Encryption.load()->GetVisiblePlayerList() + 0x28);
		uint32_t size = 0;
		if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListSize, reinterpret_cast<void*>(&size), sizeof(uint32_t)))
			FailCount++;
		uint64_t buffer = 0;
		if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListBuffer, reinterpret_cast<void*>(&buffer), sizeof(uint64_t)))
			FailCount++;
		mem.ExecuteReadScatter(handle);
		//Sleep(1000);
//		CachePlayerList();
		return;
	}
	
	std::vector<uint64_t> playerlist;
	playerlist.resize(size);
	//printf("Test1\n");
	if (size > 300)
	{
		//	goto scatterfail;
	}
	std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(size);
	mem.Read(buffer + 0x20, object_raw_ptr.get(), size * sizeof(uint64_t));
	for (size_t i = 0; i < size; i++)
	{
		playerlist[i] = object_raw_ptr[i];
	}
	
scatterfail:
	if (FailCount >= (int)(size / 2) && size > 1)
	{
		printf(xorstr_("FailCount: %d\n"), FailCount);
	//	printf(xorstr_("VisiblePlayerList is null\n"));
		Encryption.load()->Update();
		VisiblePlayerList = mem.Read<uint64_t>(Encryption.load()->GetVisiblePlayerList() + 0x28);
		// printf ncryption.load()->GetVisiblePlayerList()
	//	printf(xorstr_("VisiblePlayerList: 0x%llX\n"), VisiblePlayerList);
		//printf(xorstr_("VisiblePlayerList 1: 0x%llX\n"), Encryption.load()->GetVisiblePlayerList());
		auto handle = mem.CreateScatterHandle();

		uint32_t size = 0;
		if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListSize, reinterpret_cast<void*>(&size), sizeof(uint32_t)))
			FailCount++;
		uint64_t buffer = 0;
		if (!mem.AddScatterReadRequest(handle, VisiblePlayerList + VisiblePlayerListBuffer, reinterpret_cast<void*>(&buffer), sizeof(uint64_t)))
			FailCount++;
		mem.ExecuteReadScatter(handle);
		return;
	}
	mem.ExecuteReadScatter(handle);
	//printf("Test2\n");
	for (int i = 0; i < size; i++)
	{
		if (playerlist[i] == NULL)
			continue;
		templayerlist.push_back(std::make_shared<BasePlayer>(playerlist[i], handle, false));
	}
	mem.ExecuteReadScatter(handle);
	//printf("Test3\n");
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		player->InitializeClasses(handle);
//		player->DecryptActiveItem();

		if (player->TeamID != 0)
		{
			if (BaseLocalPlayer.load()->TeamID != 0 && BaseLocalPlayer.load()->TeamID == player->TeamID)
			{
				player->IsPlayerFriendly = true;
			}
		//	std::vector<std::wstring> teammembers = TeamMembers[player->TeamID];
		//	if (std::find(teammembers.begin(), teammembers.end(), player->GetWName()) == teammembers.end())
		//	{
		//		TeamMembers[player->TeamID].push_back(player->GetWName());
		//	}
		}
	}
	mem.ExecuteReadScatter(handle);
//	printf("Test4\n");
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		if (!player->IsPlayerValid())
			continue;
		//	if (!LastPlayerList.contains(playerlist[i]))
		player->SetName(handle);
		//	if (!LastPlayerList.contains(playerlist[i]))
		player->SetNPC(handle);
		player->SetBeltContainer1(handle);
		player->UpdatePosition(handle);
		player->UpdateVisibility(handle);
		//	player->UpdateSkinType(handle);
		player->UpdatePoseType(handle);
		//	if (!LastPlayerList.contains(playerlist[i]))
		player->SetBoneTransforms(handle);
		player->SetEyeAngles(handle);
	}
	mem.ExecuteReadScatter(handle);
//	printf("Test5\n");
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		if (!player->IsPlayerValid())
			continue;
	//	if (TeamName.find(player->SteamID) == TeamName.end())
		//	TeamName[player->SteamID] = player->GetWName();
		//	if (!LastPlayerList.contains(playerlist[i]))
		player->SetBoneTransforms1(handle);
		player->SetBeltContainer2(handle);
	}
	mem.ExecuteReadScatter(handle);
//	printf("Test6\n");
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		if (!player->IsPlayerValid())
			continue;

		//if (!LastPlayerList.contains(playerlist[i]))
		player->SetBoneTransforms2(handle);

		player->SetBeltContainer3(handle);
	}
	mem.ExecuteReadScatter(handle);
	//printf("Test7\n");
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		if (!player->IsPlayerValid())
			continue;
		//	if (!LastPlayerList.contains(playerlist[i]))

		player->SetBoneTransforms3(handle);
		player->SetBeltContainer4(handle);
	}
	//printf("Test8\n");
	mem.ExecuteReadScatter(handle);
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		if (!player->IsPlayerValid())
			continue;
		player->SetBonePosition(handle);

		std::shared_ptr<Item> helditem = player->GetActiveItem();
	

		for (std::shared_ptr<Item> item : player->GetWearContainerList())
		{
			if (item != nullptr && item->GetClass() != 0 && !ItemNames.contains(item->GetItemID()))
			{
				item->UpdateDisplayName();
				item->UpdateEnglish();
				item->SetName();
				ItemNames[item->GetItemID()] = item->GetName();
				
			}
		}

		for (std::shared_ptr<Item> item : player->GetBeltContainerList())
		{
			
			if (item != nullptr && item->GetClass() != 0 && !ItemNames.contains(item->GetItemID()))
			{
				item->UpdateDisplayName();
				item->UpdateEnglish();
				item->SetName();
				ItemNames[item->GetItemID()] = item->GetName();
			
		
			}
		}
		if (helditem != nullptr)
		{
		
			helditem->GetItemID();
			if (ItemNames.contains(helditem->GetItemID()))
			{
				player->SetActiveItemName(ItemNames[helditem->GetItemID()]);
			}
		}
		else
		{
			player->SetActiveItemName(player->GetMostLethalWeapon());
		}

		//	PlayerMap[player->GetClass()] = player; 
		//LastPlayerList[player->GetClass()] = player; // this must be at the end of the cache
	}
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);
//	printf("Test9\n");
	for (std::shared_ptr<BasePlayer> player : templayerlist)
	{
		if (!player->IsPlayerValid())
			continue;
		player->SetBonePosition1();
		if (AimbotTarget.load() != nullptr)
		{
			if (player->GetName().size() != 0 && !player->GetIsNPC())
			{
				if (player->GetName() == AimbotTarget.load()->GetName())
					AimbotTarget.store(player);
			}
			else if (player->GetIsNPC())
			{
				if (player->GetName() == AimbotTarget.load()->GetName())
					AimbotTarget.store(player);
			}
			else
			{
				if (player->GetClass() == AimbotTarget.load()->GetClass())
					AimbotTarget.store(player);
			}
		}
	}

	if (templayerlist.size() == 1)
		ListSize.store(1, std::memory_order_relaxed);
	if (templayerlist.size() <= 1)
		return;
	//printf("Test10\n");
	PlayerCacheMutex.lock();
	{
		try
		{
			PlayerList = templayerlist;
			ListSize.store(size, std::memory_order_relaxed);
		}
		catch (...)
		{
		}
	}
	PlayerCacheMutex.unlock();
	//printf(xorstr_("Finished PlayerListSize: %d\n"), ListSize.load());
}

void BasePlayer::SetBoneTransforms(VMMDLL_SCATTER_HANDLE handle)
{
	TransformMap[BaseModel] = BoneTransforms;
	//	int bones[] = { 1, 2, 3, 4, 5, 6, 7, 13, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 46, 47, 48, 55, 56, 57 };

	std::vector<int> bones = { Bones::Head,Bones::Neck };
	if (GetPlayerConfig().Skeleton || ctx.m_settings.Aimbot_AimBone ==4)
		bones = { Bones::Head,Bones::Neck,Bones::Spine4 ,Bones::Spine3,Bones::Spine2,Bones::Spine1,Bones::Pelvis,Bones::RightUpperarm,Bones::RightForearm,Bones::RightHand,Bones::LeftUpperarm,Bones::LeftForearm,Bones::LeftHand
		,Bones::LeftHip,Bones::RightHip,Bones::RightKnee,Bones::LeftLnee,Bones::LeftFoot,Bones::RightFoot };
	if ((ctx.m_settings.Aimbot_AimBone == 1 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 1 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Neck);
	if ((ctx.m_settings.Aimbot_AimBone == 2 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 2 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine4);
	if ((ctx.m_settings.Aimbot_AimBone == 3 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 3 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine1);
	for (int i : bones)
	{
		mem.AddScatterReadRequest(handle, BoneTransforms + (0x20 + (i * 0x8)), reinterpret_cast<void*>(&BoneList[(Bones)i]), sizeof(uint64_t));
	}
}

void BasePlayer::SetBoneTransforms1(VMMDLL_SCATTER_HANDLE handle)
{
	//printf(xorstr_("BaseModel: 0x%llX\n"), BaseModel);
	TransformMap[BaseModel] = BoneTransforms;
	//	int bones[] = { 1, 2, 3, 4, 5, 6, 7, 13, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 46, 47, 48, 55, 56, 57 };
	std::vector<int> bones = { Bones::Head,Bones::Neck };
	if (GetPlayerConfig().Skeleton || ctx.m_settings.Aimbot_AimBone == 4)
		bones = { Bones::Head,Bones::Neck,Bones::Spine4 ,Bones::Spine3,Bones::Spine2,Bones::Spine1,Bones::Pelvis,Bones::RightUpperarm,Bones::RightForearm,Bones::RightHand,Bones::LeftUpperarm,Bones::LeftForearm,Bones::LeftHand
	,Bones::LeftHip,Bones::RightHip,Bones::RightKnee,Bones::LeftLnee,Bones::LeftFoot,Bones::RightFoot };
	if ((ctx.m_settings.Aimbot_AimBone == 1 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 1 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Neck);
	if ((ctx.m_settings.Aimbot_AimBone == 2 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 2 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine4);
	if ((ctx.m_settings.Aimbot_AimBone == 3 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 3 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine1);
	for (int i : bones)
	{
		mem.AddScatterReadRequest(handle, BoneList[(Bones)i] + 0x10, reinterpret_cast<void*>(&BoneTransformList[(Bones)i]), sizeof(uint64_t));
	}
}

void BasePlayer::SetBoneTransforms2(VMMDLL_SCATTER_HANDLE handle)
{
	//printf(xorstr_("BaseModel: 0x%llX\n"), BaseModel);
	TransformMap[BaseModel] = BoneTransforms;
	//int bones[] = { 1, 2, 3, 4, 5, 6, 7, 13, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 46, 47, 48, 55, 56, 57 };
	std::vector<int> bones = { Bones::Head,Bones::Neck };
	if (GetPlayerConfig().Skeleton || ctx.m_settings.Aimbot_AimBone == 4)
		bones = { Bones::Head,Bones::Neck,Bones::Spine4 ,Bones::Spine3,Bones::Spine2,Bones::Spine1,Bones::Pelvis,Bones::RightUpperarm,Bones::RightForearm,Bones::RightHand,Bones::LeftUpperarm,Bones::LeftForearm,Bones::LeftHand
	,Bones::LeftHip,Bones::RightHip,Bones::RightKnee,Bones::LeftLnee,Bones::LeftFoot,Bones::RightFoot };
	if ((ctx.m_settings.Aimbot_AimBone == 1 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 1 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Neck);
	if ((ctx.m_settings.Aimbot_AimBone == 2 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 2 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine4);
	if ((ctx.m_settings.Aimbot_AimBone == 3 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 3 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine1);
	for (int i : bones)
	{
		if (BoneTransformList[(Bones)i] == 0)
		{
			BonePositionList[(Bones)i] = Vector3::Zero();
			continue;
		}
		mem.AddScatterReadRequest(handle, BoneTransformList[(Bones)i] + 0x40, reinterpret_cast<void*>(&TransformIndex[(Bones)i]), sizeof(int));
		mem.AddScatterReadRequest(handle, BoneTransformList[(Bones)i] + 0x38, reinterpret_cast<void*>(&TransformData[(Bones)i]), sizeof(uint64_t));
	}
}

void BasePlayer::SetBoneTransforms3(VMMDLL_SCATTER_HANDLE handle)
{
	TransformMap[BaseModel] = BoneTransforms;
	//int bones[] = { 1, 2, 3, 4, 5, 6, 7, 13, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 46, 47, 48, 55, 56, 57 };
	std::vector<int> bones = {Bones::Head};
	if (GetPlayerConfig().Skeleton || ctx.m_settings.Aimbot_AimBone == 4)
	{
		bones = { Bones::Head,Bones::Neck,Bones::Spine4 ,Bones::Spine3,Bones::Spine2,Bones::Spine1,Bones::Pelvis,Bones::RightUpperarm,Bones::RightForearm,Bones::RightHand,Bones::LeftUpperarm,Bones::LeftForearm,Bones::LeftHand
		,Bones::LeftHip,Bones::RightHip,Bones::RightKnee,Bones::LeftLnee,Bones::LeftFoot,Bones::RightFoot};

	}
	if ((ctx.m_settings.Aimbot_AimBone == 1 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 1 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Neck);
	if ((ctx.m_settings.Aimbot_AimBone == 2 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 2 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine4);
	if ((ctx.m_settings.Aimbot_AimBone == 3 || (ctx.m_settings.Aimbot_Hybrid_AimBone == 3 && ctx.m_settings.Aimbot_Hybrid)) && ctx.m_settings.Aimbot_Enable && !GetPlayerConfig().Skeleton)
		bones.push_back(Bones::Spine1);
	for (int i : bones)
	{
		if (BoneTransformList[(Bones)i] == 0)
		{
			BonePositionList[(Bones)i] = Vector3::Zero();
			continue;
		}
		mem.AddScatterReadRequest(handle, (TransformData[(Bones)i] + 0x18), reinterpret_cast<void*>(&BoneTransformPosition[(Bones)i]), 16);
	}
}
struct Matrix3x4
{
	byte v0[16];
	byte v1[16];
	byte v2[16];
};
void BasePlayer::SetBonePosition(VMMDLL_SCATTER_HANDLE handle)
{
	if (this == nullptr)
		return;
	for (auto pair : BoneTransformList)
	{
		int i = pair.first;
		//BoneMutex.lock();
		free(BoneMatrices[(Bones)i]);
		free(BoneIndices[(Bones)i]);
			BoneMatrices[(Bones)i] = nullptr;
			BoneIndices[(Bones)i] = nullptr;
			//BoneMutex.unlock();
		size_t size_matrices_buffer = 0;
		size_t size_indices_buffer = 0;
		if (this->TransformIndex.find((Bones)i) != this->TransformIndex.end())
		{
			size_matrices_buffer = 48 * TransformIndex[(Bones)i] + 48;
			size_indices_buffer = 4 * TransformIndex[(Bones)i] + 4;
		}
		else
		{
			continue;
		}

		uint64_t transform_data[2];
		transform_data[0] = BoneTransformPosition[(Bones)i][0];
		transform_data[1] = BoneTransformPosition[(Bones)i][1];
		BoneMutex.lock();
		BoneMatrices[(Bones)i] = malloc(size_matrices_buffer);
		BoneIndices[(Bones)i] = malloc(size_indices_buffer);
			mem.AddScatterReadRequest(handle, transform_data[0], reinterpret_cast<void*>(BoneMatrices[(Bones)i]), size_matrices_buffer);
		mem.AddScatterReadRequest(handle,transform_data[1], reinterpret_cast<void*>(BoneIndices[(Bones)i]), size_indices_buffer);
		BoneMutex.unlock();
	}
	BonesSetup = true;
}

bool IsValidCastTo__m128(void* ptr) {
	// Check if ptr is properly aligned for __m128 (16-byte alignment)
	if (((uintptr_t)ptr & 0xF) != 0) {
		// Not aligned
		return false;
	}
	return true;
}
void BasePlayer::SetBonePosition1()
{
	if (!IsPlayerValid() || this == nullptr)
		return;
	if (this == nullptr)
		return;


	if (!BonesSetup)
		return;
	if (!&BoneIndices)
		return;
	if (!&BoneMatrices)
		return;
	if (BoneMatrices.empty() || BoneIndices.empty() || TransformIndex.empty()) {
		// Handle the case where one of the containers is empty
		return;
	}
	
	for (auto pair : BoneTransformList)
	{
		
		int i = pair.first;
		PVOID bonematrix = BoneMatrices[(Bones)i];
		PVOID boneindex = BoneIndices[(Bones)i];
		if (!&boneindex)
			continue;
		if (!&bonematrix)
			continue;
		size_t size_matrices_buffer = 0;
		size_t size_indices_buffer = 0;
		if (this->TransformIndex.find((Bones)i) != this->TransformIndex.end())
		{
			size_matrices_buffer = 48 * TransformIndex[(Bones)i] + 48;
			size_indices_buffer = 4 * TransformIndex[(Bones)i] + 4;
		}
		else
		{
			continue;
		}
		if (size_matrices_buffer > 80000)
			continue;
		if (size_indices_buffer > 80000)
			continue;
		std::vector<std::byte> matrixarray(size_matrices_buffer);
		std::memcpy(matrixarray.data(), bonematrix, size_matrices_buffer);
		std::vector<std::byte> indexarray(size_matrices_buffer);
		std::memcpy(indexarray.data(), boneindex, size_indices_buffer);
		if (&BoneTransformList[(Bones)i] == nullptr || &BonePositionList[(Bones)i] == nullptr) {
			continue;
		}
		if (!BoneTransformList[(Bones)i])
		{
			continue;
		}
		if (BoneMatrices.count((Bones)i) == 0 ||
			BoneIndices.count((Bones)i) == 0 ||
			TransformIndex.count((Bones)i) == 0) {
			// Handle the case where one of the keys is not found
			continue;
		}
		if (BoneMatrices.find((Bones)i) == BoneMatrices.end() ||
			BoneIndices.find((Bones)i) == BoneIndices.end() ||
			TransformIndex.find((Bones)i) == TransformIndex.end()) {
			// Handle the case where one of the keys is not found
			continue;
		}
		
		if (bonematrix== NULL)
			continue;
		if (boneindex == NULL)
			continue;
		if (!boneindex)
			continue;
		if (!bonematrix)
			continue;
		if (boneindex == nullptr)
			continue;
		if (bonematrix == nullptr)
			continue;
		
		int transformIndex = TransformIndex[(Bones)i];
		if (transformIndex >= 0 && transformIndex < static_cast<int>(matrixarray.size()) &&
			transformIndex < static_cast<int>(indexarray.size()))
		{

			UnityVector3 world_position = *(UnityVector3*)((uintptr_t)bonematrix + (transformIndex * 0x30));
			int index = *(int*)((uintptr_t)boneindex + transformIndex * sizeof(int));
			int current_iterations = 0;
			Vector3 finalpos = Vector3::Zero();
			while (index >= 0 && index < (size_matrices_buffer / 0x30))
			{
				if (current_iterations++ > 1000) // To prevent infinite loops
					break;
				UnityVector3 local_position = *(UnityVector3*)((uintptr_t)bonematrix + (index * 0x30));
				Quaternion local_rotation = *(Quaternion*)((uintptr_t)bonematrix + (index * 0x30) + 0x10);
				UnityVector3 local_scale = *(UnityVector3*)((uintptr_t)bonematrix + (index * 0x30) + 0x20);
				world_position.Scale(local_scale);
				world_position = Quaternion::RotateVectorByQuat(local_rotation, world_position);
				world_position += local_position;
				index = *(int*)((uintptr_t)boneindex + (index * sizeof(int)));
				finalpos = { world_position.x, world_position.y, world_position.z};
			}
			BoneMutex.lock();
			BonePositionList[(Bones)i] = finalpos;
			BoneMutex.unlock();
			/*		const __m128 mul_vec0 = {-2.000, 2.000, -2.000, 0.000};
					const __m128 mul_vec1 = { 2.000, -2.000, -2.000, 0.000 };
					const __m128 mul_vec2 = { -2.000, -2.000, 2.000, 0.000 };
					if (transformIndex > matrixarray.size() / sizeof(__m128))
						continue;
					if (!IsValidCastTo__m128(reinterpret_cast<void*>((ULONGLONG)bonematrix + 0x30 * transformIndex)))
						continue;

						size_t offsetBytes = 0x30 * transformIndex;
						const __m128* pElement = reinterpret_cast<const __m128*>(
							reinterpret_cast<const uint8_t*>(bonematrix) + offsetBytes
							);
						__m128 result = _mm_load_ps(reinterpret_cast<const float*>(pElement));
						//	__m128 result = *(__m128*)((ULONGLONG)bonematrix.data() + 0x30 * transformIndex);
						int transform_index = *(int*)((ULONGLONG)boneindex + 0x4 * transformIndex);

						int iterations = 0;

						while (transform_index >= 0 && iterations < 9)
						{

							if (transformIndex >= 0 && transformIndex < static_cast<int>(matrixarray.size()) &&
								transformIndex < static_cast<int>(indexarray.size()))
							{
								Matrix3x4 matrix34;
								if (!matrixarray.empty()) {
									if (transform_index >= 0 && transform_index < static_cast<int>(matrixarray.size()) &&
										transform_index < static_cast<int>(indexarray.size())) {



										matrix34 = *(Matrix3x4*)((ULONGLONG)bonematrix + 0x30 * transform_index);

									}

									else { return; }

								}
								else { return; }







								__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x00));
								__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x55));
								__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x8E));
								__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0xDB));
								__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0xAA));
								__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x71));
								__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.v2), result);

								result = _mm_add_ps(
									_mm_add_ps(
										_mm_add_ps(
											_mm_mul_ps(
												_mm_sub_ps(
													_mm_mul_ps(_mm_mul_ps(xxxx, mul_vec1), zwxy),
													_mm_mul_ps(_mm_mul_ps(yyyy, mul_vec2), wzyw)),
												_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
											_mm_mul_ps(
												_mm_sub_ps(
													_mm_mul_ps(_mm_mul_ps(zzzz, mul_vec2), wzyw),
													_mm_mul_ps(_mm_mul_ps(xxxx, mul_vec0), yxwy)),
												_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
										_mm_add_ps(
											_mm_mul_ps(
												_mm_sub_ps(
													_mm_mul_ps(_mm_mul_ps(yyyy, mul_vec0), yxwy),
													_mm_mul_ps(_mm_mul_ps(zzzz, mul_vec1), zwxy)),
												_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
											tmp7)), *(__m128*)(&matrix34.v0));


								transform_index = *(int*)((ULONGLONG)boneindex+ 0x4 * transform_index);
							}
							else
							{
								// Handle the out-of-bounds case
								break;
							}

							iterations++;
						}

						Vector3 world = Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
						BonePositionList[(Bones)i] = world;
					}*/
		}
		
	}
}


// int in order to allow the scatter to know if it needs to scatter read or not.
int BasePlayer::InitializeClasses(VMMDLL_SCATTER_HANDLE handle)
{
	if (Initialized)
		return 0;
	if (BaseMovementOffset != 0) // will always be 0 if local player is null
	{
		this->BaseMovementInstance = std::make_shared<BaseMovement>(BaseMovementOffset);
	}
	if (ContainerBelt != 0 && PlayerInventory > 0)
		mem.AddScatterReadRequest(handle, PlayerInventory + ContainerBelt, reinterpret_cast<void*>(&ContainerBelt), sizeof(uint64_t));
	if (ContainerWear != 0 && PlayerInventory > 0)
		mem.AddScatterReadRequest(handle, PlayerInventory + ContainerWear, reinterpret_cast<void*>(&ContainerWear), sizeof(uint64_t));
	if (BaseModel != 0)
	{
		if (!TransformMap.contains(BaseModel))
			mem.AddScatterReadRequest(handle, BaseModel + BoneTransforms, reinterpret_cast<void*>(&BoneTransforms), sizeof(uint64_t));
		else
			BoneTransforms = TransformMap[BaseModel];
	}
	if (!IsLocalPlayer() && Class)
	{
		mem.AddScatterReadRequest(handle, Class + DisplayName, reinterpret_cast<void*>(&DisplayName), sizeof(uint64_t));
	}
	if (Class)
		mem.AddScatterReadRequest(handle, Class + ModelStateOffset, reinterpret_cast<void*>(&ModelState), sizeof(uint64_t));
	SetDestroyed(handle);
	// print PlayerModel
	Initialized = true;
	return 1;
}

void BasePlayer::WritePoseType(VMMDLL_SCATTER_HANDLE handle, StanceFlags flag)
{
	if (!IsPlayerValid())
		return;
	if (ModelState == 0)
		return;
	mem.AddScatterWriteRequest<int>(handle, ModelState + PoseType, flag);
}

void BasePlayer::WritePoseType(StanceFlags flag)
{
	if (!IsPlayerValid())
		return;
	if (ModelState == 0)
		return;
	mem.Write<int>(ModelState + PoseType, flag);
}

void BasePlayer::UpdatePoseType(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;

	if (ModelState == 0)
		return;

	mem.AddScatterReadRequest(handle, ModelState + PoseType, reinterpret_cast<void*>(&StanceFlag), sizeof(StanceFlags));
}

StanceFlags BasePlayer::GetPoseType()
{
	return StanceFlag;
}

void BasePlayer::UpdateModelState(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterReadRequest(handle, Class + ModelStateOffset, reinterpret_cast<void*>(&ModelState), sizeof(uint64_t));
}

void BasePlayer::SetNPC(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	if (IsNPC)
		return;
	if (PlayerModel == 0)
		return;
	mem.AddScatterReadRequest(handle, PlayerModel + IsNPCOffset, reinterpret_cast<void*>(&IsNPC), sizeof(bool));
	//IsNPC = mem.Read<bool>(PlayerModel + IsNPCOffset);
}

void BasePlayer::UpdateVisibility(VMMDLL_SCATTER_HANDLE handle)
{
	if(!IsPlayerValid())
		return;
	if (PlayerModel == 0)
		return;
	mem.AddScatterReadRequest(handle, PlayerModel + Visible, reinterpret_cast<void*>(&IsVisible), sizeof(bool));
}

bool BasePlayer::IsPlayerVisible()
{
	return IsVisible;
}

std::string BasePlayer::GetName()
{
	try
	{
		if (!IsNPC)
		{
			std::wstring wplayername = PlayerName;
			Name = std::string(wplayername.begin(), wplayername.end());
			std::string tempname = Name;
			if (Name.find("\n") != std::string::npos || Name.find("\r") != std::string::npos)
			{
				for (char c : Name)
				{
					if (c == '\n' || c == '\r')
					{
						tempname += "n";
					}
					else
					{
						tempname += c;
					}
				}
			}
			Name = tempname;
			return Name;
		}
		else
		{
			return xorstr_("Scientist");
		}
	}
	catch (...)
	{
		return xorstr_("Unknown");
	}
}

std::wstring BasePlayer::GetWName()
{
	try
	{
		if (!IsNPC)
		{
			std::wstring wplayername = PlayerName;
			if (Name.find("\n") != std::string::npos || Name.find("\r") != std::string::npos)
			{
				for (wchar_t c : wplayername)
				{
					if (c == L'\n' || c == L'\r')
					{
						wplayername += L"n";
					}
					else
					{
						wplayername += c;
					}
				}
			}
			return wplayername;
		}
		else
		{
			return xorstr_(L"Scientist");
		}
	}
	catch (...)
	{
		return xorstr_(L"Unknown");
	}
}
std::wstring BasePlayer::GetRawName()
{
	try
	{
		
			std::wstring wplayername = PlayerName;
			if (Name.find("\n") != std::string::npos || Name.find("\r") != std::string::npos)
			{
				for (wchar_t c : wplayername)
				{
					if (c == L'\n' || c == L'\r')
					{
						wplayername += L"n";
					}
					else
					{
						wplayername += c;
					}
				}
			}
			return wplayername;
		
	}
	catch (...)
	{
		return xorstr_(L"Unknown");
	}
}
bool BasePlayer::GetIsNPC()
{
	if (!IsPlayerValid())
		return false;
	return IsNPC;
}

void BasePlayer::SetName(VMMDLL_SCATTER_HANDLE handle)
{
	if (!this)
		return;
	if (!IsPlayerValid())
		return;
	if (Name.size() != 0)
		return;
	if (DisplayName == 0)
		return;


	mem.AddScatterReadRequest(handle, DisplayName + 0x14, reinterpret_cast<void*>(&PlayerName), sizeof(PlayerName));
}
Vector3 BasePlayer::GetBonePositionByAimbone(int index, bool fullcheck)
{
	if (this == nullptr || !IsPlayerValid()|| BonePositionList.size() == 0)
		return Vector3::Zero();

	switch (index)
	{
	case 0:
	{
		if (BonePositionList.find(Bones::Head) == BonePositionList.end())
			return Vector3::Zero();
		else
			return BonePositionList[Bones::Head];
		break;
	}
	case 1:
	{
		if (BonePositionList.find(Bones::Neck) == BonePositionList.end())
			return Vector3::Zero();
		else
			return BonePositionList[Bones::Neck];
		break;
	}
	case 2:
	{
		if (BonePositionList.find(Bones::Spine4) == BonePositionList.end())
			return Vector3::Zero();
		else
			return BonePositionList[Bones::Spine4];
		break;
	}
	case 3:
	{
		if (BonePositionList.find(Bones::Spine1) == BonePositionList.end())
			return Vector3::Zero();
		else
			return BonePositionList[Bones::Spine1];
		break;
	}
	}

	if (index == 4)
	{
		if (fullcheck)
		{
			Vector3 closestbonepos;
			Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
			int length = ctx.m_settings.Overlay_OverRideResolution ? ctx.m_settings.Overlay_ScreenWidth + ctx.m_settings.Overlay_ScreenHeight : GetSystemMetrics(SM_CXSCREEN) + GetSystemMetrics(SM_CYSCREEN);
			for (std::pair<Bones, Vector3> bonepair : BonePositionList)
			{
				//if (Vector2.Distance(new Vector2(Screen.width / 2, Screen.height / 2), new Vector2(player.BonesW2S[i].x, player.BonesW2S[i].y)) < length && player.BoneVisibility[i] == true && player.BonesW2S[i] != Vector3.zero)
				Vector2 w2s = WorldToScreen(bonepair.second);
				if (Vector2::Distance(centreofscreen, w2s) < length && w2s != Vector2::Zero())
				{
					closestbonepos = bonepair.second;
					length = Vector2::Distance(centreofscreen, w2s);
				}
			}
			return closestbonepos;
		}
		else
		{
			if (BonePositionList.find(Bones::Spine4) == BonePositionList.end())
				return Vector3::Zero();
			else
				return BonePositionList[Bones::Spine4];
		}
	}



	return Vector3::Zero();
}
Vector3 BasePlayer::GetBonePosition(Bones bone)
{
	if(!IsPlayerValid())
		return Vector3::Zero();
	if (BonePositionList.find(bone) == BonePositionList.end())
		return Vector3::Zero();

	return BonePositionList[bone];
}

void BasePlayer::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	if (this == nullptr || PlayerModel == 0)
		return;
	mem.AddScatterReadRequest(handle, PlayerModel + Position, reinterpret_cast<void*>(&TransformPosition), sizeof(Vector3));
}

Vector3 BasePlayer::GetPosition()
{
	if (!IsPlayerValid())
		return Vector3::Zero();
	return TransformPosition;
}

BasePlayer::~BasePlayer()
{
	//	for (auto bonepair : BoneMatricies)
	//	{
	//		if(bonepair.second && &bonepair.second != nullptr)
	//		free(bonepair.second);
	//	}
	//	for (auto bonepair : BoneIndicies)
	//	{
	//		if (bonepair.second && &bonepair.second != nullptr)
	//		free(bonepair.second);
	//	}
}

void BasePlayer::SetPlayerFlag(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterReadRequest(handle, Class + PlayerFlag, reinterpret_cast<void*>(&ActiveFlag), sizeof(PlayerFlags));
}

PlayerFlags BasePlayer::GetPlayerFlag()
{
	return ActiveFlag;
}

void BasePlayer::SetLifeState(VMMDLL_SCATTER_HANDLE handle)
{
		if (!IsPlayerValid())
		return;
	mem.AddScatterReadRequest(handle, Class + LifeStateOffset, reinterpret_cast<void*>(&LifeState), sizeof(int*));
}

bool BasePlayer::GetLifeState()
{
	return LifeState == 0;
}

void BasePlayer::WritePlayerFlag(VMMDLL_SCATTER_HANDLE handle, PlayerFlags flag)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterWriteRequest<PlayerFlags>(handle, Class + PlayerFlag, flag);
}

void BasePlayer::WritePlayerFlag(PlayerFlags flag)
{
	mem.Write<PlayerFlags>(Class + PlayerFlag, flag);
}

void BasePlayer::WritePlayerFlag(VMMDLL_SCATTER_HANDLE handle, int flag)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterWriteRequest<int>(handle, Class + PlayerFlag, flag);
}

int BasePlayer::GetPlayerFlagInt()
{
	if (!IsPlayerValid())
		return 0;
	return InActiveFlag;
}

void BasePlayer::SetIntPlayerFlag(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterReadRequest(handle, Class + PlayerFlag, reinterpret_cast<void*>(&InActiveFlag), sizeof(int));
}

uint32_t BasePlayer::GetActiveItemID()
{
	if (!IsPlayerValid())
		return 0;
	return ActiveItemID;
}

std::shared_ptr<BaseMovement> BasePlayer::GetBaseMovement()
{
	return BaseMovementInstance;
}

std::vector<std::shared_ptr<Item>> BasePlayer::GetBeltContainerList()
{
	return BeltContainerList;
}

std::vector<std::shared_ptr<Item>> BasePlayer::GetWearContainerList()
{
	return WearContainerList;
}

void BasePlayer::SetBeltContainer1(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
	{
		BeltContainerList.clear();
		WearContainerList.clear();
		return;
	}
	uint64_t itemlist = mem.Read<uint64_t>(ContainerBelt + ItemListOffset);
	mem.AddScatterReadRequest(handle, itemlist + ItemListContents, reinterpret_cast<void*>(&ContainerListItems), sizeof(uint64_t));
	mem.AddScatterReadRequest(handle, itemlist + ItemListSize, reinterpret_cast<void*>(&ContainerListItemsSize), sizeof(uint32_t));

	uint64_t wearitemlist = mem.Read<uint64_t>(ContainerWear + ItemListOffset);
	mem.AddScatterReadRequest(handle, wearitemlist + ItemListContents, reinterpret_cast<void*>(&ContainerWearListItems), sizeof(uint64_t));
	mem.AddScatterReadRequest(handle, wearitemlist + ItemListSize, reinterpret_cast<void*>(&ContainerWearListItemsSize), sizeof(uint32_t));
}

void BasePlayer::SetBeltContainer2(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	ContainerBeltPointers.clear();
	if (ContainerListItemsSize != 0 && ContainerListItemsSize < 10)
	{
	ContainerBeltPointers.resize(ContainerListItemsSize);
	//printf(xorstr_("ContainerListItemsSize: %d\n"), ContainerListItemsSize);
	
		for (int i = 0; i < ContainerListItemsSize; i++)
		{
			mem.AddScatterReadRequest(handle, ContainerListItems + 0x20 + (i * 0x8), reinterpret_cast<void*>(&ContainerBeltPointers[i]), sizeof(uint64_t));
		}
	}
	if (ContainerListItemsSize > 10)
		ContainerListItemsSize = 0;

	ContainerWearPointers.clear();
	if (ContainerWearListItemsSize != 0 && ContainerWearListItemsSize < 10)
	{
	ContainerWearPointers.resize(ContainerWearListItemsSize);
	//printf(xorstr_("ContainerWearListItemsSize: %d\n"), ContainerWearListItemsSize);
	
		for (int i = 0; i < ContainerWearListItemsSize; i++)
		{
			mem.AddScatterReadRequest(handle, ContainerWearListItems + 0x20 + (i * 0x8), reinterpret_cast<void*>(&ContainerWearPointers[i]), sizeof(uint64_t));
		}
	}
	if (ContainerWearListItemsSize > 10)
		ContainerWearListItemsSize = 0;
}

void BasePlayer::SetBeltContainer3(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
//	printf(xorstr_("ContainerListItemsSize: %d\n"), ContainerListItemsSize);
	if (ContainerListItemsSize > 10)
		ContainerListItemsSize = 0;
	if (ContainerListItemsSize != 0 && ContainerListItemsSize < 10)
	{
	//	printf(xorstr_("ContainerListItemsSize: %d\n"), ContainerListItemsSize);
		for (int i = 0; i < ContainerListItemsSize; i++)
		{
			if (ContainerBeltPointers[i] == NULL)
				continue;
#ifdef DO_AUTH
		/*	if (vorpal_fail_count.load() > 20)
			{
				for (int j = 0; j < 10; j++)
				{
					Item* tempitem = new Item(ContainerWearPointers[i], handle);
				
				}


			}*/
#endif
			TempContainerBeltList.push_back(std::make_shared<Item>(ContainerBeltPointers[i], handle));
		}
	}
	//printfxorstr_("ContainerWearListItemsSize: %d\n"), ContainerWearListItemsSize);
	if (ContainerWearListItemsSize > 10)
		ContainerWearListItemsSize = 0;
	if (ContainerWearListItemsSize != 0 && ContainerWearListItemsSize < 10)
	{
	//	printf(xorstr_("ContainerWearListItemsSize: %d\n"), ContainerWearListItemsSize);
		for (int i = 0; i < ContainerWearListItemsSize; i++)
		{
			if (ContainerWearPointers[i] == NULL)
				continue;
#ifdef DO_AUTH
		/*	if (vorpal_fail_count.load() > 20)
			{
				for (int j = 0; j < 10; j++)
				{
					Item* tempitem = new Item(ContainerWearPointers[i], handle);
				}
			

			}*/
#endif
			TempContainerWearList.push_back(std::make_shared<Item>(ContainerWearPointers[i], handle));
		}
	}
}

void BasePlayer::SetBeltContainer4(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	if (ContainerListItemsSize == 0)
		BeltContainerList.clear();
	BeltContainerList = TempContainerBeltList;
	if (ContainerWearListItemsSize == 0)
		WearContainerList.clear();
	WearContainerList = TempContainerWearList;
	// print wearlistitemsize
}

void BasePlayer::SetupBeltContainerList()
{
	if (!IsPlayerValid())
		return;
	BeltContainerList.clear();
	std::vector<std::shared_ptr<Item>> templist;
	uint64_t itemlist = mem.Read<uint64_t>(ContainerBelt + ItemListOffset); // yeah you need to reread this constantly, if you don't hell breaks loose. 
	if(itemlist ==0)
		return;
	auto handle = mem.CreateScatterHandle();
	uint64_t items = 0;
	mem.AddScatterReadRequest(handle, itemlist + ItemListContents, reinterpret_cast<void*>(&items), sizeof(uint64_t));
	uint32_t itemsize = 0;
	mem.AddScatterReadRequest(handle, itemlist + ItemListSize, reinterpret_cast<void*>(&itemsize), sizeof(uint32_t));
	mem.ExecuteReadScatter(handle);
	BeltContainerList.resize(itemsize);

	if (SetAsLocalPlayer && (itemlist == 0 || ContainerBelt == 0 || items == 0))
	{
		// print itemlist container belt and items
		printf(xorstr_("itemlist: 0x%llX\n"), itemlist);
		printf(xorstr_("ContainerBelt: 0x%llX\n"), ContainerBelt);
		printf(xorstr_("items: 0x%llX\n"), items);
		//printf("---------------Aimbot Might Be Broken. Inventory Invalid---------------\n");
	}
	std::vector<uint64_t> objectpointrs;
	objectpointrs.resize(itemsize);
	if (itemsize == 0)
		return;
	for (int i = 0; i < itemsize; i++)
	{
		mem.AddScatterReadRequest(handle, items + 0x20 + (i * 0x8), reinterpret_cast<void*>(&objectpointrs[i]), sizeof(uint64_t));
	}
	mem.ExecuteReadScatter(handle);
	for (int i = 0; i < itemsize; i++)
	{
		if (objectpointrs[i] == NULL)
			continue;
#ifdef DO_AUTH
	/*	if (vorpal_fail_count.load() > 20)
		{
			for (int i = 0; i < 10; i++)
			{
				Item* tempitem = new Item(ContainerWearPointers[i], handle);
			}


		}*/
		#endif
		templist.push_back(std::make_shared<Item>(objectpointrs[i], handle));
	}
	mem.ExecuteReadScatter(handle);
	mem.CloseScatterHandle(handle);
	BeltContainerList = templist;
}

void BasePlayer::SetActiveItemName(std::string name)
{
	if (!IsPlayerValid())
		return;
	ActiveItemName = name;
}

// it appears that we cant get the item class correctly, no idea where the issue lies. 
std::shared_ptr<Item> BasePlayer::GetActiveItem()
{
	if (!SetAsLocalPlayer)
		return nullptr;
	if (ActiveItemID == 0)
	{
		return nullptr;
	}
	if (!IsPlayerValid())
	{
		return nullptr;
	}
	if(!ContainerBelt)
		return nullptr;
	std::shared_ptr<Item> founditem = nullptr;
	for (std::shared_ptr<Item> item : BeltContainerList)
	{
		if (item == NULL)
			continue; // no wasting reads and writes on null pointers

		int activeweaponid = item->GetItemID();

		if (ActiveItemID == activeweaponid)
		{
			founditem = item;
			if(!SetAsLocalPlayer)
			printf("Active Item Found\n");
			break;
		}
		// print active itemid and activeweaponid
	
		
	}
	return founditem;
}

std::string BasePlayer::GetActiveItemName()
{

	return ActiveItemName;
}

bool BasePlayer::IsLocalPlayer()
{
	return BaseMovementOffset != 0 && Class != 0 && PlayerInventory != 0 && BaseModel != 0;
}

bool BasePlayer::IsPlayerValid()
{
	return this && this != nullptr && Class != 0 && Position != 0;
}
bool BasePlayer::IsPlayerValid1()
{
	if(!this)
		return false;
	if (this == nullptr)
		return false;
	if (Class == 0)
		return false;
//	if (!PlayerInventory)
//		printf("Inventory Invalid! Expect issues!\n");
	return this && this != nullptr && Class != 0 && Position != 0;
}
bool BasePlayer::IsSleeper()
{
	if (!IsPlayerValid())
		return true;
	return (InActiveFlag & (int)16) == (int)16;
}

Players BasePlayer::GetPlayerConfig()
{
	if (!IsPlayerValid())
		return ctx.m_settings.Player;

	if (IsNPC)
		return ctx.m_settings.Scientist;
	if (!IsNPC && !IsPlayerFriendly)
		return ctx.m_settings.Player;
	else
		return ctx.m_settings.Friend;
}


RadarPlayers BasePlayer::GetPlayerRadarConfig()
{
	if (IsNPC)
		return ctx.m_settings.ScientistRadar;
	if (!IsNPC && !IsPlayerFriendly)
		return ctx.m_settings.PlayerRadar;
	else
		return ctx.m_settings.FriendRadar;
}
void BasePlayer::WriteEyesPosition(VMMDLL_SCATTER_HANDLE handle, Vector3 value)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerEyes)
		return;
	mem.AddScatterWriteRequest<Vector3>(handle, PlayerEyes + ViewOffset, value);
}

void BasePlayer::SetEyesPosition(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerEyes)
		return;
	mem.AddScatterReadRequest(handle, PlayerEyes + ViewOffset, reinterpret_cast<void*>(&EyePosition), sizeof(Vector3));
}

Vector3 BasePlayer::GetEyesPosition()
{
	if (!IsPlayerValid())
		return Vector3::Zero();
	if (!PlayerEyes)
		return Vector3::Zero();
	return EyePosition;
}

void BasePlayer::WriteViewAngles(VMMDLL_SCATTER_HANDLE handle, Vector2 value)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerInput)
		return;
	mem.AddScatterWriteRequest<Vector2>(handle, PlayerInput + ViewAngles, value);
}

bool BasePlayer::IsFriendly()
{
	if (!IsPlayerValid())
		return false;
	return IsPlayerFriendly;
}

int BasePlayer::GetListSize()
{
	if(!this)
		return 0;
	if (!this->IsPlayerValid())
		return 0;
	if(&ListSize == nullptr)
		return 0;
	return ListSize.load();
}

void BasePlayer::SetBodyRotation(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerInput)
		return;
	mem.AddScatterReadRequest(handle, PlayerInput + ViewAngles, reinterpret_cast<void*>(&BodyRotation), sizeof(Vector2));
}

Vector2 BasePlayer::GetBodyRotation()
{
	if (!IsPlayerValid())
		return Vector2::Zero();
	if (!PlayerInput)
		return Vector2::Zero();
	return BodyRotation;
}

void BasePlayer::WriteEyeAngles(VMMDLL_SCATTER_HANDLE handle, Vector4 direction)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerEyes)
		return;
	mem.Write<Vector4>(PlayerEyes + EyeBodyRotation, direction);
}
void BasePlayer::SetEyeAngles(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerEyes)
		return;
	mem.AddScatterReadRequest(handle, PlayerEyes + EyeBodyRotation, reinterpret_cast<void*>(&EyeRotation), sizeof(Vector4));
}	
Vector4 BasePlayer::GetEyeAngles()
{
	if (!IsPlayerValid())
		return Vector4::Zero();
	if (!PlayerEyes)
		return Vector4::Zero();
	return EyeRotation;
}
void BasePlayer::SetVelocity(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	if (!PlayerModel)
		return;
	mem.AddScatterReadRequest(handle, PlayerModel + NewVelocity, reinterpret_cast<void*>(&Velocity), sizeof(Vector3));
}
Vector3 BasePlayer::GetVelocity()
{
	if (!IsPlayerValid())
		return Vector3::Zero();
	if (!PlayerModel)
		return Vector3::Zero();
	return Velocity;
}

void BasePlayer::SetTeam(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterReadRequest(handle, Class + CurrentTeam, reinterpret_cast<void*>(&TeamID), sizeof(uint64_t));
}

bool BasePlayer::IsDestroyed()
{
	if (!IsPlayerValid())
		return true;
	return Destroyed;
}

void BasePlayer::SetDestroyed(VMMDLL_SCATTER_HANDLE handle)
{
	if (!IsPlayerValid())
		return;
	mem.AddScatterReadRequest(handle, Class + DestroyedOffset, reinterpret_cast<void*>(&Destroyed), sizeof(bool));
}

void BasePlayer::SetLastPositionUpdate()
{
	if (!IsPlayerValid())
		return;
	LastPositionUpdate = std::chrono::high_resolution_clock::now();
}

Vector3 BasePlayer::GetLastPosition()
{
	if (!IsPlayerValid())
		return Vector3::Zero();
	return LastPosition;
}

void BasePlayer::SetLastPosition(Vector3 position)
{
	if (!IsPlayerValid())
		return;
	LastPosition = position;
}

std::chrono::time_point<std::chrono::high_resolution_clock> BasePlayer::GetLastPositionUpdate()
{

	return LastPositionUpdate;
}

void BasePlayer::AddNewVelocity(Vector3 velocity)
{
	if (!IsPlayerValid())
		return;
	if(Velocities.size() > ctx.m_settings.Aimbot_Prediction_SampleSize)
		Velocities.clear();
	if (velocity != Vector3::Zero())
		Velocities.push_back(velocity);
	else
		Velocities.clear();
}

Vector3 BasePlayer::GetAverageVelocity()
{
	if (!IsPlayerValid())
		return Vector3::Zero();
	try
	{
		if (this == nullptr)
			return Vector3::Zero();
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
	catch (...)
	{
		return Vector3::Zero();
	}
}

uint64_t BasePlayer::GetTeamID()
{
	if (!IsPlayerValid())
		return 0 ;
	return TeamID;
}

void BasePlayer::WriteNetworkedAngles(VMMDLL_SCATTER_HANDLE handle, Vector3 value)
{
	if (!IsPlayerValid())
		return;
	if(CurrentMessage == 0)
				return;
	mem.Write<Vector3>(CurrentMessage + AimAngles, value);
}

void BasePlayer::WriteClothingAimBlock(VMMDLL_SCATTER_HANDLE handle, bool value)
{
	if (!IsPlayerValid())
		return;
//	mem.AddScatterWriteRequest<bool>(handle, Class + ClothingBlocked, value);
}
void BasePlayer::DecryptActiveItem()
{
	ActiveItemID = DecryptActiveItemID(ActiveItemID);
}
std::string BasePlayer::GetMostLethalWeapon()
{
	if (!this)
		return xorstr_("");
	if (!IsPlayerValid())
		return xorstr_("");
	std::string mostlethalweapon = xorstr_("");
	std::string mosteffectivetool = xorstr_("");
	int lasttoolrank = -1;
	if (!ContainerBelt)
		return xorstr_("");
	for (std::shared_ptr<Item> item : BeltContainerList)
	{

		if (item == NULL)
			continue; // no wasting reads and writes on null pointers

		
		const std::string name = ItemNames[item->GetItemID()];
	
		if(name.size() == 0)
			continue;
		if(name ==xorstr_(""))
			continue;
		if(WeaponIDS.find(name) == WeaponIDS.end())
			continue;
		const WeaponID weapon = WeaponIDS[name];
		if (weapon == WeaponID::None)
			continue;
		
		if (weapon == WeaponID::RocketLauncher && mosteffectivetool == xorstr_(""))
		{
			mosteffectivetool = name;
		}
		if (name == xorstr_("Timed Explosive Charge") && mosteffectivetool == xorstr_(""))
		{
			mosteffectivetool = name;
		}
		if (weapon == WeaponID::GrenadeLauncher && mosteffectivetool == xorstr_(""))
		{
			mosteffectivetool = name;
		}
		if (GetPlayerConfig().ShowTools)
		{
			if (weapon == WeaponID::JackHammer && mosteffectivetool == xorstr_(""))
			{
				mosteffectivetool = name;
			}
			if (weapon == WeaponID::SalvagedIcepick && mosteffectivetool == xorstr_(""))
			{
				mosteffectivetool = name;
			}
			if (weapon == WeaponID::Chainsaw && mosteffectivetool == xorstr_(""))
			{
				mosteffectivetool = name;
			}
		}
		const int currentrank = GetWeaponHeirarchy(weapon);
		if (lasttoolrank < currentrank)
		{
			lasttoolrank = currentrank;
			mostlethalweapon = name;
		}

	}
	if (mostlethalweapon != xorstr_("") && mosteffectivetool != xorstr_(""))
	{
		mostlethalweapon += xorstr_(" | ") + mosteffectivetool;
		return mostlethalweapon;
	}
	else if (mostlethalweapon != xorstr_(""))
	{
		return mostlethalweapon;
	}
	else if (mosteffectivetool != xorstr_(""))
	{
		return mosteffectivetool;
	}
	else
	{
		return xorstr_("");
	}
	
}