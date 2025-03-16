#include "Pch.h"
#include "Encryption.h"
#include "Globals.h"
#include <array>
#include "misc/CheatFunction.h"
#include "Zydis/Zydis.h"
#include <iostream>
#include <Windows.h>
#include <minwindef.h>
inline bool PlayerDataFoundElsewhere = false;
inline bool PlayerFoundElsewhere = false;
#define __PAIR64__(hi, lo) ((static_cast<uint64_t>(hi) << 32) | (lo))
#define LODWORD(x) (*((uint32_t*)&(x)))
uint32_t __ROR4__(uint32_t value, unsigned int rotate)
{
	return (value >> rotate) | (value << (32 - rotate));
}

inline uint32_t ROR(uint32_t Value, uint32_t Count)
{
	uint32_t ValueMask = (8 * sizeof(Value) - 1);
	Count &= ValueMask;
	return (Value >> Count) | (Value << ((ValueMask + 1 - Count) & ValueMask));
}

inline bool TEST_BITD(uint32_t Value, uint32_t BitPosition)
{
	return (Value & (1 << BitPosition)) != 0;
}

uint64_t GCHandleDecryption = 0x0;
uint64_t CachedBaseNetworkable = 0x0;
uint64_t CachedVisiblePlayerList = 0x0;
uint64_t CachedLocalPlayer = 0x0;
const const char* GCHandleSig = "48 8D 05 ? ? ? ? 83 E2 07 8B F9 FF CA C1 EF 03 48 8D 0C 92 48 8D 34 C8";
const const char* LocalPlayerSig = "48 8B 05 ?? ?? ?? ?? 48 8B 80 B8 00 00 00 48 8B 15 ?? ?? ?? ?? 48 8B 48 30";
static void ScanGCHandle()
{
	// 48 8D 05 ?? ?? ?? ?? 83 E1 07 C1 EF 03
	// 48 8D 0D ?? ?? ?? ?? 45 33 C0 E8
	// 48 8D 05 ?? ?? ?? ?? 4A 8D 0C C8
	// //48 89 5C 24 08 57 48 83 EC 60 41 0F B6 D8 -> lea rax unk
	//GCHandleDecryption = mem.FindSignature(xorstr_("48 8D 05 ? ? ? ? 83 E2 07 8B F9 FF CA C1 EF 03 48 8D 0C 92 48 8D 34 C8"), mem.GetBaseAddress(xorstr_("GameAssembly.dll")), mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + mem.GetBaseSize(xorstr_("GameAssembly.dll")));
	//int relative = mem.Read<int>(GCHandleDecryption + 3);
	//printf("Offset: 0x%p\n", (ptr + 7 + relative) - base);
//	GCHandleDecryption = mem.Read<uintptr_t>(GCHandleDecryption + 7 + relative);
	//printf(xorstr_("[Encryption] GCHandleDecryption: 0x%llX\n"), GCHandleDecryption - mem.GetBaseAddress(xorstr_("GameAssembly.dll")));

	//GCHandleDecryption = GCHandleDecryption - mem.GetBaseAddress(xorstr_("GameAssembly.dll"));
}

// 48 89 6C 24 18 56 57 41 57 48 83 EC 20 8B
// C6 05 ?? ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 83 B9 E0 00 00 00 00 75 0C E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B 81 B8 00 00 00 48 83 78 38 00 75 14 Read + 3 + 7
/*
ptr = mem.FindSignature(xorstr_("C6 05 ? ? ? ? ? 48 8B 0D ? ? ? ? 83 B9 E0 00 00 00 00 75 0C E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 81 B8 00 00 00 48 83 78 38 00 75 14"), base, base + size);
		int relative = mem.Read<int>(ptr + 3);
uintptr_t addrptr = (ptr + 7 + relative) - 0x18
*/
/*
To find the function for encryption on local player:
Classnameofencryption = public sealed class %48368ea274e73962c2a6e3ce6481023676a0f549<T> : Pool.IPooled, IDisposable where T : class
Script.json Search: _object_ (BasePlayer__Classnameofencryption_T__o* 
Look for a function with only 1 other parameter, being a method info
Function should look like:

	public static T %ade45d0a7bdd3a0f7539ddbfee313c340e7e84c0<T>(BasePlayer<T>.%48368ea274e73962c2a6e3ce6481023676a0f549 %c0c582beefaf472f01fa11f714ff3f4ceb55ddb4) where T : class
	{
		return null;
	}
 v9 = __PAIR64__(__ROR4__(__ROR4__(HIDWORD(v9), 6) - 341843471, 17), __ROR4__(__ROR4__(v9, 6) - 341843471, 17));
*/

