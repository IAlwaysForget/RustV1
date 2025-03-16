#pragma once
#include "xorstr.hpp"

class ConsoleSystem
{
	/*
      "Address": 55086408,
      "Name": "ConsoleSystem.Index_TypeInfo",
      "Signature": "ConsoleSystem_Index_c*"
	*/
	uint64_t Class = 166368400;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8; // Static Padding To Access Static Fields
	uint64_t AllBackingField = 0x10; //private static ConsoleSystem.Command[] <All>k__BackingField;
	uint32_t ListSize = 0x18;
	//
	// public class Command offsets
	uint64_t AllowRunFromServer = 0x60; // public bool allowRunFromServer;
	uint64_t Name = 0x10; // public string Name;
	std::vector<std::wstring> BlacklistedCommands = {xorstr_(L"noclip"),xorstr_(L"infiniteammo"),xorstr_(L"god"),xorstr_(L"camspeed"),xorstr_(L"camzoomlerp"),xorstr_(L"camzoomspeed"),xorstr_(L"debugcamera_fov"),xorstr_(L"camlockspeed"),xorstr_(L"camlerp"),xorstr_(L"camlookspeed")
		,xorstr_(L"camfov"),xorstr_(L"camlerptilt"),xorstr_(L"camoffset"),xorstr_(L"debugcamera_unfreeze"),xorstr_(L"debugcamera_guide"),xorstr_(L"debugcamera") };
	bool Blocked = false;

public:
	ConsoleSystem();
	void DisableCommand(uint64_t command);
	bool CommandsBlocked();
};
