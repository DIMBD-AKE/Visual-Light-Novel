#pragma once

#define MAX_INPUT_KEY 255
#define MAX_INPUT_MOUSE 8

class Input
{
	SINGLETON(Input)

public:
	void Update();
	
private:
	enum class KeyState
	{
		KEY_STATUS_NONE,
		KEY_STATUS_DOWN,
		KEY_STATUS_UP,
		KEY_STATUS_PRESS
	};

	byte keyState[MAX_INPUT_KEY];
	byte oldKeyState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];

public:
	bool GetKeyDown(DWORD key) { return keyMap[key] == static_cast<byte>(KeyState::KEY_STATUS_DOWN); }
	bool GetKeyUp(DWORD key) { return keyMap[key] == static_cast<byte>(KeyState::KEY_STATUS_UP); }
	bool GetKey(DWORD key) { return keyMap[key] == static_cast<byte>(KeyState::KEY_STATUS_PRESS); }
	POINT GetMousePos(string window);
	void SetMousePos(string window, float x, float y);
};

#define INPUT Input::GetInstance()