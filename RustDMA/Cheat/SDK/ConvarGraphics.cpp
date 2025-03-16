#include "Pch.h"
#include "ConvarGraphics.h"
#include "Globals.h"
struct tinnertest {
uint64_t fov;
};
ConvarGraphics::ConvarGraphics()
{
	//printf("[ConvarGraphics] Initialized\n");
//	uint64_t graphics = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
//	printf("[ConvarGraphics] ConvarGraphics: 0x%llX\n", graphics);
//	this->StaticField = mem.Read<uint64_t>(graphics + StaticField); // Set Static Padding
	//printf("[ConvarGraphics] Static Fields: 0x%llX\n", StaticField);
	//346,819,079 = 90
	// 3466981359 = 70
	// 3468685259 = 80

	//-826675217 = 90
	// -826282001 = 80
	// -827985937 = 70

}

void ConvarGraphics::WriteFOV(float fov)
{
	
	
	//mem.Write<float>(StaticField + FOV, Encryption.load()->EncryptFloatFOV(fov));
	//printf("[ConvarGraphics] Failed to write FOV\n");
}

float ConvarGraphics::ReadFOV()
{
	//return mem.Read<float>(StaticField + FOV);
	return 0;
}
