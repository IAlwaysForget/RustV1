#include "pch.h"
#include "World.h"
#include <xorstr.hpp>
#include <DMALibrary/Memory/Memory.h>

World::World()
{
//	printf("[World] Initialized\n");
	Class = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class);
//	printf("[World] World: 0x%llX\n", Class);
	StaticField = mem.Read<uint64_t>(Class + StaticField);
	printf("[World] Static Fields: 0x%llX\n", StaticField);
	if (StaticField != 0)
	{
		Size = mem.Read<uint32_t>(StaticField + Size);
	//	printf("[World] Size: 0x%llX\n", Size);
		Seed = mem.Read<uint32_t>(StaticField + Seed);
		//printf("[World] Seed: 0x%llX\n", Seed);
	}
}

bool World::IsValid()
{
	return StaticField != 0;
}
int World::GetSize()
{
	return (int)Size;
}
int World::GetSeed()
{
	return (int)Seed;
}