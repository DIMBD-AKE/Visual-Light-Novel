#include "../stdafx.h"
#include "Window.h"
#include "../Scenes/Main.h"
#include <fstream>

function<void(UINT, UINT)> Window::OnResize = nullptr;

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (Window::OnResize != nullptr && wParam != SIZE_MINIMIZED)
			Window::OnResize(lParam & 0xffff, (lParam >> 16) & 0xffff);
		break;
	case WM_DESTROY:
	{
		ofstream file("Engine.ini");
		file.close();
		for (auto& window : SETTING->GetWindow())
		{
			RECT rc;
			GetWindowRect(SETTING->GetWindow(window.second.SettingName)->WindowHandle, &rc);
			string pos = to_string(rc.left) + ", " + to_string(rc.top);
			char dir[256];
			GetCurrentDirectory(256, dir);
			strcat(dir, "//Engine.ini");
			WritePrivateProfileString("Window", window.second.SettingName.c_str(), pos.c_str(), dir);
		}
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}

Window::Window()
{
}

Window::~Window()
{
	
}

void Window::Init(Setting * setting)
{
	if (find(nameList.begin(), nameList.end(), setting->AppName) == nameList.end())
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = setting->WindowInstance;
		wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = setting->AppName.c_str();
		wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
		WORD wHr = RegisterClassEx(&wcex);
		assert(wHr != 0);
		nameList.push_back(setting->AppName);
	}

	if (setting->IsFullScreen)
	{
		DEVMODE devMode;
		ZeroMemory(&devMode, sizeof(DEVMODE));

		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = setting->Width;
		devMode.dmPelsHeight = setting->Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	DWORD option = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (setting->IsFullScreen == true)
		option |= WS_POPUP;
	else
		option |= WS_OVERLAPPED;
	if (setting->IsMenu == true)
		option |= WS_SYSMENU;

	HWND handle = CreateWindowEx
	(
		WS_EX_APPWINDOW,
		setting->AppName.c_str(),
		setting->AppName.c_str(),
		option,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		setting->WindowInstance,
		nullptr
	);
	assert(handle != nullptr);
	setting->WindowHandle = handle;

	RECT rect;
	ZeroMemory(&rect, sizeof(RECT));

	rect.left = 0;
	rect.top = 0;
	rect.right = setting->Width;
	rect.bottom = setting->Height;

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - setting->Width) * 0.5f;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - setting->Height) * 0.5f;

	char buffer[64];
	char dir[256];
	GetCurrentDirectory(256, dir);
	strcat(dir, "//Engine.ini");
	GetPrivateProfileString("Window", setting->SettingName.c_str(), NULL, buffer, 64, dir);
	if (buffer[0] != '\0')
	{
		char * context;
		string x = strtok_s(buffer, ", ", &context);
		centerX = atoi(x.c_str());
		centerY = atoi(context);
	}

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		setting->WindowHandle,
		centerX,
		centerY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		TRUE
	);
	SetForegroundWindow(setting->WindowHandle);
	SetFocus(setting->WindowHandle);
	ShowCursor(TRUE);

	ShowWindow(setting->WindowHandle, SW_SHOWNORMAL);
	UpdateWindow(setting->WindowHandle);
}

WPARAM Window::Run()
{
	for (auto& window : SETTING->GetWindow())
	{
		Init(&window.second);
		GRAPHICS->Init(&window.second);
		WRITE->Init(window.second.SettingName);
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Main * main = new Main();
	main->Init();

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			INPUT->Update();
			PROFILER->Update();

			GRAPHICS->BeginScene();
			WRITE->BeginDraw();

			main->Update();
			main->Render();
					   
			WRITE->EndDraw();
			GRAPHICS->EndScene();
		}
	}

	main->Release();
	SAFE_DELETE(main);

	return msg.wParam;
}
