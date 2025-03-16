#include "pch.h"
#include "PlayerConvar.h"
#include "Globals.h"

PlayerConvar::PlayerConvar()
{
  //  printf("[Player Convar] Initialized\n");
   // uint64_t client = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
   // printf("[Player Convar] Player Convar: 0x%llX\n", client);
    //this->StaticField = mem.Read<uint64_t>(client + StaticField); // Set Static Padding
    //printf("[Player Convar] Static Fields: 0x%llX\n", StaticField);
}


void PlayerConvar::WriteClientTickInterval(VMMDLL_SCATTER_HANDLE handle, float value)
{
  //  mem.AddScatterWriteRequest<float>(handle, this->StaticField + this->ClientTickInterval, value);
}
void PlayerConvar::WriteClientTickRate(VMMDLL_SCATTER_HANDLE handle, float value)
{
   // mem.AddScatterWriteRequest<int>(handle, this->StaticField + this->ClientTickRate, value);
}