#pragma once
#include <DMALibrary/Memory/Memory.h>
#include "Misc/Vector.h"
class BaseMovement
{
	// https://www.unknowncheats.me/forum/rust/616286-external-capsule.html YOINKED
	struct PxCapsuleGeometry
	{
		int mType;
		float radius;
		float halfHeight;
	};

	PxCapsuleGeometry normalCapsule {2, 0.5f, 0.4f};
	PxCapsuleGeometry smallCapsule {2, 0.44f, 0.46f};

	uint64_t Class = 0x0;
	uint64_t GroundAngle = 0xCC; // private float groundAngle;
	uint64_t GroundAngleNew = 0xD0; //private float groundAngleNew;
	uint64_t MaxAngleClimbing = 0x98; //public float maxAngleClimbing;
	uint64_t MaxAngleWalking = 0x94; //public float maxAngleWalking;
	uint64_t GroundTime = 0xDC; // 	private float groundTime;
	uint64_t JumpTime = 0xE0; //private float jumpTime;
	uint64_t LandTime = 0xE4; //private float landTime;
	uint64_t Grounded = 0x54; //private float <Grounded>k__BackingField; -> in BaseMovement
	uint64_t AdminCheat = 0x18; //public bool adminChat;
	uint64_t Running = 0x48; //private float <Running>k__BackingField; BaseMovement
	uint64_t GravityMultiplier = 0x8C; //public float gravityMultiplier;
	uint64_t CapsuleOffset = 0xB8; //private CapsuleCollider capsule;
	uint64_t TargetMovement = 0x3C; // private Vector3 <TargetMovement>k__BackingField;
	uint64_t Flying = 0x151; // private bool flying;
	uint64_t WasFlying = 0x152; // 	private bool wasFlying;
	uint64_t MaxVelocity = 0xC8; // private float maxVelocity; // just below groundangle
	uint64_t Capsule = 0x0;
	uint64_t CapsuleInstance;
	uint64_t CapsuleShape;
	Vector3 TargetMovementVector;

public:
	BaseMovement(uint64_t address);
	float GetGroundAngle();
	float GetGroundAngleNew();
	float GetMaxAngleClimbing();
	float GetMaxAngleWalking();
	float GetGroundTime();
	float GetJumpTime();
	float GetLandTime();
	// Create a scatter handle and then execute it after the function calls.
	void WriteAdminCheat(bool value);
	void WriteGroundAngle(VMMDLL_SCATTER_HANDLE handle, float angle);
	void WriteGroundAngleNew(VMMDLL_SCATTER_HANDLE handle, float angle);
	void WriteMaxAngleClimbing(VMMDLL_SCATTER_HANDLE handle, float angle);
	void WriteMaxAngleWalking(VMMDLL_SCATTER_HANDLE handle, float angle);
	void WriteGroundTime(VMMDLL_SCATTER_HANDLE handle, float time);
	void WriteJumpTime(VMMDLL_SCATTER_HANDLE handle, float time);
	void WriteLandTime(VMMDLL_SCATTER_HANDLE handle, float time);
	void WriteGrounded(VMMDLL_SCATTER_HANDLE handle, float grounded);
	void WriteRunning(VMMDLL_SCATTER_HANDLE handle, float running);
	void WriteGravityMultiplier(VMMDLL_SCATTER_HANDLE handle, float multiplier);
	void SetCapsule(VMMDLL_SCATTER_HANDLE handle, bool value);
	void WriteMaxVelocity(VMMDLL_SCATTER_HANDLE handle, float value);
	void UpdateCapsule();
	void WriteCanJump(VMMDLL_SCATTER_HANDLE handle);
	void WriteFlying(VMMDLL_SCATTER_HANDLE handle, bool value);
	Vector3 GetTargetMovement();
	void SetTargetMovement(VMMDLL_SCATTER_HANDLE handle);
	void WriteTargetMovement(VMMDLL_SCATTER_HANDLE handle,Vector3 pos);
	void WriateWasFlying(VMMDLL_SCATTER_HANDLE handle, bool value);	
};
