#include "Pch.h"
#include "OcclusionCulling.h"
#include "Globals.h"

OcclusionCulling::OcclusionCulling()
{
//	printf("[OcclusionCulling] Initialized\n");
	uint64_t convarculling = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
//	printf("[OcclusionCulling] OcclusionCulling: 0x%llX\n", convarculling);
	this->StaticField = mem.Read<uint64_t>(convarculling + StaticField); // Set Static Padding
//	printf("[OcclusionCulling] Static Fields: 0x%llX\n", StaticField);
//	mem.Write<DebugFilter>(StaticField + DebugShow, DebugFilter::Dynamic);
	this->Instance = mem.Read<uint64_t>(StaticField + Instance); // instance 
//	printf("[OcclusionCulling] Instance: 0x%llX\n", Instance);
	this->DebugSettings = mem.Read<uint64_t>(Instance + DebugSettings); // debugSettings
//	printf("[OcclusionCulling] DebugSettings: 0x%llX\n", Instance);

//	WriteDebugSettings(DebugFilter::Dynamic);
//	WriteLayerMask(131072);
}

void OcclusionCulling::WriteLayerMask(int mask)
{
	/*	if (!mem.Write<int>(this->DebugSettings + LayerFilter, mask))
	{
		std::printf("[OcclusionCulling] Mask Write Failure");
	}*/
}

void OcclusionCulling::WriteDebugSettings(DebugFilter debugfilter)
{
/*	if (!mem.Write<DebugFilter>(this->StaticField + DebugShow, debugfilter))
	{
		std::printf("[OcclusionCulling] Debug Settings Write Failure");
	}*/
}
