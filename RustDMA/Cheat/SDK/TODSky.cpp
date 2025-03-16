#include "Pch.h"
#include "Globals.h"
#include "TODSky.h"

TODSky::TODSky()
{
	//printf("[TODSky] Initialized\n");
	Class = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class);
	//printf("[TODSky] Class: 0x%llX\n", Class);
	StaticField = mem.Read<uint64_t>(Class + StaticField); // static field and 0x0 is 	private static List<TOD_Sky> instances;
	uint64_t list = mem.Read<uint64_t>(StaticField + 0x0);
	uint64_t intancevalues = mem.Read<uint64_t>(list + 0x10); // 0x10 = list values
	Instance = mem.Read<uint64_t>(intancevalues + 0x20); // first value in instances list
	if (Instance == 0)
		return;
//	printf("[TODSky] Instance: 0x%llX\n", Instance);
	NightParameters = mem.Read<uint64_t>(Instance + NightParameters);
	DayParameters = mem.Read<uint64_t>(Instance + DayParameters);
	Components = mem.Read<uint64_t>(Instance + Components);
	//Scatter = mem.Read<uint64_t>(Components + Scatter);
	//ScatterMaterial = mem.Read<uint64_t>(Scatter + ScatterMaterial);
	Cycle = mem.Read<uint64_t>(Instance + Cycle);
	//printf("[TODSky] Night Parameters: 0x%llX\n", Cycle);
	if (Instance != 0)
	{
		AmbientParamaters = mem.Read<uint64_t>(Instance + AmbientParamaters);
	}
	

}


void TODSky::WriteNightLightIntensity(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(NightParameters ==0)
				return;
	mem.AddScatterWriteRequest<float>(handle, NightParameters + LightIntensityNight, value);
	//	printf("[TODSky] Failed to write Night Light Intensity\n");
}

void TODSky::WriteNightAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(NightParameters ==0)
						return;
	mem.AddScatterWriteRequest<float>(handle, NightParameters + AmbientMultiplierNight, value);
	//	printf("[TODSky] Failed to write Night Ambient Multiplier\n");
}

void TODSky::WriteDayAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(DayParameters ==0)
						return;
	mem.AddScatterWriteRequest<float>(handle, DayParameters + AmbientMultiplierDay, value);
	//		printf("[TODSky] Failed to write Day Ambient Multiplier\n");
}

void TODSky::SetHour(VMMDLL_SCATTER_HANDLE handle)
{
	if(Cycle ==0)
		return;
	mem.AddScatterReadRequest(handle, Cycle + HourOffset, reinterpret_cast<void*>(&Hour), sizeof(float));
}
void TODSky::WriteHour(float time)
{
	if (Cycle == 0)
		return;
	mem.Write < float > (Cycle + HourOffset, time);
}

float TODSky::GetNightLightIntensity()
{
	return ActiveNightLightIntensity;
}

float TODSky::GetDayAmbientMultiplier()
{
	return ActiveDayAmbientMultiplier;
}

void TODSky::WriteSaturation(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(AmbientParamaters ==0)
		return;
	mem.AddScatterWriteRequest<float>(handle, AmbientParamaters + Saturation, value);
}

void TODSky::WriteUpdateInterval(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(AmbientParamaters ==0)
		return;
	mem.AddScatterWriteRequest<float>(handle, AmbientParamaters + UpdateInterval, value);
}

int TODSky::GetHour()
{
	return Hour;
}

void TODSky::UpdateNightLightIntensity(VMMDLL_SCATTER_HANDLE handle)
{
	if(NightParameters ==0)
		return;
	mem.AddScatterReadRequest(handle, NightParameters + LightIntensityNight, reinterpret_cast<void*>(&ActiveNightLightIntensity), sizeof(float));
}

void TODSky::UpdateDayAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle)
{
	if(DayParameters ==0)
		return;
	mem.AddScatterReadRequest(handle, DayParameters + AmbientMultiplierDay, reinterpret_cast<void*>(&ActiveDayAmbientMultiplier), sizeof(float));
}
