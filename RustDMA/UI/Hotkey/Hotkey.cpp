#include "Hotkey.h"

#include <string>
#include <vector>

namespace Hotkey
{
	int lastPressedKey = 0;
	KeyState keyMap[254]; //since there are only 254 different VirtualKeys
	bool IsDown(int vKey)
	{
		if (keyMap[int(vKey)] == KeyState::Down)
		{
			return true;
		}
		return false;
	}

	bool IsPressed(int vKey)
	{
		if (keyMap[int(vKey)] == KeyState::Pressed)
		{
			keyMap[int(vKey)] = KeyState::Up;
			return true;
		}
		return false;
	}

	bool keyboardEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WPARAM key = wParam;
		KeyState state = KeyState::None;

		switch (uMsg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			state = KeyState::Down;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			state = KeyState::Up;
			break;
		default:
			return false;
		}

		if (state == KeyState::Up)
			lastPressedKey = int(key);
		if (keyMap[int(key)] == KeyState::Pressed) keyMap[int(key)] = KeyState::Up;

		if (state == KeyState::Up && keyMap[int(key)] == KeyState::Down)
			keyMap[int(key)] = KeyState::Pressed;
		else
			keyMap[int(key)] = state;
		return true;
	}
}
