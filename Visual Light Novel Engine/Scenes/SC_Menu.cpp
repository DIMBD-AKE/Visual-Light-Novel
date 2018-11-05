#include "../stdafx.h"
#include "SC_Menu.h"
#include "../Engine/Object/Object.h"
#include "../Core/Util.h"
#include "SC_Editor.h"
#include "../Engine/EngineIO.h"

using namespace nlohmann;

SC_Menu::SC_Menu()
	: sideUIObject(nullptr)
{
	
}


SC_Menu::~SC_Menu()
{
}

void SC_Menu::Init()
{
	vector<UIObjectList> list;
	UIObjectList listData;
	UIObjectAction action;
	action.Type = UIObjectActionType::CHANGE_SCENE;
	action.ChangeSceneWindow = "Game";

	action.ChangeSceneName = "Preview";
	listData.Text = L"Preview";
	listData.Action = action;
	list.push_back(listData);
	action.ChangeSceneName = "Editor";
	listData.Text = L"Editor";
	listData.Action = action;
	list.push_back(listData);

	action.Type = UIObjectActionType::CUSTOM_FUNCTION;
	action.CustomFunction = bind(&EngineIO::ProjectLoad);
	listData.Text = L"Load";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&EngineIO::ProjectSave);
	listData.Text = L"Save";
	listData.Action = action;
	list.push_back(listData);

	sideUIObject = new UIObject(UIObjectType::LISTV, "Menu", "Images/UI/Menu Button.png");
	sideUIObject->SetList(list);
}

void SC_Menu::Update()
{
	sideUIObject->Update();}

void SC_Menu::Render()
{
	sideUIObject->Render();
}

bool SC_Menu::Release()
{
	SAFE_DELETE(sideUIObject);
	return true;
}