#pragma once
class TODSky
{
	/*
	"Address": 54842648,
      "Name": "TOD_Sky_TypeInfo",
      "Signature": "TOD_Sky_c*"
	*/
	uint64_t Class = 166538160;
	uint64_t StaticField = 0xB8;
	uint64_t NightParameters = 0x60; // public TOD_NightParameters Night;
	uint64_t DayParameters = 0x58; // 	public TOD_DayParameters Day;
	uint64_t Instance = 0;
	uint64_t AmbientMultiplierDay = 0x50; // public float AmbientMultiplier;
	uint64_t AmbientMultiplierNight = 0x50; // public float AmbientMultiplier;
	uint64_t LightIntensityDay = 0x48; // public float LightIntensity;
	uint64_t LightIntensityNight = 0x48; // public float LightIntensity;
	uint64_t AmbientParamaters = 0x98; // public TOD_AmbientParameters Ambient;

	uint64_t Components = 0xB0; // private TOD_Components <Components>k__BackingField;
	uint64_t Cycle = 0x40;  // public TOD_CycleParameters Cycle;
	uint64_t HourOffset = 0x10; // public float HourOffset;
	uint64_t UpdateInterval = 0x18; //TOD_AmbientParameters -> public float UpdateInterval;
	uint64_t Saturation = 0x14; //TOD_AmbientParameters -> public float Saturation;
	float Hour = 0;
	float ActiveNightLightIntensity;
	float ActiveDayAmbientMultiplier;
public:
	TODSky();
	void WriteNightLightIntensity(VMMDLL_SCATTER_HANDLE handle,float value);
	void WriteNightAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteDayAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteSaturation(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteUpdateInterval(VMMDLL_SCATTER_HANDLE handle, float value);
	void SetHour(VMMDLL_SCATTER_HANDLE handle);
	void WriteHour(float time);
	float GetDayAmbientMultiplier();
	float GetNightLightIntensity();
	void UpdateNightLightIntensity(VMMDLL_SCATTER_HANDLE handle);
	void UpdateDayAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle);
	int GetHour();


};