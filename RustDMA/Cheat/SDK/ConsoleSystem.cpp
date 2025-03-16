#include "Pch.h"
#include "consolesystem.h"
#include "Globals.h"

void ConsoleSystem::DisableCommand(uint64_t command)
{
	uintptr_t commandname = mem.Read<uint64_t>(command + Name);

	wchar_t commandstring[36] = {'\0'};
	mem.Read((ULONG64)commandname + 0x14, reinterpret_cast<void*>(&commandstring), sizeof(commandstring));
	for (std::wstring blacklistedcommand : BlacklistedCommands)
	{
		//bool blocked = mem.Read<bool>(command + AllowRunFromServer);

		if (wcscmp(commandstring, blacklistedcommand.c_str()) == 0)
		{
			//printf("[ConsoleSystem] ");
			//wprintf(commandstring);
			mem.Write<bool>(command + AllowRunFromServer, false);
			continue;
		}
		///if(blocked)
		//	printf("[ConsoleSystem] %ls\n", commandstring);
	}
	Blocked = true;
}

ConsoleSystem::ConsoleSystem()
{
	//printf("[ConsoleSystem] Initialized\n");
	uint64_t console = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class); // Get Class Start Address
	//	printf("[ConsoleSystem] Console: 0x%llX\n", console);
	//printf("[ConsoleSystem] Console: 0x%llX\n", console);
	this->StaticField = mem.Read<uint64_t>(console + StaticField);
	//	printf("[ConsoleSystem] Static Fields: 0x%llX\n", StaticField);
	//	printf("[ConsoleSystem] Static Fields: 0x%llX\n", StaticField);
	this->AllBackingField = mem.Read<uint64_t>(StaticField + AllBackingField);
	//	printf("[ConsoleSystem] AllBackingField: 0x%llX\n", AllBackingField);
	//printf("[ConsoleSystem] AllBackingField: 0x%llX\n", AllBackingField);
	this->ListSize = mem.Read<uint32_t>(AllBackingField + ListSize);
	//printf("[ConsoleSystem] ListSize: 0x%llX\n", ListSize);

	//	printf("[ConsoleSystem] ListSize: 0x%llX\n", ListSize);
	// read size from 0x18

	for (int i = 0; i < ListSize; i++)
	{
		uint64_t command = mem.Read<uint64_t>(AllBackingField + (0x20 + (i * 0x8)));
		if (command == 0x0)
			continue;
		DisableCommand(command);
	}
}

bool ConsoleSystem::CommandsBlocked()
{
	return Blocked;
}