uintptr_t Il2cppGetHandle(int32_t ObjectHandleID)
{
	if (ObjectHandleID == 0)
		return 0;
	
	uint64_t rdi_1 = ((uint64_t)(ObjectHandleID >> 3));
	uint64_t rcx_1 = ((uint64_t)((ObjectHandleID & 7) - 1));

	if ((rdi_1 < mem.Read<uint32_t>((rcx_1 * 0x28) + (mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + GCHandleDecryption + 0x10)) && TEST_BITD(mem.Read<uint32_t>(mem.Read<uintptr_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + GCHandleDecryption + (rcx_1 * 0x28)) + ((((uint64_t)rdi_1) >> 5) << 2)), (rdi_1 & 0x1f))))
	{
		uintptr_t ObjectArray = mem.Read<uintptr_t>((rcx_1 * 0x28) + (mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + GCHandleDecryption + 0x8)) + (rdi_1 << 3);

		if (mem.Read<BYTE>((rcx_1 * 0x28) + (mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + GCHandleDecryption + 0x14)) > 1)
		{
		//	printf(xorstr_("[Encryption] ObjectArray: 0x%llX\n"), ObjectArray);
			return mem.Read<uintptr_t>(ObjectArray);
		}
		else
		{
			uint32_t eax = mem.Read<uint32_t>(ObjectArray);
			eax = ~eax;
		//	printf(xorstr_("[Encryption] ObjectArray2: 0x%llX\n"), eax);
			return eax;
		}
	}
//	printf(xorstr_("[Encryption] ObjectArray3:\n"));
	return 0;
}

uint64_t EncryptionSystem::DecryptBaseNetworkable(uint64_t address)
{


	if (address == 0) return 0;

	uint64_t* a2;
	uint64_t a3;
	uint32_t v5; // ecx
	int32_t v6; // eax
	bool v7; // zf
	uint64_t v24; // [rsp+20h] [rbp-28h] BYREF
	uint64_t v8; // rcx

	a2 = &v24;
	LODWORD(a3) = 2;
	v24 = mem.Read<uint64_t>(address + 0x18);

	do
	{
		v5 = *(uint32_t*)a2;
		v6 = *(uint32_t*)a2;
		a2 = (uint64_t*)((char*)a2 + 4);
		*((uint32_t*)a2 - 1) = ((((v6 << 20) | (v5 >> 12)) + 0x525B9596) << 14) | ((((v6 << 20) | (v5 >> 12)) + 0x525B9596) >> 18);
		v7 = (uint32_t)a3 == 1;
		a3 = (uint32_t)(a3 - 1);
	} while (!v7);

	v8 = v24;

	return Il2cppGetHandle(v8);
}
//48 8B 05 ?? ?? ?? ?? 48 8B 80 B8 00 00 00 48 8B 15 ?? ?? ?? ?? 48 8B 48 30

