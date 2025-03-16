#pragma once
#include <DMALibrary/Memory/Memory.h>
class Client
{
	/*
	  "Address": 60760128,
	  "Name": "ConVar.Client_TypeInfo",
	  "Signature": "ConVar_Client_c*"

	  Search for:
	  [Token(Token = "0x2001189")]
	public enum CameraSensorType
	{
		// Token: 0x040068D3 RID: 26835
		[Token(Token = "0x40068D3")]
		_8Mm,
		// Token: 0x040068D4 RID: 26836
		[Token(Token = "0x40068D4")]
		Super8Mm,
		// Token: 0x040068D5 RID: 26837
		[Token(Token = "0x40068D5")]
		_16Mm,
		// Token: 0x040068D6 RID: 26838
		[Token(Token = "0x40068D6")]
		Super16mm,
		// Token: 0x040068D7 RID: 26839
		[Token(Token = "0x40068D7")]
		_32mm2Perf,
		// Token: 0x040068D8 RID: 26840
		[Token(Token = "0x40068D8")]
		_35mmAcademy,
		// Token: 0x040068D9 RID: 26841
		[Token(Token = "0x40068D9")]
		Super35,
		// Token: 0x040068DA RID: 26842
		[Token(Token = "0x40068DA")]
		_65mmALEXA,
		// Token: 0x040068DB RID: 26843
		[Token(Token = "0x40068DB")]
		_70mm,
		// Token: 0x040068DC RID: 26844
		[Token(Token = "0x40068DC")]
		_70mmImax,
		// Token: 0x040068DD RID: 26845
		[Token(Token = "0x40068DD")]
		ALEXA_LF,
		// Token: 0x040068DE RID: 26846
		[Token(Token = "0x40068DE")]
		ALEXA_35,
		// Token: 0x040068DF RID: 26847
		[Token(Token = "0x40068DF")]
		Custom
	*/
	uint64_t Class = 89319232;
	uint64_t StaticField = 0xB8; // Static Padding To Access Static Fields
	uint64_t CamSpeed = 0x78; // private static float _camspeed;
	uint64_t CamZoomSpeed = 0x28; // private static float camzoomspeed;
	uint64_t CamLookSpeed = 0xd8; // private static float camlookspeed;
	uint64_t CamFov = 0x80; // private static float camfov;
	uint64_t ClientTickInterval = 0xc; //	public static float clientTickInterval;

public:
	Client();
	void WriteCamSpeed(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteCamZoomSpeed(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteCamLookSpeed(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteCamFov(VMMDLL_SCATTER_HANDLE handle, float value);
	void WriteClientTickInterval(VMMDLL_SCATTER_HANDLE handle, float value);
};