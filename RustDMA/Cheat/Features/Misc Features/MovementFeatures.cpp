#include "pch.h"
#include "MovementFeatures.h"
#include "Client.h"
#include "context.h"
#include "Globals.h"
#include "MiscFeatures.h"
#include "Misc/CheatFunction.h"
#include "SDK/BasePlayer.h"
#include <VMProtectSDK.h>

bool RewriteGravity = false;
bool RewriteTickInterval = false;
inline bool FlyHacking = false;
inline bool SpeedHacking = false;
std::shared_ptr<CheatFunction> UpdateCapsule = std::make_shared<CheatFunction>(10000, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (BaseLocalPlayer.load()->GetBaseMovement() == nullptr)
		return;
	BaseLocalPlayer.load()->GetBaseMovement()->UpdateCapsule();
});

void OmniSprint()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (BaseLocalPlayer.load()->GetBaseMovement() == nullptr)
		return;
	if (ctx.m_settings.Movement_Misc_OmniSprint)
	{
		if ((BaseLocalPlayer.load()->GetPoseType() & (int)StanceFlags::Sprinting) != (int)StanceFlags::Sprinting)
		{
			StanceFlags newflag = (StanceFlags)((int)BaseLocalPlayer.load()->GetPoseType() + (int)StanceFlags::Sprinting);
			BaseLocalPlayer.load()->WritePoseType(MiscWriteScatterHandle, StanceFlags::Sprinting);
		}
		BaseLocalPlayer.load()->GetBaseMovement()->WriteRunning(MiscWriteScatterHandle, 1.f);
		MiscWriteScatterCount++;
	}
}
std::shared_ptr<CheatFunction> RemoveFallDamage = std::make_shared<CheatFunction>(3000, []()
	{
		if (BaseLocalPlayer.load() == nullptr)
			return;
		if (BaseLocalPlayer.load()->GetBaseMovement() == nullptr)
			return;
		if (ctx.m_settings.Movement_Misc_NoFallDamage)
		{
			BaseLocalPlayer.load()->GetBaseMovement()->WriteMaxVelocity(MiscWriteScatterHandle, 15.f);
			MiscWriteScatterCount++;
		}
	});
