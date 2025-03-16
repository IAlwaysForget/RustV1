#include "Pch.h"
#include "FacepunchInput.h"
#include "Memory.h"
#include "xorstr.hpp"
#include "DMALibrary/Memory/Memory.h"

FacepunchInput::FacepunchInput()
{
	/*
	So we get all the key pointers here.. 
	Then we can create functions for is key down or not
	We can then execute console commands on keys by copying a key with a known command and replacing the command string
	So we can debug camera, set player flag to ground, set admin flag, do all this when the bind key is clicked and instantly undo admin flag
	*/
	VirtualKeyTranslator();
	//	printf("[FacepunchInput] Initialized\n");
	uint64_t input = mem.Read<uint64_t>(mem.GetBaseAddress(xorstr_("GameAssembly.dll")) + Class);
	//	printf("[FacepunchInput] FacepunchInput: 0x%llX\n", input);
	this->StaticField = mem.Read<uint64_t>(input + StaticField);
	//printf("[FacepunchInput] Static Fields: 0x%llX\n", StaticField);
	this->Buttons = mem.Read<uint64_t>(StaticField + Buttons);
	//printf("[FacepunchInput] Buttons: 0x%llX\n", Buttons);
	this->ButtonBuffer = mem.Read<uint64_t>(Buttons + ButtonBuffer);
	//	printf("[FacepunchInput] ButtonBuffer: 0x%llX\n", ButtonBuffer);
	this->ButtonSize = mem.Read<uint64_t>(Buttons + ButtonSize);
	//	printf("[FacepunchInput] ButtonSize: 0x%llX\n", ButtonSize);

	for (int i = 0; i < ButtonSize; i++)
	{
		uint64_t button = mem.Read<uint64_t>(ButtonBuffer + 0x20 + (i * 0x8));
		uint64_t name = mem.Read<uint64_t>(button + ButtonName);
		wchar_t buttonname[36] = {'\0'};
		mem.Read(name + 0x14, reinterpret_cast<void*>(&buttonname), sizeof(buttonname));
		//	printf("[FacepunchInput] Button: %ls\n", buttonname);
		//	printf("[FacepunchInput] Button: 0x%llX\n", button);
		uint64_t bind = mem.Read<uint64_t>(button + Binds);
		//	printf("[FacepunchInput] Bind: 0x%llX\n", bind);
		KeyCode keycode = mem.Read<KeyCode>(button + UnityKeyCode);
		//  printf("[FacepunchInput] KeyCode: %d\n", keycode);
		KeyMap[keycode] = button;

		if (bind != 0 && (keycode == KeyCode::Keypad1 || keycode == KeyCode::Keypad2 || keycode == KeyCode::Keypad3 || keycode == KeyCode::Keypad4 || keycode == KeyCode::Keypad5 || keycode == KeyCode::Keypad6))
		{
			uint32_t bindssize = mem.Read<uint32_t>(bind + 0x18);
			if (bindssize == 1)
				CachedBind = bind;
			for (int j = 0; j < bindssize; j++)
			{
				uint64_t bindname = mem.Read<uint64_t>(bind + 0x20 + (j * 0x8));
				wchar_t bindnamestr[36] = {'\0'};
				mem.Read(bindname + 0x14, reinterpret_cast<void*>(&bindnamestr), sizeof(bindnamestr));

				//printf("[FacepunchInput] Bind: %ls\n", bindnamestr);
			}
		}
		// 0x20 == true and 0x21 == true then key is held
		// 0x20 and not 0x21 key is pressed
	}
}

bool FacepunchInput::IsKeyDown(int key)
{
	uint64_t keyaddress = KeyMap[virtualToUnityKey[key]];
	if (keyaddress == 0)
		return false;
	bool down = mem.Read<bool>(keyaddress + KeyDown);
	return down;
}

bool FacepunchInput::IsKeyClicked(int key)
{
	uint64_t keyaddress = KeyMap[virtualToUnityKey[key]];
	if (keyaddress == 0)
		return false;

	if (KeyTimes[virtualToUnityKey[key]] > (clock() * 0.00001f))
		return false;
	bool down = mem.Read<bool>(keyaddress + KeyDown);
	if (down)
		KeyTimes[virtualToUnityKey[key]] = (clock() * 0.00001f) + 0.002f;
	return down;
}

