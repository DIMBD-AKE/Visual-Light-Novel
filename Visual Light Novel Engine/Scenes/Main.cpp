#include "../stdafx.h"
#include "Main.h"
#include "../Scenes/SC_Preview.h"
#include "../Scenes/SC_Menu.h"
#include "../Scenes/SC_Tool.h"
#include "../Scenes/SC_Editor.h"
#include "../Scenes/SC_Blueprint.h"

Main::Main()
{
}


Main::~Main()
{
}

void Main::Init()
{
	SCENE->AddScene(new SC_Preview(), "Preview");
	SCENE->ChangeScene("Preview", "Game");

	SCENE->AddScene(new SC_Menu(), "Menu");
	SCENE->ChangeScene("Menu", "Menu");

	SCENE->AddScene(new SC_Tool(), "Tool");
	SCENE->ChangeScene("Tool", "Tool");

	SCENE->AddScene(new SC_Editor(), "Editor");

	SCENE->AddScene(new SC_Blueprint(), "Blueprint");
	SCENE->ChangeScene("Blueprint", "Blueprint");
}

void Main::Update()
{
	SCENE->Update();

	WRITE->SetAlign(WriteAlign::CENTER);
	string temp = SCENE->GetCurrentSceneName("Game");
	wstring currentSceneName;
	currentSceneName.assign(temp.begin(), temp.end());
	wstring text = L"ÇöÀç¾À : " + currentSceneName + L"\n"
		+ L"DT : " + to_wstring(PROFILER->GetDeltaTime()) + L"\n"
		+ L"FPS : " + to_wstring(PROFILER->GetFPS()) + L"\n"
		+ L"Mem : " + to_wstring(PROFILER->GetMemoryUsage()) + L"MB\n";
	for (auto window : SETTING->GetWindow())
		if (IsFocus(window.second.SettingName))
		{
			text += L"X : " + to_wstring(INPUT->GetMousePos(window.second.SettingName).x) +
				L" Y : " + to_wstring(INPUT->GetMousePos(window.second.SettingName).y);
			break;
		}
	WRITE->Text(text, D3DXVECTOR2(80, 240), "Menu");
}

void Main::Render()
{
	SCENE->Render();
}

bool Main::Release()
{
	SCENE->Destroy();
	WRITE->Destroy();
	RESOURCES->Destroy();

	return true;
}
