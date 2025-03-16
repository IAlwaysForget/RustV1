#include "pch.h"
#include "Globals.h"
#include "Item.h"

Item::Item(uint64_t address, VMMDLL_SCATTER_HANDLE handle)
{
	
	Class = address;
	mem.AddScatterReadRequest(handle, Class + ItemID, reinterpret_cast<void*>(&ItemID), sizeof(uint32_t));
	// for some reason scatter reads wont work here in release
	mem.Read( Class + HeldEntity, reinterpret_cast<void*>(&HeldEntity), sizeof(uint64_t));
	mem.Read( Class + Info, reinterpret_cast<void*>(&Info), sizeof(uint64_t));

}
Item::~Item()
{

}

uint64_t Item::GetClass()
{
	return Class;
}

uint32_t Item::GetItemID()
{
	if (Class != 0 && this != nullptr)
		return ItemID;
	else
		return 0;
}

void Item::SetSkin(VMMDLL_SCATTER_HANDLE handle, ULONG skinid)
{
	if (Class == 0 || this == nullptr || HeldEntity == 0)
		return;
	mem.AddScatterWriteRequest<ULONG>(handle,HeldEntity + Skin, skinid);
}
void Item::UpdateEnglish()
{
	if (Class == 0 || this == nullptr || DisplayName == 0)
		return;
	English = mem.Read<uint64_t>(DisplayName + English);
}
void Item::UpdateDisplayName()
{
	if (Class == 0 || this == nullptr || Info == 0)
		return;
	DisplayName = mem.Read<uint64_t>(Info + DisplayName);
}
void Item::SetName()
{
	if (Class == 0 || this == nullptr || English == 0)
		return;

	mem.Read((ULONG64)English + 0x14, reinterpret_cast<void*>(&ItemName), sizeof(ItemName));
}
void Item::SetNameScatter(VMMDLL_SCATTER_HANDLE handle)
{
	if (Class == 0 || this == nullptr || English == 0)
		return;
	mem.AddScatterReadRequest(handle,(ULONG64)English + 0x14, reinterpret_cast<void*>(&ItemName), sizeof(ItemName));
}

void Item::UpdateEnglishScattter(VMMDLL_SCATTER_HANDLE handle)
{
	if (Class == 0 || this == nullptr || DisplayName == 0)
		return;
	mem.AddScatterReadRequest(handle, (ULONG64)DisplayName + English, reinterpret_cast<void*>(&English), sizeof(uint64_t));
}

void Item::UpdateDisplayNameScatter(VMMDLL_SCATTER_HANDLE handle)
{
	if (Class == 0 || this == nullptr || Info == 0)
		return;
	mem.AddScatterReadRequest(handle, (ULONG64)Info + DisplayName, reinterpret_cast<void*>(&DisplayName), sizeof(uint64_t));
}
std::string Item::GetName()
{
	if (Class == 0 || this == nullptr)
		return xorstr_("");
	std::wstring witemname = ItemName;
	return std::string(witemname.begin(), witemname.end());
}
uint64_t Item::GetHeldEntity()
{
	if (Class == 0 || this == nullptr || HeldEntity == 0)
		return 0;

	return HeldEntity;
}

std::shared_ptr<BaseProjectile> Item::GetBaseProjectile()
{
	if (Class == 0 || this == nullptr)
		return nullptr;
	if (HeldEntity == 0)
		return nullptr;
	BaseProjectileInstance = std::make_shared<BaseProjectile>(HeldEntity);
	return BaseProjectileInstance;
}
void Item::SetEokaStrikes(VMMDLL_SCATTER_HANDLE handle, const std::string& name)
{
	if (Class == 0 || this == nullptr)
		return;
	if (HeldEntity == 0)
		return;
	if(name != xorstr_("Eoka Pistol"))
			return;
	mem.AddScatterWriteRequest<float>(handle,HeldEntity + EokaSuccessRate, 10000.0f);
}
void Item::SetCompoundBow(VMMDLL_SCATTER_HANDLE handle, const std::string& name)
{
	if (Class == 0 || this == nullptr)
		return;
	if (HeldEntity == 0)
		return;
	if (name != xorstr_("Compound Bow"))
		return;
	mem.AddScatterWriteRequest<float>(handle, HeldEntity + BowStringDuration, 0.0f);
}
void Item::FastHammer(VMMDLL_SCATTER_HANDLE handle, const std::string& name)
{
	if (Class == 0 || this == nullptr)
		return;
	if (HeldEntity == 0)
		return;

	if (name != xorstr_("Hammer"))
		return;
	mem.AddScatterWriteRequest<float>(handle, HeldEntity + AnimationDelay, 0.01f);
	mem.AddScatterWriteRequest<float>(handle, HeldEntity + RepeatDelay, 0.219f);
}
void Item::SetRotation(VMMDLL_SCATTER_HANDLE handle, const std::string& name, Vector3 rotation)
{
	if (Class == 0 || this == nullptr)
		return;
	if (HeldEntity == 0)
		return;

	if (name != xorstr_("Building Plan"))
		return;
	mem.AddScatterWriteRequest<Vector3>(handle, HeldEntity + RotationOffset, rotation);
}
void Item::SetAttackDistance(VMMDLL_SCATTER_HANDLE handle)
{
	if (Class == 0 || this == nullptr)
		return;
	if (HeldEntity == 0)
		return;
	mem.AddScatterWriteRequest<float>(handle, HeldEntity + AttackDistance, 5.0f);
	mem.AddScatterWriteRequest<float>(handle, HeldEntity + AttackRadius, 3.0f);

}

void Item::SetAttackableSprinting(VMMDLL_SCATTER_HANDLE handle)
{
	if (Class == 0 || this == nullptr)
		return;
	if (HeldEntity == 0)
		return;
	mem.AddScatterWriteRequest<bool>(handle, HeldEntity + SprintBlock, false);
}
