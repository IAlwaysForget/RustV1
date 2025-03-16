#include "Pch.h"
#include "maincamera.h"
#include "Globals.h"

MainCamera::MainCamera()
{
//	printf("[MainCamera] Initialized\n");
	uint64_t maincamera = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
	//printf("[MainCamera] MainCamera: 0x%llX\n", maincamera);
	this->StaticField = mem.Read<uint64_t>(maincamera + StaticField); // Set Static Padding
	//printf("[MainCamera] Static Fields: 0x%llX\n", StaticField);
	this->Camera = mem.Read<uint64_t>(StaticField + Camera); // Current MainCamera
//	printf("[MainCamera] Camera: 0x%llX\n", Camera);
	this->CameraGamoObject = mem.Read<uint64_t>(Camera + CameraGamoObject); // get the native gameobject
//	printf("[MainCamera] CameraGamoObject: 0x%llX\n", CameraGamoObject);
}

bool MainCamera::IsValid()
{
	return Camera != 0;
}

ViewMatrix MainCamera::GetViewMatrix()
{
	ViewMatrix viewmatrix;
	viewmatrix = mem.Read<ViewMatrix>(CameraGamoObject + ViewMatrixOffset);
	//printf("[MainCamera] ViewMatrix: %f %f %f %f\n", viewmatrix.matrix[0][0], viewmatrix.matrix[0][1], viewmatrix.matrix[0][2], viewmatrix.matrix[0][3]);

	return viewmatrix;
}

void MainCamera::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	mem.AddScatterReadRequest(handle, CameraGamoObject + PositionOffset, &Position, sizeof(Vector3));
}

Vector3 MainCamera::GetPosition()
{
	return Position;
}
