#include "Pch.h"
#include "ConvarAdmin.h"
#include "Globals.h"

ConvarAdmin::ConvarAdmin()
{
	//printf("[ConvarAdmin] Initialized\n");
	//uint64_t graphics = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
	//printf("[ConvarAdmin] ConvarAdmin: 0x%llX\n", graphics);
//	this->StaticField = mem.Read<uint64_t>(graphics + StaticField); // Set Static Padding
	//printf("[ConvarAdmin] Static Fields: 0x%llX\n", StaticField);
}

void ConvarAdmin::ClearVisionInWater(bool value)
{
	//if (!mem.Write<bool>(StaticField + AdminUnderWaterEffect, !value))
	//	printf("[ConvarAdmin] Failed to write AdminUnderWaterEffect\n");
}	

void ConvarAdmin::SetAdminTime(float value)
{
	//if (value == 0)
	//	value = -1; // this is just easier to manage with a slider
//	if (!mem.Write<float>(StaticField + AdminTime, value))
	//	printf("[ConvarAdmin] Failed to write AdminTime\n");
}
