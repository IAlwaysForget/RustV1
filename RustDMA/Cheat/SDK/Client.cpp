#include "pch.h"
#include "Client.h"
#include "Globals.h"

Client::Client()
{
   // printf("[Client] Initialized\n");
   // uint64_t client = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
  //  printf("[Client] Client: 0x%llX\n", client);
   // this->StaticField = mem.Read<uint64_t>(client + StaticField); // Set Static Padding
//    printf("[Client] Static Fields: 0x%llX\n", StaticField);
}

void Client::WriteCamSpeed(VMMDLL_SCATTER_HANDLE handle, float speed)
{
  //  mem.AddScatterWriteRequest<float>(handle, this->StaticField + this->CamSpeed, speed);
}

void Client::WriteCamZoomSpeed(VMMDLL_SCATTER_HANDLE handle, float speed)
{
   // mem.AddScatterWriteRequest<float>(handle, this->StaticField + this->CamZoomSpeed, speed);
}

void Client::WriteCamLookSpeed(VMMDLL_SCATTER_HANDLE handle, float speed)
{
   // mem.AddScatterWriteRequest<float>(handle, this->StaticField + this->CamLookSpeed, speed);
}

void Client::WriteCamFov(VMMDLL_SCATTER_HANDLE handle, float fov)
{
   // mem.AddScatterWriteRequest<float>(handle, this->StaticField + this->CamFov, fov);
}

void Client::WriteClientTickInterval(VMMDLL_SCATTER_HANDLE handle, float value)
{
  //  printf("[Client] ClientTickInterval: 0x%llX\n", this->StaticField );
  //  mem.AddScatterWriteRequest<float>(handle, this->StaticField + this->ClientTickInterval, value);
}