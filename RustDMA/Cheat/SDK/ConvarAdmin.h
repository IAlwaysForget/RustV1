#pragma once
class ConvarAdmin //Convar.Admin
{
	/*
	Script.json
	   "Address": 54580312,
      "Name": "ConVar.Admin_TypeInfo",
      "Signature": "ConVar_Admin_c*"
	  Search for this
	  public struct ServerUGCInfo
	{
		// Token: 0x0400684D RID: 26701
		[Token(Token = "0x400684D")]
		[FieldOffset(Offset = "0x0")]
		public NetworkableId entityId;

		// Token: 0x0400684E RID: 26702
		[Token(Token = "0x400684E")]
		[FieldOffset(Offset = "0x8")]
		public uint[] crcs;

		// Token: 0x0400684F RID: 26703
		[Token(Token = "0x400684F")]
		[FieldOffset(Offset = "0x10")]
		public UGCType contentType;

		// Token: 0x04006850 RID: 26704
		[Token(Token = "0x4006850")]
		[FieldOffset(Offset = "0x14")]
		public uint entityPrefabID;

		// Token: 0x04006851 RID: 26705
		[Token(Token = "0x4006851")]
		[FieldOffset(Offset = "0x18")]
		public string shortPrefabName;

		// Token: 0x04006852 RID: 26706
		[Token(Token = "0x4006852")]
		[FieldOffset(Offset = "0x20")]
		public ulong[] playerIds;
	}

	*/
	uint64_t Class = 63386328;
	//Dump.cs / DummyDLL
	uint64_t StaticField = 0xB8;// Static Padding To Access Static Fields
	uint64_t AdminTime = 0x20;// public static float admintime;
	uint64_t AdminUnderWaterEffect = 0x8;// public static bool underwater_effect;
public:
	ConvarAdmin();
	void SetAdminTime(float value);
	void ClearVisionInWater(bool value);
};