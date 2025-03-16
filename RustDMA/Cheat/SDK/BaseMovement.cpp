#include "Pch.h"
#include "BaseMovement.h"

#include "Config.h"
#include "Globals.h"

BaseMovement::BaseMovement(uint64_t address)
{
//	printf(xorstr_("[BaseMovement] Initialized\n"));	
	//printf(xorstr_("[BaseMovement] Address: 0x%llX\n"), address);
	//mem.Write<float>(address + 0xC8, 15.0f);
	if (this != nullptr)
	{
		this->Class = address;
	//	printf(xorstr_("[BaseMovement] Class: 0x%llX\n"), Class);
		//Capsule = mem.Read<uint64_t>(Class + CapsuleOffset);
		UpdateCapsule();
	}
	else
	{
		Sleep(1000);
		Initialize();
	}
}

float BaseMovement::GetGroundAngle()
{
	return mem.Read<float>(Class + GroundAngle);
}

float BaseMovement::GetGroundAngleNew()
{
	return mem.Read<float>(Class + GroundAngleNew);
}

float BaseMovement::GetMaxAngleClimbing()
{
	return mem.Read<float>(Class + MaxAngleClimbing);
}

float BaseMovement::GetMaxAngleWalking()
{
	return mem.Read<float>(Class + MaxAngleWalking);
}

float BaseMovement::GetGroundTime()
{
	return mem.Read<float>(Class + GroundTime);
}

void BaseMovement::WriteMaxAngleWalking(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	mem.AddScatterWriteRequest<float>(handle, Class + MaxAngleWalking, angle);
}

float BaseMovement::GetJumpTime()
{
	return mem.Read<float>(Class + JumpTime);
}

float BaseMovement::GetLandTime()
{
	return mem.Read<float>(Class + LandTime);
}

void BaseMovement::WriteGroundAngle(VMMDLL_SCATTER_HANDLE handle, float angle)
{
//	mem.AddScatterWriteRequest<float>(handle, Class + GroundAngle, angle);
}

void BaseMovement::WriteGroundAngleNew(VMMDLL_SCATTER_HANDLE handle, float angle)
{
//	mem.AddScatterWriteRequest<float>(handle, Class + GroundAngleNew, angle);
}
void BaseMovement::WriteMaxVelocity(VMMDLL_SCATTER_HANDLE handle, float value)
{
	mem.AddScatterWriteRequest(handle, Class + MaxVelocity, value);
}
void BaseMovement::WriteGrounded(VMMDLL_SCATTER_HANDLE handle, float grounded)
{
	mem.AddScatterWriteRequest<float>(handle, Class + Grounded, grounded);
}

void BaseMovement::WriteMaxAngleClimbing(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	if (!mem.AddScatterWriteRequest<float>(handle, Class + MaxAngleClimbing, angle))
		printf(xorstr_("[BaseMovement] Failed to write MaxAngleClimbing\n"));
}

void BaseMovement::WriteGroundTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	mem.AddScatterWriteRequest<float>(handle, Class + GroundTime, time);
	
}

void BaseMovement::WriteJumpTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	mem.AddScatterWriteRequest<float>(handle, Class + JumpTime, time);
}

void BaseMovement::WriteLandTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	if (!mem.AddScatterWriteRequest<float>(handle, Class + LandTime, time))
		printf(xorstr_("[BaseMovement] Failed to write LandTime\n"));
}

void BaseMovement::WriteAdminCheat(bool value)
{
	mem.Write<bool>(Class + AdminCheat, value);
}

void BaseMovement::WriteRunning(VMMDLL_SCATTER_HANDLE handle, float value)
{
	mem.AddScatterWriteRequest<float>(handle, Class + Running, value);
}

void BaseMovement::WriteGravityMultiplier(VMMDLL_SCATTER_HANDLE handle, float value)
{
	mem.AddScatterWriteRequest<float>(handle, Class + GravityMultiplier, value);
}

// this is pasted from https://www.unknowncheats.me/forum/rust/616286-external-capsule.html

void BaseMovement::UpdateCapsule()
{
/*	if (!this)
		return;
	try
	{
		CapsuleInstance = mem.Read<uint64_t>(Capsule + 0x10);
		CapsuleShape = mem.Read<uint64_t>(CapsuleInstance + 0x48);
	}
	catch (std::exception ex)
	{
		Sleep(1000);
		UpdateCapsule();
	}*/
}

bool LastSetRegular = false;

void BaseMovement::SetCapsule(VMMDLL_SCATTER_HANDLE handle, bool value)
{
	/*if (Capsule != 0 && CapsuleShape != 0 && value)
	{
		mem.AddScatterWriteRequest<float>(handle, CapsuleInstance + 0x70, smallCapsule.radius);
		mem.AddScatterWriteRequest<PxCapsuleGeometry>(handle, CapsuleShape + 0x98, smallCapsule);
		LastSetRegular = false;
	}
	else if (value && CapsuleShape != 0 && Capsule != 0)
	{
		if (LastSetRegular)
			return;
		mem.AddScatterWriteRequest<float>(handle, CapsuleInstance + 0x70, normalCapsule.radius);
		mem.AddScatterWriteRequest<PxCapsuleGeometry>(handle, CapsuleShape + 0x98, normalCapsule);

		LastSetRegular = true;
	}*/
}

void BaseMovement::WriteCanJump(VMMDLL_SCATTER_HANDLE handle)
{
	mem.AddScatterWriteRequest<float>(handle, Class + JumpTime, 0.51f);
	mem.AddScatterWriteRequest<float>(handle, Class + GroundTime, 9999999.f);
	mem.AddScatterWriteRequest<float>(handle, Class + LandTime, 0.3f);
}

void BaseMovement::WriteFlying(VMMDLL_SCATTER_HANDLE handle, bool value)
{
	mem.AddScatterWriteRequest<float>(handle, Class + Flying, value);
}

void BaseMovement::SetTargetMovement(VMMDLL_SCATTER_HANDLE handle)
{
	mem.AddScatterReadRequest(handle, Class + TargetMovement,reinterpret_cast<void*>(&TargetMovementVector),sizeof(Vector3));
}

Vector3 BaseMovement::GetTargetMovement()
{
	return TargetMovementVector;
}

void BaseMovement::WriteTargetMovement(VMMDLL_SCATTER_HANDLE handle, Vector3 value)
{
	mem.AddScatterWriteRequest<Vector3>(handle, Class + TargetMovement, value);
}

void BaseMovement::WriateWasFlying(VMMDLL_SCATTER_HANDLE handle, bool value)
{
	mem.AddScatterWriteRequest<float>(handle, Class + WasFlying, value);
}	