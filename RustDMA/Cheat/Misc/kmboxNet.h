#pragma once
#include <thread>
#include <Winsock2.h>
#include "Config.h"
#include "HidTable.h"
#include <stdio.h>


#pragma warning(disable : 4996)



class KmBoxMouse
{
public:
	soft_mouse_t MouseData{ };

public:
	int Move(int x, int y);
	int Move_Auto(int x, int y, int Runtime);
	int Left(bool Down);
	int Right(bool Down);
	int Middle(bool Down);
};

class KmBoxKeyBoard
{
public:
	std::thread t_Listen;
	WORD MonitorPort;
	SOCKET s_ListenSocket = 0;
	bool ListenerRuned = false;

public:
	standard_keyboard_report_t hw_Keyboard;
	standard_mouse_report_t hw_Mouse;

public:
	~KmBoxKeyBoard();
	void ListenThread();
	int StartMonitor(WORD Port);
	void EndMonitor();

public:
	bool GetKeyState(WORD vKey);
};

class KmBoxNetManager
{
private:
	SOCKADDR_IN AddrServer;
	SOCKET s_Client = 0;
	client_data ReceiveData;
	client_data PostData;

private:
	int NetHandler();
	int SendData(int DataLength);

public:
	~KmBoxNetManager();

	int InitDevice(const std::string& IP, WORD Port, const std::string& Mac);
	int RebootDevice();
	int SetConfig(const std::string& IP, WORD Port);
	int DrawLogo();

public:
	friend class KmBoxMouse;
	KmBoxMouse Mouse;
	friend class KmBoxKeyBoard;
	KmBoxKeyBoard KeyBoard;
};

extern KmBoxNetManager KmBoxMgr;