uint64_t EncryptionSystem::DecryptLocalPlayer(uint64_t address)
{

	address = mem.Read<uint64_t>(address + 0x18);
	uint64_t output = 0;
	uint32_t lo = __ROR4__(__ROR4__(static_cast<uint32_t>(address), 0x10) - 0x39AE528E, 0x10);
	uint32_t hi = __ROR4__(__ROR4__(static_cast<uint32_t>(address >> 32), 0x10) - 0x39AE528E, 0x10);
	output = __PAIR64__(hi, lo);
	uint64_t ret = Il2cppGetHandle(output);
	return ret;
}
__forceinline uint64_t decrypt_uint32(uint64_t encrypted_value) {
	uint32_t* parts = reinterpret_cast<uint32_t*>(&encrypted_value);

	for (int i = 0; i < 2; ++i) {
		uint32_t value = parts[i];
		value = std::rotl(value + 932770716, 24) ^ 0x3E1D0E5D;
		parts[i] = value + 1842716310;
	}

	return encrypted_value;
}
uint64_t EncryptionSystem::DecryptVisiblePlayer(uint64_t address)
{



	return Il2cppGetHandle(decrypt_uint32(mem.Read<uintptr_t>(address + 0x18)));
}
uint64_t EncryptionSystem::DecryptPlayerInventory(uint64_t address, uint64_t classaddress)
{

	if (PlayerInventoryMap.find(classaddress) != PlayerInventoryMap.end())
	{
		return PlayerInventoryMap[classaddress];
	}
	address = mem.Read<uint64_t>(address);
	uint64_t val = DecryptVisiblePlayer(address);
	PlayerInventoryMap[classaddress] = val;
	return val;
}
uint64_t EncryptionSystem::DecryptPlayerEyes(uint64_t address, uint64_t classaddress)
{
	if (PlayerEyesMap.find(classaddress) != PlayerEyesMap.end())
	{
	//	printf(xorstr_("[Encryption] Found Player Eyes\n"));
	//	printf(xorstr_("[Encryption] Player Eyes: 0x%llX\n"), PlayerEyesMap[classaddress]);
		return PlayerEyesMap[classaddress];
	}
	address = mem.Read<uint64_t>(address);
	uint64_t val = DecryptVisiblePlayer(address);
	PlayerEyesMap[classaddress] = val;
	return val;
}
EncryptionSystem::EncryptionSystem()
{
	if (!GCHandleDecryption)
	{
		ScanGCHandle();
		GCHandleDecryption = 0xA0A4680;
		uint64_t networkable = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + BaseNetworkableClass); // Get Class Start Address
		//printf("[BaseNetworkable] BaseNetworkable: 0x%llX\n", networkable);
		uint64_t staticfield = mem.Read<uint64_t>(networkable + 0xb8);
//		printf("[BaseNetworkable] Static Fields: 0x%llX\n", staticfield);
		uint64_t listparent = mem.Read<uint64_t>(staticfield + 0x20); // the class containing the list
//		printf("[BaseNetworkable] ListParent: 0x%llX\n", listparent);
		uint64_t decryptedlistparent = DecryptBaseNetworkable(listparent);
	//	printf("[BaseNetworkable] DecryptedListParent: 0x%llX\n", decryptedlistparent);
		uint64_t entitylist = DecryptBaseNetworkable(mem.Read<uint64_t>(decryptedlistparent + 0x10));
		printf("[BaseNetworkable] EntityList: 0x%llX\n", entitylist);
		CachedBaseNetworkable = entitylist;

		uint64_t baseplayer = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + BasePlayerClass); // Get Class Start Address	
	//	printf("[BasePlayer] BasePlayer: 0x%llX\n", baseplayer);
		uint64_t playerstaticfield = mem.Read<uint64_t>(baseplayer + 0xb8);
		printf("[BasePlayer] Static Fields: 0x%llX\n", playerstaticfield);
		uint64_t visibleplayerlist = mem.Read<uint64_t>(playerstaticfield + PlayerList);
		CachedVisiblePlayerList = DecryptVisiblePlayer(visibleplayerlist);	
		printf("[BasePlayer] VisiblePlayerList: 0x%llX\n", CachedVisiblePlayerList);

	



	}
	/*uint64_t localplayerbase = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + LocalPlayerClass); // Get Class Start Address
	//printf("[BasePlayer] LocalPlayerBase: 0x%llX\n", localplayerbase);
	uint64_t localplayerstaticfield = mem.Read<uint64_t>(localplayerbase + 0xb8);
	//printf("[BasePlayer] LocalPlayerStaticFields: 0x%llX\n", localplayerstaticfield);
	uint64_t localplayerencrypted = mem.Read<uint64_t>(localplayerstaticfield + 0x40);
	//printf("[BasePlayer] localplayerencrypted: 0x%llX\n", localplayerencrypted);
	uint64_t localplayer = DecryptLocalPlayer(localplayerencrypted);
	printf("[BasePlayer] LocalPlayer: 0x%llX\n", localplayer);
	LocalPlayer = localplayer;*/
	if (CachedBaseNetworkable != 0x0)
	{
	BaseNetworkableList = CachedBaseNetworkable;
	}
	if (CachedVisiblePlayerList != 0x0)
	{
		VisiblePlayerList = CachedVisiblePlayerList;
	}

	//printf(xorstr_("[Encryption] GCHandleDecryption: 0x%llX\n"), GCHandleDecryption);

	//	disassemble_and_copy_function(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + 0x302856E0);
}

void EncryptionSystem::Update()
{
}

uint64_t EncryptionSystem::GetVisiblePlayerList()
{
	return VisiblePlayerList;
}

uint64_t EncryptionSystem::GetBaseNetworkableList()
{
	return BaseNetworkableList;
}

uint64_t EncryptionSystem::GetPlayerEyes(uint64_t player)
{
	if (PlayerEyesMap.find(player) != PlayerEyesMap.end())
	{
		return PlayerEyesMap[player];
	}
	return 0;
}

uint64_t EncryptionSystem::GetPlayerInventory(uint64_t player)
{
	if (PlayerInventoryMap.find(player) != PlayerInventoryMap.end())
	{
		return PlayerInventoryMap[player];
	}
	return 0;
}

uint64_t EncryptionSystem::GetCapsuleCollider(uint64_t player)
{
	if (CapsuleColliderMap.find(player) != CapsuleColliderMap.end())
	{
		return CapsuleColliderMap[player];
	}
	return 0;
}

uint64_t EncryptionSystem::GetLocalPlayer()
{
	return LocalPlayer;
}