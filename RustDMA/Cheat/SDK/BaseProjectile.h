#pragma once
#include "../DMALibrary/DMALibrary/Memory/Memory.h"

struct ProjectileWeaponModModifier
{
	bool Enabled;
	float Scalar;
	float Offset;
};

class BaseProjectile
{
	uint64_t Class = 0x0;
	uint64_t RecoilProperties = 0x310; //	public RecoilProperties recoil;

	// Part of  RecoilProperties
	uint64_t RecoilOverride = 0x80; //	public RecoilProperties newRecoilOverride;
	uint64_t RecoilYawMin = 0x18; // RecoilProperties-> public float yawMin;
	uint64_t RecoilYawMax = 0x1C; //  RecoilProperties->public float yawMax;
	uint64_t RecoilPitchMin = 0x20; // RecoilProperties-> public float pitchMin;
	uint64_t RecoilPitchMax = 0x24; // RecoilProperties-> public float pitchMax;
	uint64_t ProjectileThickness = 0x2c; // Projectile-> public float thickness;
	uint64_t AimSway = 0x308; // public float aimSway;
	uint64_t AimCone = 0x320; // public float aimCone;
	uint64_t AimConeHip = 0x324; // public float hipAimCone;
	uint64_t AimConeMaxPenalty = 0x32C; // public float aimConePenaltyMax;
	//uint64_t AimConeStancePenalty = 0x354; //private float stancePenalty;
	uint64_t AimConeSightedScale = 0x338; //public float stancePenaltyScale;
	uint64_t Automatic = 0x2B8; // public bool automatic;
	uint64_t PrimaryMagazine = 0x2F0; // 	public global::BaseProjectile.Magazine primaryMagazine;
	uint64_t AmmoType = 0x20; // public ItemDefinition ammoType;
	uint64_t ShortName = 0x28; // public string shortname;
	uint64_t Children = 0x68; // BaseNetworkable - > public readonly List<global::BaseEntity> children
	uint64_t ProjectileVelocityScale = 0x2B4; // public float projectileVelocityScale;
	float VelocityScale = 1.0f;
	wchar_t AmmoName[34] = {'\0'};
	std::string AmmoNameString = "";
	ProjectileWeaponModModifier ProjectileModifier;
	uint64_t IsLight = 0x1F8; // ProjectileWeaponMod -> public bool isLight;
	uint64_t IsMuzzleBrake = 0x1F9; // ProjectileWeaponMod -> public bool IsMuzzleBrake;
	uint64_t IsMuzzleBoost = 0x1FA; // ProjectileWeaponMod -> public bool IsMuzzleBoost;
	uint64_t IsScope = 0x1FB; // ProjectileWeaponMod -> public bool isScope;
	uint64_t IsSilencer = 0x188; // ProjectileWeaponMod -> public bool IsSilencer;


public:
	BaseProjectile(uint64_t address);
	bool IsValidWeapon();
	void WriteRecoilYaw(VMMDLL_SCATTER_HANDLE handle, const std::string& name, int percent);
	void WriteRecoilPitch(VMMDLL_SCATTER_HANDLE handle, const std::string& name, int percent);
	void WriteSway(VMMDLL_SCATTER_HANDLE handle, const std::string& sway, int perrcent);
	void WriteAimcone(VMMDLL_SCATTER_HANDLE handle, const std::string& name, int percent);
	void WriteAutomatic(VMMDLL_SCATTER_HANDLE handle, const std::string& name, bool value);
	void WriteAmmoName();
	void WriteProjectileWeaponMod();
	ProjectileWeaponModModifier GetProjectileWeaponMod();
	std::string GetAmmoName();
	float GetVelocityScale();
	uint64_t GetClass();
};
