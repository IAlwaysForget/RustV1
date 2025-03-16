#pragma once
#include <DMALibrary/libs/vmmdll.h>
#include "Misc/vector.h"

class MainCamera
{
	/*
	Script.json
	"Address": 55074032,
	  "Name": "MainCamera_TypeInfo",
	  "Signature": "MainCamera_c*"
	*/
	uint64_t Class = 165867736;
	uint64_t StaticField = 0xB8; // Static Padding To Access Static Fields
	uint64_t Camera = 0x90; // public static Camera mainCamera
	uint64_t CameraGamoObject = 0x10;
	uint64_t ViewMatrixOffset = 0x30C;
	uint64_t PositionOffset = 0x454;
	Vector3 Position;
	ViewMatrix CacheMatrix;

public:
	MainCamera();
	bool IsValid();
	ViewMatrix GetViewMatrix();
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetPosition();
};
