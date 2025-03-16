#pragma once
#include "BaseProjectile.h"
#include "Misc/vector.h"
class Item
{
	// GO to playerinventory then the obfuscated itemcontainer class and click through till you find a class with itemid in it
	/*
	public enum Flag
	{
		// Token: 0x04003B01 RID: 15105
		[Token(Token = "0x4003B01")]
		None = 0,
		// Token: 0x04003B02 RID: 15106
		[Token(Token = "0x4003B02")]
		Placeholder = 1,
		// Token: 0x04003B03 RID: 15107
		[Token(Token = "0x4003B03")]
		IsOn = 2,
		// Token: 0x04003B04 RID: 15108
		[Token(Token = "0x4003B04")]
		OnFire = 4,
		// Token: 0x04003B05 RID: 15109
		[Token(Token = "0x4003B05")]
		IsLocked = 8,
		// Token: 0x04003B06 RID: 15110
		[Token(Token = "0x4003B06")]
		Cooking = 16
	}
	*/
	uint64_t Class = 0x0;
	uint64_t Info = 0x98; //public ItemDefinition info;
	uint32_t ItemID = 0x38; //	public ItemId uid;
	uint64_t HeldEntity = 0x28; //private EntityRef heldEntity; // contains 2 values: baseentity, networkableid, struct name is now hashed 
	uint64_t DisplayName = 0x30;
	uint64_t English = 0x18;
	uint32_t ItemIdValue = 0x0; // ItemID -> public ulong Value;
	uint64_t Skin = 0xB8; //	BaseEntity -> public ulong skinID; // after flags, before has brain
	uint64_t EokaSuccessRate = 0x3B8; // FlintStrikeWeapon-> public float successFraction;
	uint64_t BowStringDuration = 0x3D0; // CompoundBow-> public float stringHoldDuration; // stringHoldDurationMax
	uint64_t SprintBlock = 0x2C9; // public bool blockSprintOnAttack; // basemelee
	uint64_t AttackDistance = 0x2C0; //	public float maxDistance;
	uint64_t AttackRadius = 0x2C4; // public float attackRadius;
	uint64_t RepeatDelay = 0x214; // 	public float repeatDelay;
	uint64_t AnimationDelay = 0x218; // public float animationDelay;
	uint64_t RotationOffset = 0x210; // planner -> private Vector3 rotationOffset; // first private vector3
	std::shared_ptr<BaseProjectile> BaseProjectileInstance = nullptr;
	wchar_t ItemName[36] = { '\0' };
public:
	Item(uint64_t address, VMMDLL_SCATTER_HANDLE handle);
	~Item();
	uint64_t GetClass();
	uint32_t GetItemID();
	uint64_t GetHeldEntity();
	std::string GetName();
	void UpdateDisplayName();
	void UpdateEnglish();
	void SetItemID(VMMDLL_SCATTER_HANDLE handle);
	void SetName();
	void UpdateDisplayNameScatter(VMMDLL_SCATTER_HANDLE handle);
	void UpdateEnglishScattter(VMMDLL_SCATTER_HANDLE handle);
	void SetNameScatter(VMMDLL_SCATTER_HANDLE handle);
	void SetSkin(VMMDLL_SCATTER_HANDLE handle, ULONG skinid);
	void SetEokaStrikes(VMMDLL_SCATTER_HANDLE handle, const std::string& name);
	void SetCompoundBow(VMMDLL_SCATTER_HANDLE handle, const std::string& name);
	void FastHammer(VMMDLL_SCATTER_HANDLE handle, const std::string& name);
	void SetAttackDistance(VMMDLL_SCATTER_HANDLE handle);
	void SetAttackableSprinting(VMMDLL_SCATTER_HANDLE handle);
	void SetRotation(VMMDLL_SCATTER_HANDLE handle, const std::string& name,Vector3 rotation);
	std::shared_ptr<BaseProjectile> GetBaseProjectile();
};