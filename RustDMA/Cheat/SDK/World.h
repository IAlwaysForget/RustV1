#pragma once
/*
	  "Address": 60470408,
	  "Name": "World_TypeInfo",
	  "Signature": "World_c*"

	  //Search for worldconfig, check where its being used, also 	public Action<string> statusFunction;
*/
class World
{
	uint64_t Class = 166226656;
	uint64_t StaticField = 0xB8; // Static Padding To Access Static Fields
	uint64_t Seed = 0x18c; //	private static uint <Seed>k__BackingField;
	uint32_t Size = 0x2d4; // private static uint _size;
public:
	World();
	bool IsValid();
	int GetSize();
	int GetSeed();
};