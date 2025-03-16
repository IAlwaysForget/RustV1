#pragma once
#include <DMALibrary/libs/vmmdll.h>
#include "misc/CheatFunction.h"
class EncryptionSystem
{
	/*
	      "Address": 63349520,
      "Name": "%09955fe8067518e570836b986a96cd8ffbfcbf17_TypeInfo",
      "Signature": "_09955fe8067518e570836b986a96cd8ffbfcbf17_c*"
	  Go to basenetworkable, Find the encrypted pointer:
	  [Token(Token = "0x4002A69")]
	[FieldOffset(Offset = "0x0")]
	public static %fb5a18ea36a252080a5d8504bae6754802c91cfe<global::BaseNetworkable.%e9135f689a353246b9465cc26fe83c88dabc5a7a> %f9d0782622c2d0abbb173e3f589fb0f6f8c51e86;
	Then go to the class and check the inherriter.
	public sealed class %fb5a18ea36a252080a5d8504bae6754802c91cfe<T> : %09955fe8067518e570836b986a96cd8ffbfcbf17, Pool.IPooled, IDisposable where T : class
	That class after : is the encryption class. Get the pointer to it in dump

	*/
	uint64_t Class = 63367544;
	uint64_t StaticField = 0xB8; // Static Padding To Access Static Fields
	uint64_t ClassInfoDict = 0x0;  //private static readonly Dictionary<ulong, object> %2fb3689e5731cddcc9c3b85ac2ce2493db77e630;
	uint64_t VisiblePlayerList = 0x0;
	uint64_t BaseNetworkableList = 0x0;
	uint64_t LocalPlayer = 0x0;
	std::unordered_map<uint64_t, uint64_t> PlayerEyesMap;
	std::unordered_map<uint64_t, uint64_t> PlayerInventoryMap;
	std::unordered_map<uint64_t, uint64_t> CapsuleColliderMap;
	/*
"Address": 89544112,
	 "Name": "BaseNetworkable_TypeInfo",
	 "Signature": "BaseNetworkable_c*"
   */
	uint64_t BaseNetworkableClass = 165917080;
	
	/*
	  "Address": 54626928,
  "Name": "BasePlayer_TypeInfo",
  "Signature": "BasePlayer_c*"
*/
	uint64_t BasePlayerClass = 166249112;
	uint64_t PlayerList = 0x368; //private static global::BasePlayer<ListDictionary<ulong, global::BasePlayer>>. % 48368ea274e73962c2a6e3ce6481023676a0f549 % 7588f86ed54fc2f3294b913f86edea2f4024fe53;

	/*
	Localplayer class can be found by searching IPlayerItem then checking usedby 
	*/
	uint64_t LocalPlayerClass = 100059776;
public:
	EncryptionSystem();
	void Update();
	uint64_t GetBaseNetworkableList();
	uint64_t GetVisiblePlayerList();
	uint64_t GetPlayerEyes(uint64_t player);
	uint64_t GetPlayerInventory(uint64_t player);
	uint64_t GetCapsuleCollider(uint64_t player);
	uint64_t DecryptBaseNetworkable(uint64_t address);
	uint64_t DecryptVisiblePlayer(uint64_t address);
	uint64_t DecryptLocalPlayer(uint64_t address);
	uint64_t DecryptPlayerInventory(uint64_t address,uint64_t classaddress);
	uint64_t DecryptPlayerEyes(uint64_t address, uint64_t classaddress);
	uint64_t GetLocalPlayer();
	
};