void FacepunchInput::InvokeKey(int key)
{
	uint64_t keyaddress = KeyMap[virtualToUnityKey[key]];
	mem.Write<bool>(keyaddress + KeyDown, true);
	mem.Write<bool>(keyaddress + LastValue, false);
}

void FacepunchInput::SetKeyCommand(int key, const std::wstring& command)
{
	/*  uint64_t keyaddress = KeyMap[virtualToUnityKey[key]];
	  printf("[FacepunchInput] KeyAddress: 0x%llX\n", keyaddress);
  
	  
	  mem.Write<uint64_t>(keyaddress + Binds, CachedBind);
	  uint64_t realbind = mem.Read<uint64_t>(keyaddress + Binds);
	  printf("[FacepunchInput] RealBind: 0x%llX\n", realbind);
	  uint64_t nameaddress = mem.Read<uint64_t>(realbind + 0x20);
	  printf("[FacepunchInput] NameAddress: 0x%llX\n", nameaddress);
	  
	  auto commandinstance = command.c_str();
	  mem.Write<uint32_t>(nameaddress + 0x10, command.size());// write to first instance
	  mem.Write(nameaddress +0x14,reinterpret_cast<void*>(&commandinstance),sizeof(commandinstance));// write to first instance*/
}

void FacepunchInput::VirtualKeyTranslator()
{
	virtualToUnityKey[(int)VirtualKeyCodes::LBUTTON] = KeyCode::Mouse0;
	virtualToUnityKey[(int)VirtualKeyCodes::RBUTTON] = KeyCode::Mouse1;
	virtualToUnityKey[(int)VirtualKeyCodes::CANCEL] = KeyCode::Break;
	virtualToUnityKey[(int)VirtualKeyCodes::MBUTTON] = KeyCode::Mouse2;
	virtualToUnityKey[(int)VirtualKeyCodes::VKXBUTTON1] = KeyCode::Mouse3;
	virtualToUnityKey[(int)VirtualKeyCodes::VKXBUTTON2] = KeyCode::Mouse4;
	virtualToUnityKey[(int)VirtualKeyCodes::BACK] = KeyCode::Backspace;
	virtualToUnityKey[(int)VirtualKeyCodes::TAB] = KeyCode::Tab;
	virtualToUnityKey[(int)VirtualKeyCodes::CLEAR] = KeyCode::Clear;
	virtualToUnityKey[(int)VirtualKeyCodes::RETURN] = KeyCode::Return;
	virtualToUnityKey[(int)VirtualKeyCodes::SHIFT] = KeyCode::LeftShift;
	virtualToUnityKey[(int)VirtualKeyCodes::CONTROL] = KeyCode::LeftControl;
	virtualToUnityKey[(int)VirtualKeyCodes::MENU] = KeyCode::LeftAlt;
	virtualToUnityKey[(int)VirtualKeyCodes::PAUSE] = KeyCode::Pause;
	virtualToUnityKey[(int)VirtualKeyCodes::CAPITAL] = KeyCode::CapsLock;
	virtualToUnityKey[(int)VirtualKeyCodes::KANA] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::HANGUEL] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::HANGUL] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::JUNJA] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::FINAL] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::HANJA] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::KANJI] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::ESCAPE] = KeyCode::Escape;
	virtualToUnityKey[(int)VirtualKeyCodes::CONVERT] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::NONCONVERT] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::ACCEPT] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::MODECHANGE] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::SPACE] = KeyCode::Space;
	virtualToUnityKey[(int)VirtualKeyCodes::PRIOR] = KeyCode::PageUp;
	virtualToUnityKey[(int)VirtualKeyCodes::NEXT] = KeyCode::PageDown;
	virtualToUnityKey[(int)VirtualKeyCodes::END] = KeyCode::End;
	virtualToUnityKey[(int)VirtualKeyCodes::HOME] = KeyCode::Home;
	virtualToUnityKey[(int)VirtualKeyCodes::LEFT] = KeyCode::LeftArrow;
	virtualToUnityKey[(int)VirtualKeyCodes::UP] = KeyCode::UpArrow;
	virtualToUnityKey[(int)VirtualKeyCodes::RIGHT] = KeyCode::RightArrow;
	virtualToUnityKey[(int)VirtualKeyCodes::DOWN] = KeyCode::DownArrow;
	virtualToUnityKey[(int)VirtualKeyCodes::SELECT] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::PRINT] = KeyCode::Print;
	virtualToUnityKey[(int)VirtualKeyCodes::EXECUTE] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::SNAPSHOT] = KeyCode::SysReq;
	virtualToUnityKey[(int)VirtualKeyCodes::INSERT] = KeyCode::Insert;
	virtualToUnityKey[(int)VirtualKeyCodes::KeyDELETE] = KeyCode::Delete;
	virtualToUnityKey[(int)VirtualKeyCodes::HELP] = KeyCode::Help;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_0] = KeyCode::Alpha0;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_1] = KeyCode::Alpha1;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_2] = KeyCode::Alpha2;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_3] = KeyCode::Alpha3;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_4] = KeyCode::Alpha4;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_5] = KeyCode::Alpha5;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_6] = KeyCode::Alpha6;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_7] = KeyCode::Alpha7;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_8] = KeyCode::Alpha8;
	virtualToUnityKey[(int)VirtualKeyCodes::VK_9] = KeyCode::Alpha9;
	virtualToUnityKey[(int)VirtualKeyCodes::A] = KeyCode::A;
	virtualToUnityKey[(int)VirtualKeyCodes::B] = KeyCode::B;
	virtualToUnityKey[(int)VirtualKeyCodes::C] = KeyCode::C;
	virtualToUnityKey[(int)VirtualKeyCodes::D] = KeyCode::D;
	virtualToUnityKey[(int)VirtualKeyCodes::E] = KeyCode::E;
	virtualToUnityKey[(int)VirtualKeyCodes::F] = KeyCode::F;
	virtualToUnityKey[(int)VirtualKeyCodes::G] = KeyCode::G;
	virtualToUnityKey[(int)VirtualKeyCodes::H] = KeyCode::H;
	virtualToUnityKey[(int)VirtualKeyCodes::I] = KeyCode::I;
	virtualToUnityKey[(int)VirtualKeyCodes::J] = KeyCode::J;
	virtualToUnityKey[(int)VirtualKeyCodes::K] = KeyCode::K;
	virtualToUnityKey[(int)VirtualKeyCodes::L] = KeyCode::L;
	virtualToUnityKey[(int)VirtualKeyCodes::M] = KeyCode::M;
	virtualToUnityKey[(int)VirtualKeyCodes::N] = KeyCode::N;
	virtualToUnityKey[(int)VirtualKeyCodes::O] = KeyCode::O;
	virtualToUnityKey[(int)VirtualKeyCodes::P] = KeyCode::P;
	virtualToUnityKey[(int)VirtualKeyCodes::Q] = KeyCode::Q;
	virtualToUnityKey[(int)VirtualKeyCodes::R] = KeyCode::R;
	virtualToUnityKey[(int)VirtualKeyCodes::S] = KeyCode::S;
	virtualToUnityKey[(int)VirtualKeyCodes::T] = KeyCode::T;
	virtualToUnityKey[(int)VirtualKeyCodes::U] = KeyCode::U;
	virtualToUnityKey[(int)VirtualKeyCodes::V] = KeyCode::V;
	virtualToUnityKey[(int)VirtualKeyCodes::W] = KeyCode::W;
	virtualToUnityKey[(int)VirtualKeyCodes::X] = KeyCode::X;
	virtualToUnityKey[(int)VirtualKeyCodes::Y] = KeyCode::Y;
	virtualToUnityKey[(int)VirtualKeyCodes::Z] = KeyCode::Z;
	virtualToUnityKey[(int)VirtualKeyCodes::LWIN] = KeyCode::LeftWindows;
	virtualToUnityKey[(int)VirtualKeyCodes::RWIN] = KeyCode::RightWindows;
	virtualToUnityKey[(int)VirtualKeyCodes::APPS] = KeyCode::Menu;
	virtualToUnityKey[(int)VirtualKeyCodes::SLEEP] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD0] = KeyCode::Keypad0;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD1] = KeyCode::Keypad1;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD2] = KeyCode::Keypad2;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD3] = KeyCode::Keypad3;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD4] = KeyCode::Keypad4;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD5] = KeyCode::Keypad5;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD6] = KeyCode::Keypad6;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD7] = KeyCode::Keypad7;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD8] = KeyCode::Keypad8;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMPAD9] = KeyCode::Keypad9;
	virtualToUnityKey[(int)VirtualKeyCodes::MULTIPLY] = KeyCode::KeypadMultiply;
	virtualToUnityKey[(int)VirtualKeyCodes::ADD] = KeyCode::KeypadPlus;
	virtualToUnityKey[(int)VirtualKeyCodes::SEPARATOR] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::SUBTRACT] = KeyCode::KeypadMinus;
	virtualToUnityKey[(int)VirtualKeyCodes::DECIMAL] = KeyCode::KeypadPeriod;
	virtualToUnityKey[(int)VirtualKeyCodes::DIVIDE] = KeyCode::KeypadDivide;
	virtualToUnityKey[(int)VirtualKeyCodes::F1] = KeyCode::F1;
	virtualToUnityKey[(int)VirtualKeyCodes::F2] = KeyCode::F2;
	virtualToUnityKey[(int)VirtualKeyCodes::F3] = KeyCode::F3;
	virtualToUnityKey[(int)VirtualKeyCodes::F4] = KeyCode::F4;
	virtualToUnityKey[(int)VirtualKeyCodes::F5] = KeyCode::F5;
	virtualToUnityKey[(int)VirtualKeyCodes::F6] = KeyCode::F6;
	virtualToUnityKey[(int)VirtualKeyCodes::F7] = KeyCode::F7;
	virtualToUnityKey[(int)VirtualKeyCodes::F8] = KeyCode::F8;
	virtualToUnityKey[(int)VirtualKeyCodes::F9] = KeyCode::F9;
	virtualToUnityKey[(int)VirtualKeyCodes::F10] = KeyCode::F10;
	virtualToUnityKey[(int)VirtualKeyCodes::F11] = KeyCode::F11;
	virtualToUnityKey[(int)VirtualKeyCodes::F12] = KeyCode::F12;
	virtualToUnityKey[(int)VirtualKeyCodes::F13] = KeyCode::F13;
	virtualToUnityKey[(int)VirtualKeyCodes::F14] = KeyCode::F14;
	virtualToUnityKey[(int)VirtualKeyCodes::F15] = KeyCode::F15;
	virtualToUnityKey[(int)VirtualKeyCodes::NUMLOCK] = KeyCode::Numlock;
	virtualToUnityKey[(int)VirtualKeyCodes::SCROLL] = KeyCode::ScrollLock;
	virtualToUnityKey[(int)VirtualKeyCodes::LSHIFT] = KeyCode::LeftShift;
	virtualToUnityKey[(int)VirtualKeyCodes::RSHIFT] = KeyCode::RightShift;
	virtualToUnityKey[(int)VirtualKeyCodes::LCONTROL] = KeyCode::LeftControl;
	virtualToUnityKey[(int)VirtualKeyCodes::RCONTROL] = KeyCode::RightControl;
	virtualToUnityKey[(int)VirtualKeyCodes::LMENU] = KeyCode::LeftAlt;
	virtualToUnityKey[(int)VirtualKeyCodes::RMENU] = KeyCode::RightAlt;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_BACK] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_FORWARD] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_REFRESH] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_STOP] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_SEARCH] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_FAVORITES] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::BROWSER_HOME] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::VOLUME_MUTE] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::VOLUME_DOWN] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::VOLUME_UP] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::MEDIA_NEXT_TRACK] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::MEDIA_PREV_TRACK] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::MEDIA_STOP] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::MEDIA_PLAY_PAUSE] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::LAUNCH_MAIL] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::LAUNCH_MEDIA_SELECT] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::LAUNCH_APP1] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::LAUNCH_APP2] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_1] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_PLUS] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_COMMA] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_MINUS] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_PERIOD] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_2] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_3] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_4] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_5] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_6] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_7] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_8] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_102] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::PROCESSKEY] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::PACKET] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::ATTN] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::CRSEL] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::EXSEL] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::EREOF] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::PLAY] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::ZOOM] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::NONAME] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::PA1] = KeyCode::None;
	virtualToUnityKey[(int)VirtualKeyCodes::OEM_CLEAR] = KeyCode::None;
}
