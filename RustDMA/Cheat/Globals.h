#pragma once
#include "Memory.h"
#include <mutex>
#include "BaseNetworkable.h"
#include "BasePlayer.h"
#include "MainCamera.h"
#include "TODSky.h"
#include "FacepunchInput.h"
#include "ConsoleSystem.h"
#include "World.h"
#include "sdk/Client.h"
#include "sdk/PlayerConvar.h"
#include "SDK/Encryption.h"
#define DO_AUTH
extern std::atomic<std::shared_ptr<BasePlayer>> BaseLocalPlayer;
extern std::atomic<std::shared_ptr<BaseNetworkable>> Networkable;
extern std::atomic<std::shared_ptr<MainCamera>> Camera;
extern std::atomic<std::shared_ptr<EncryptionSystem>> Encryption;
extern std::mutex PlayerCacheMutex;
extern std::mutex ConfigMutex;
extern std::atomic<std::shared_ptr<TODSky>> TodSky;
extern std::atomic<std::shared_ptr<FacepunchInput>> Input;
extern std::atomic<std::shared_ptr<ConsoleSystem>> Console;
extern std::atomic<std::shared_ptr<World>> MapWorld;
extern std::atomic<std::shared_ptr<Client>> ClientInstance;
extern std::atomic<std::shared_ptr<PlayerConvar>> PlayerConvarInstance;
extern VMMDLL_SCATTER_HANDLE MainReadScatterHandle;
extern VMMDLL_SCATTER_HANDLE MainWriteScatterHandle;
extern std::atomic<std::shared_ptr<BasePlayer>> AimbotTarget;
//extern std::atomic<std::chrono::steady_clock::time_point> LastHeartbeatCheck;
//extern std::atomic<int> vorpal_fail_count;
extern int MainScatterReadCount;
extern int MainScatterWriteCount;
extern float DeltaTime;
extern HWND Hwnd;
extern void Initialize();
extern void StartCheat();
extern bool Initialized;
extern std::mutex InitializeMutex;
