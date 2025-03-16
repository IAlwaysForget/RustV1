#pragma once
class ConvarGraphics //Convar.Graphics
{
	/*
	Script.json
	  "Address": 54876232,
	  "Name": "ConVar.Graphics_TypeInfo",
	  "Signature": "ConVar_Graphics_c*"
	// search for public static Func<IGrouping<Vector2Int, Resolution>, Resolution>
	[ConsoleSystem.Factory("graphics")]
	*/
	uint64_t Class = 63434136;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t FOV = 0x8;// private static float _fov;
public:
	ConvarGraphics();
	void WriteFOV(float value);
	float ReadFOV();
};