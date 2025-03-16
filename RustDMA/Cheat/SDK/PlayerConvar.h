#pragma once
#include <DMALibrary/Memory/Memory.h>
class PlayerConvar
{
	/*
	  "Address": 63165216,
      "Name": "ConVar.Player_TypeInfo",
      "Signature": "ConVar_Player_c*"

	  Go to search in dnspy. Click the options arro and turn on "match whole words", "Case Sensitive Search" Should be one of the top results
	  ffcc6b4cafa86407d479e66a79405044d28cadf3

	  [ConsoleSystem.Factory("player")]

	*/
	uint64_t Class = 63360632;
	uint64_t StaticField = 0xb8;
	uint64_t ClientTickRate = 0x4; // public static int clientTickRate;
	uint64_t ClientTickInterval = 0x30; //	public static float clientTickInterval;

public:
	PlayerConvar();
	void WriteClientTickInterval(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteClientTickRate(VMMDLL_SCATTER_HANDLE handle, float value);
};