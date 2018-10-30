#include "../stdafx.h"
#include "Settings.h"

void Settings::AddWindow(string name, Setting setting)
{
	if (settings.find(name) != settings.end())
		return;

	setting.SettingName = name;
	settings[name] = setting;
}

Setting * Settings::GetWindow(string name)
{
	if (settings.find(name) == settings.end())
		return nullptr;

	return &settings[name];
}