std::shared_ptr<CheatFunction> Movement = std::make_shared<CheatFunction>(38, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (BaseLocalPlayer.load()->GetBaseMovement() == nullptr)
		return;
	if (ctx.m_settings.Movement_Misc_Spiderman)
	{
		BaseLocalPlayer.load()->GetBaseMovement()->WriteGroundAngleNew(MiscWriteScatterHandle, 0.f);
		BaseLocalPlayer.load()->GetBaseMovement()->WriteMaxAngleWalking(MiscWriteScatterHandle, 100.f);
		BaseLocalPlayer.load()->GetBaseMovement()->WriteGroundAngle(MiscWriteScatterHandle, 0.f);
		MiscWriteScatterCount++;
	}
	
	if (ctx.m_settings.Movement_Misc_AlwaysShoot)
	{
		if ((BaseLocalPlayer.load()->GetPoseType() & (int)StanceFlags::OnGround) != (int)StanceFlags::OnGround)
		{
			StanceFlags newflag = (StanceFlags)((int)BaseLocalPlayer.load()->GetPoseType() + (int)StanceFlags::OnGround);
			BaseLocalPlayer.load()->WritePoseType(MiscWriteScatterHandle, newflag);
			BaseLocalPlayer.load()->GetBaseMovement()->WriteGrounded(MiscWriteScatterHandle, 1.f);
			MiscWriteScatterCount++;
		}
	}
	BaseLocalPlayer.load()->GetBaseMovement()->SetCapsule(MiscWriteScatterHandle, ctx.m_settings.Movement_Misc_CapsuleBypass || FlyHacking);
	if (ctx.m_settings.Movement_Misc_NoSprintDelays)
	{
		// This is OmniSprint, Too intensive For DMA, It does speed the player up, call it no sprint delays
		if ((BaseLocalPlayer.load()->GetPoseType() & (int)StanceFlags::Sprinting) != (int)StanceFlags::Sprinting)
		{
			StanceFlags newflag = (StanceFlags)((int)BaseLocalPlayer.load()->GetPoseType() + (int)StanceFlags::Sprinting);
			BaseLocalPlayer.load()->WritePoseType(MiscWriteScatterHandle, StanceFlags::Sprinting);
			BaseLocalPlayer.load()->GetBaseMovement()->WriteRunning(MiscWriteScatterHandle, 1.f);
			MiscWriteScatterCount++;
		}
	}
	if (ctx.m_settings.Movement_Misc_InfiniteJump)
	{
		BaseLocalPlayer.load()->GetBaseMovement()->WriteCanJump(MiscWriteScatterHandle);
		MiscWriteScatterCount++;
	}
	if (ctx.m_settings.Movement_Misc_GravityModifier)
	{
		BaseLocalPlayer.load()->GetBaseMovement()->WriteGravityMultiplier(MiscWriteScatterHandle, ctx.m_settings.Movement_Misc_GravityModifierAmount);
		MiscWriteScatterCount++;
		RewriteGravity = true;
	}
	else
	{
		if (RewriteGravity)
		{
			BaseLocalPlayer.load()->GetBaseMovement()->WriteGravityMultiplier(MiscWriteScatterHandle, 2.5f);
			MiscWriteScatterCount++;
			RewriteGravity = false;
		}
	}
	if (ctx.m_settings.Movement_Misc_FakeLag)
	{
		// this should give us higher forgiveness
	/*	float lag = (float)(rand() % ctx.m_settings.Movement_Misc_FakeLagDelay) / 1000.f;
		srand(__rdtsc());
		if (lag < 0.1f)
			lag = 0.1f;
		PlayerConvarInstance.load()->WriteClientTickInterval(MiscWriteScatterHandle, lag);
		RewriteTickInterval = true;
		MiscWriteScatterCount++;*/
	}
	else
	{
		if (RewriteTickInterval)
		{
			
			PlayerConvarInstance.load()->WriteClientTickInterval(MiscWriteScatterHandle, 0.03125f);
			MiscWriteScatterCount++;
			RewriteTickInterval = false;
		}
	}
});

int Debugx = 0;
int Debugy = 0;
int Debugz = 0;
bool DebugCam = false;
float ManipulateAmount = 0;
int ManipulationMode = 0;
std::shared_ptr<CheatFunction> UpdateDebugCam = std::make_shared<CheatFunction>(0, []()
{
	if (BaseLocalPlayer.load() == nullptr)
		return;

	if (DebugCam)
	{
	//	if (ManipulationMode != 0)
	//		return;
		//	BaseLocalPlayer.load()->SetEyesPosition(MiscWriteScatterHandle);
	//	MiscWriteScatterCount++;
	//	BaseLocalPlayer.load()->WriteEyesPosition(MiscWriteScatterHandle, Vector3(Debugx, Debugy, Debugz));
	}
	else
	{
		Debugx = 0;
		Debugy = 0;
		Debugz = 0;
	}
});

std::shared_ptr<CheatFunction> DebugKeys = std::make_shared<CheatFunction>(100, []()
{
	if (!Input.load())
		return;
	if (!ctx.m_settings.Movement_Misc_InteractiveDebug || ManipulationMode != 0)
	{
		DebugCam = false;
		return;
	}
	if (Input.load()->IsKeyClicked(ctx.m_settings.Movement_Misc_DebugCamKey))
		DebugCam = !DebugCam;
	if (!DebugCam)
		return;
	// else ifs will help with performance here but only allows 1 key at a time
	if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_DebugUp))
	{
		Debugy += 1;
	}
	else if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_DebugDown))
	{
		Debugy -= 1;
	}

	else if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_DebugRight))
	{
		Debugx += 1;
	}
	else if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_DebugLeft))
	{
		Debugx -= 1;
	}

	else if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_DebugForward))
	{
		Debugz += 1;
	}
	else if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_DebugBackwards))
	{
		Debugz -= 1;
	}
});

