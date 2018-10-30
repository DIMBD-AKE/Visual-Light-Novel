#pragma once

struct Setting
{
	float Width, Height, FOV;
	bool IsVSync, IsFullScreen, IsOrtho, IsMenu;
	D3D11_VIEWPORT Viewport;
	string AppName;
	HWND WindowHandle;
	HINSTANCE WindowInstance;
	string SettingName;
};

class Settings
{
	SINGLETON_H(Settings)

public:
	void AddWindow(string name, Setting setting);
	Setting * GetWindow(string name);
	map<string, Setting> & GetWindow() { return settings; }

private:
	map<string, Setting> settings;

};

#define SETTING Settings::GetInstance()
#define IsFocus(window) (SETTING->GetWindow(window)->WindowHandle == GetFocus())