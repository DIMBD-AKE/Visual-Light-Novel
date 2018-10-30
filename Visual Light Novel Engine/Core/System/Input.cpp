#include "../../stdafx.h"
#include "Input.h"

Input::Input()
{
}


Input::~Input()
{
}

void Input::Update()
{
	memcpy(oldKeyState, keyState, sizeof(oldKeyState));
	
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));

	GetKeyboardState(keyState);

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
	{
		byte key = keyState[i] & 0x80;
		keyState[i] = key ? 1 : 0;

		if (oldKeyState[i] == 0 && keyState[i] == 1)
			keyMap[i] = static_cast<byte>(KeyState::KEY_STATUS_DOWN);
		else if (oldKeyState[i] == 1 && keyState[i] == 0)
			keyMap[i] = static_cast<byte>(KeyState::KEY_STATUS_UP);
		else if (oldKeyState[i] == 1 && keyState[i] == 1)
			keyMap[i] = static_cast<byte>(KeyState::KEY_STATUS_PRESS);
		else
			keyMap[i] = static_cast<byte>(KeyState::KEY_STATUS_NONE);
	}
}

POINT Input::GetMousePos(string window)
{
	POINT pos;
	GetCursorPos(&pos);
	RECT rect;
	GetWindowRect(SETTING->GetWindow(window)->WindowHandle, &rect);
	pos.x -= rect.left + 8;
	pos.y -= rect.top + 30;
	return pos;
}

void Input::SetMousePos(string window, float x, float y)
{
	RECT rect;
	GetWindowRect(SETTING->GetWindow(window)->WindowHandle, &rect);
	rect.left += 8;
	rect.top += 30;
	SetCursorPos(x + rect.left, y + rect.top);
}