std::shared_ptr<CheatFunction> ManipulationKeys = std::make_shared<CheatFunction>(100, []()
{
	if (!Input.load())
		return;
	if (!ctx.m_settings.Movement_Misc_Manipulation || DebugCam)
	{
		ManipulationMode = 0;
		return;
	}
	if (Input.load()->IsKeyClicked(ctx.m_settings.Movement_Misc_ManipulationKey))
	{
		if (ManipulationMode > 3)
		{
			ManipulationMode = 0;
		}
		ManipulationMode++;
		if (ManipulationMode == 1)
		{
			ManipulateAmount = 1.5;
		}
		else if (ManipulationMode == 2)
		{
			ManipulateAmount = -1.5;
		}
		else if (ManipulationMode == 3)
		{
			ManipulateAmount = 1.1;
		}
	}
	if (ManipulationMode == 0)
		return;

	if (Input.load()->IsKeyClicked(ctx.m_settings.Movement_Misc_IncreaseManipulation))
	{
		if (ManipulationMode == 1 && ManipulateAmount != 1.5)
		{
			ManipulateAmount += 0.1;
		}
		else if (ManipulationMode == 2 && ManipulateAmount != 0)
		{
			ManipulateAmount += 0.1;
		}
		else if (ManipulationMode == 3 && ManipulateAmount != 1.1)
		{
			ManipulateAmount += 0.1;
		}
	}
	else if (Input.load()->IsKeyClicked(ctx.m_settings.Movement_Misc_DecreaseManipulation))
	{
		if (ManipulationMode == 1 && ManipulateAmount != 0)
		{
			ManipulateAmount -= 0.1;
		}
		else if (ManipulationMode == 2 && ManipulateAmount != -1.5)
		{
			ManipulateAmount -= 0.1;
		}
		else if (ManipulationMode == 3 && ManipulateAmount != 0)
		{
			ManipulateAmount -= 0.1;
		}
	}
});
std::shared_ptr<CheatFunction> Manipulate = std::make_shared<CheatFunction>(5, []()
{
	if (!ctx.m_settings.Movement_Misc_Manipulation)
		return;
	if (DebugCam)
		return;
	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (ManipulationMode == 1)
	{
		MiscWriteScatterCount++;
		BaseLocalPlayer.load()->WriteEyesPosition(MiscWriteScatterHandle, Vector3(ManipulateAmount, 0, 0));
	}
	if (ManipulationMode == 2)
	{
		MiscWriteScatterCount++;
		BaseLocalPlayer.load()->WriteEyesPosition(MiscWriteScatterHandle, Vector3(ManipulateAmount, 0, 0));
	}
	if (ManipulationMode == 3)
	{
		MiscWriteScatterCount++;
		BaseLocalPlayer.load()->WriteEyesPosition(MiscWriteScatterHandle, Vector3(0, ManipulateAmount, 0));
	}
});
std::shared_ptr<CheatFunction> SpeedKeyManager = std::make_shared<CheatFunction>(50, []()
	{
		if (!ctx.m_settings.Movement_Misc_SpeedHack)
			return;
		if (ctx.m_settings.Movement_Misc_SpeedHack_UseKey)
		{
			if (Input.load()->IsKeyDown(ctx.m_settings.Movement_Misc_SpeedHack_Key))
			{
				SpeedHacking = true;
			
			}
			else
			{
				SpeedHacking = false;
			}
			return;
		}
		SpeedHacking = true;
	});
void SpeedHack()
{
	if (!ctx.m_settings.Movement_Misc_SpeedHack)
		return;
	if (FlyHacking)
		return;
	if(!SpeedHacking)
				return;
	Vector4 euler = BaseLocalPlayer.load()->GetEyeAngles();
	Vector3 newpos;

	if (Input.load()->IsKeyDown(87)) // W
	{
		newpos = newpos + Vector4::Mult(euler, Vector3(0, 0, 1));
	}
	if (Input.load()->IsKeyDown(83)) // S
	{
		newpos = newpos - Vector4::Mult(euler, Vector3(0, 0, 1));
	}
	if (Input.load()->IsKeyDown(65)) // A
	{
		newpos = newpos - Vector4::Mult(euler, Vector3(1, 0, 0));
	}
	if (Input.load()->IsKeyDown(68)) // D
	{
		newpos = newpos + Vector4::Mult(euler, Vector3(1, 0, 0));
	}

	auto handle = mem.CreateScatterHandle();
	if (newpos != Vector3::Zero())
	{
		newpos.x *= ctx.m_settings.Movement_Misc_SpeedHack_Speed;
		newpos.z *= ctx.m_settings.Movement_Misc_SpeedHack_Speed;
			newpos.y = 0;
		BaseLocalPlayer.load()->GetBaseMovement()->WriteTargetMovement(handle, newpos);

	}


	mem.ExecuteWriteScatter(handle);
	mem.CloseScatterHandle(handle);
}
void Noclip()
{
	if (!ctx.m_settings.Movement_Misc_FlyHack)
		return;
	if (BaseLocalPlayer.load()->GetBaseMovement() == nullptr)
		return;
	if (Input.load()->IsKeyClicked(ctx.m_settings.Movement_Misc_FlyHack_Key))
	{
		FlyHacking = !FlyHacking;
		if (!FlyHacking)
		{
			auto handle = mem.CreateScatterHandle();
			if (ctx.m_settings.Movement_Misc_GravityModifier)
			{

				BaseLocalPlayer.load()->GetBaseMovement()->WriteGravityMultiplier(handle, (ctx.m_settings.Movement_Misc_GravityModifierAmount));

			}
			else
			{
				BaseLocalPlayer.load()->GetBaseMovement()->WriteGravityMultiplier(handle, 2.5f);
			}
			BaseLocalPlayer.load()->GetBaseMovement()->WriteFlying(handle, false);
			BaseLocalPlayer.load()->GetBaseMovement()->WriteJumpTime(handle, 0.0f);
			mem.ExecuteWriteScatter(handle);
			mem.CloseScatterHandle(handle);
		}
	}
	if (!FlyHacking)
		return;
		UpdateCapsule->Execute();
		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->GetBaseMovement()->WriteFlying(handle, true);
		
			BaseLocalPlayer.load()->GetBaseMovement()->WriteGravityMultiplier(handle, 0.0f);
			BaseLocalPlayer.load()->GetBaseMovement()->WriteGroundTime(handle, 0.f);
	
			BaseLocalPlayer.load()->GetBaseMovement()->WriteJumpTime(handle, 9999999.f);
			if (!ctx.m_settings.Movement_Misc_FlyHack_NoLadder)
			{
			if (!ctx.m_settings.Movement_Misc_Spiderman)
			{
				BaseLocalPlayer.load()->GetBaseMovement()->WriteGroundAngleNew(handle, 0.f);
				BaseLocalPlayer.load()->GetBaseMovement()->WriteMaxAngleWalking(handle, 100.f);
				BaseLocalPlayer.load()->GetBaseMovement()->WriteGroundAngle(handle, 0.f);
			}
			if ((BaseLocalPlayer.load()->GetPoseType() & (int)StanceFlags::OnLadder) != (int)StanceFlags::OnLadder)
			{
				StanceFlags newflag = (StanceFlags)((int)BaseLocalPlayer.load()->GetPoseType() + (int)StanceFlags::OnLadder);
				BaseLocalPlayer.load()->WritePoseType(MiscWriteScatterHandle, StanceFlags::OnLadder);
				MiscWriteScatterCount++;
			}
		}
		mem.ExecuteWriteScatter(handle);
		mem.CloseScatterHandle(handle);
		handle = mem.CreateScatterHandle();
		Vector4 euler = BaseLocalPlayer.load()->GetEyeAngles();
		Vector3 newpos;

		if (Input.load()->IsKeyDown(87)) // W
		{
			newpos = newpos + Vector4::Mult(euler, Vector3(0, 0, 1));
		}
		if (Input.load()->IsKeyDown(83)) // S
		{
			newpos = newpos - Vector4::Mult(euler, Vector3(0, 0, 1));
		}
		if (Input.load()->IsKeyDown(65)) // A
		{
			newpos = newpos - Vector4::Mult(euler, Vector3(1, 0, 0));
		}
		if (Input.load()->IsKeyDown(68)) // D
		{
			newpos = newpos + Vector4::Mult(euler, Vector3(1, 0, 0));
		}

		float speed = 2.5f;
		if (Input.load()->IsKeyDown(16)) // Shift
		{
			speed = 5.0f;
		}
		if (Input.load()->IsKeyDown(17)) // Ctrl
		{
			speed = 1.5f;
		}

		if (newpos != Vector3::Zero())
		{
			newpos.x *= speed;
			newpos.z *= speed;
			if (newpos.y > 1)
				newpos.y = 0;
			BaseLocalPlayer.load()->GetBaseMovement()->WriteTargetMovement(handle, newpos);

		}

		
		mem.ExecuteWriteScatter(handle);
		mem.CloseScatterHandle(handle);
	
}
bool AntiAimLeft = false;
bool AntiAimRight = false;
bool AntiAimBack = true;
bool AntiAimToggle = true;
void DrawAntiAim()
{
	if (!ctx.m_settings.Movement_Misc_AntiAim)
		return;
	if (!AntiAimToggle)
		return;
	Vector2 centreofscreen = ctx.m_settings.Overlay_OverRideResolution ? Vector2(ctx.m_settings.Overlay_ScreenWidth / 2, ctx.m_settings.Overlay_ScreenHeight / 2) : Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	auto drawlist = ImGui::GetBackgroundDrawList();
	ImColor col = ctx.m_settings.Movement_Misc_AntiAim_Indicator_Colour;
	ImU32 colour = col;
	if (ctx.m_settings.Movement_Misc_AntiAim_Mode == 1)
	{
		
		if (!AntiAimLeft)
		{
			ImVec2 p1 = ImVec2(centreofscreen.x - 100, centreofscreen.y);
			ImVec2 p2 = ImVec2(p1.x + 20, p1.y - 20);
			ImVec2 p3 = ImVec2(p1.x + 20, p1.y + 20);
			drawlist->AddTriangle(p1, p2, p3, colour, 2);
		}
		if (AntiAimLeft)
		{
			ImVec2 p1 = ImVec2(centreofscreen.x - 100, centreofscreen.y);
			ImVec2 p2 = ImVec2(p1.x + 20, p1.y - 20);
			ImVec2 p3 = ImVec2(p1.x + 20, p1.y + 20);
			drawlist->AddTriangleFilled(p1, p2, p3, colour);
		}
		if (!AntiAimRight)
		{
			ImVec2 p1 = ImVec2(centreofscreen.x + 100, centreofscreen.y);
			ImVec2 p2 = ImVec2(p1.x - 20, p1.y + 20);
			ImVec2 p3 = ImVec2(p1.x - 20, p1.y - 20);
			drawlist->AddTriangle(p1, p2, p3, colour, 2);
		}
		if (AntiAimRight)
		{
			ImVec2 p1 = ImVec2(centreofscreen.x + 100, centreofscreen.y);
			ImVec2 p2 = ImVec2(p1.x - 20, p1.y + 20);
			ImVec2 p3 = ImVec2(p1.x - 20, p1.y - 20);
			drawlist->AddTriangleFilled(p1, p2, p3, colour);
		}
		if (!AntiAimBack)
		{
			ImVec2 p1 = ImVec2(centreofscreen.x, centreofscreen.y + 100);
			ImVec2 p2 = ImVec2(p1.x - 20, p1.y - 20);
			ImVec2 p3 = ImVec2(p1.x + 20, p1.y - 20);
			drawlist->AddTriangle(p1, p2, p3, colour, 2);
		}
		if (AntiAimBack)
		{
			ImVec2 p1 = ImVec2(centreofscreen.x, centreofscreen.y + 100);
			ImVec2 p2 = ImVec2(p1.x - 20, p1.y - 20);
			ImVec2 p3 = ImVec2(p1.x + 20, p1.y - 20);
			drawlist->AddTriangleFilled(p1, p2, p3, colour);
		}
	}
	else
	{
		drawlist->AddText(ImVec2(centreofscreen.x, centreofscreen.y - 500), colour, xorstr_("Anti Aim Enabled"));
	}
}
std::shared_ptr<CheatFunction> AntiAimKeyManager = std::make_shared<CheatFunction>(10, []()
	{
		if (Input.load()->IsKeyClicked(ctx.m_settings.Movement_Misc_AntiAim_Hotkey))
		{
			AntiAimToggle = !AntiAimToggle;

		}
		if (AntiAimToggle)
		{
		
		if (Input.load()->IsKeyDown(VK_LEFT))
		{
			AntiAimLeft = true;
			AntiAimRight = false;
			AntiAimBack = false;
		}
		if (Input.load()->IsKeyDown(VK_RIGHT))
		{
			AntiAimRight = true;
			AntiAimLeft = false;
			AntiAimBack = false;
		}
		if (Input.load()->IsKeyDown(VK_DOWN))
		{
			AntiAimBack = true;
			AntiAimRight = false;
			AntiAimLeft = false;

		}
		
		}
	});
inline bool HasAntiAimed = false;
void AntiAim()
{
	if (!ctx.m_settings.Movement_Misc_AntiAim && HasAntiAimed)
	{
		HasAntiAimed = false;
		auto handle = mem.CreateScatterHandle();
		PlayerConvarInstance.load()->WriteClientTickRate(handle, 32);
		mem.ExecuteWriteScatter(handle);
		mem.CloseScatterHandle(handle);
	}
	if(!ctx.m_settings.Movement_Misc_AntiAim)
		return;
	HasAntiAimed = true;
	AntiAimKeyManager->Execute();
	if(!AntiAimToggle)
		return;
	// 32
	if (ctx.m_settings.Movement_Misc_AntiAim_Mode == 1)
	{
		Vector4 rotation;
		rotation = BaseLocalPlayer.load()->GetEyeAngles();
		float yaw = (atan2(-2 * (rotation.x * rotation.z - rotation.w * rotation.y), rotation.w * rotation.w - rotation.x * rotation.x - rotation.y * rotation.y + rotation.z * rotation.z)) * (180 / 3.14159265358979323846f);
		
		if (AntiAimLeft)
		{
			auto handle = mem.CreateScatterHandle();
			BaseLocalPlayer.load()->WriteNetworkedAngles(handle, Vector3(130, yaw + 90, 0));
			PlayerConvarInstance.load()->WriteClientTickRate(handle, 64);	
			mem.ExecuteWriteScatter(handle);
			mem.CloseScatterHandle(handle);
		}
		if (AntiAimRight)
		{
			auto handle = mem.CreateScatterHandle();
			BaseLocalPlayer.load()->WriteNetworkedAngles(handle, Vector3(130, yaw - 90, 0));
			PlayerConvarInstance.load()->WriteClientTickRate(handle, 64);
			mem.ExecuteWriteScatter(handle);
			mem.CloseScatterHandle(handle);
		}
		if (AntiAimBack)
		{
			auto handle = mem.CreateScatterHandle();
			BaseLocalPlayer.load()->WriteNetworkedAngles(handle, Vector3(130, yaw, 0));
			PlayerConvarInstance.load()->WriteClientTickRate(handle, 64);
			mem.ExecuteWriteScatter(handle);
			mem.CloseScatterHandle(handle);
		}
	}
	else
	{
		srand(__rdtsc());
		int randnum = rand() % 1999 - 999;
		auto handle = mem.CreateScatterHandle();
		BaseLocalPlayer.load()->WriteNetworkedAngles(handle, Vector3(130, randnum, 0));
		PlayerConvarInstance.load()->WriteClientTickRate(handle, 96);
		mem.ExecuteWriteScatter(handle);
		mem.CloseScatterHandle(handle);
	}
}
void MovementFunctions()
{

	if (BaseLocalPlayer.load() == nullptr)
		return;
	if (BaseLocalPlayer.load()->GetBaseMovement() == nullptr)
		return;
	AntiAim();
	SpeedKeyManager->Execute();
	SpeedHack();
	Noclip();
	UpdateCapsule->Execute();
	UpdateDebugCam->Execute();
	DebugKeys->Execute();
	RemoveFallDamage->Execute();
//	ManipulationKeys->Execute();
//	Manipulate->Execute();
	Movement->Execute();
	OmniSprint();
}
