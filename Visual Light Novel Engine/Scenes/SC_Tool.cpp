#include "../stdafx.h"
#include "SC_Tool.h"
#include "../Engine/UI.h"
#include "../Scenes/SC_Editor.h"
#include "../Engine/Layer.h"
#include "../Engine/Blueprint/Blueprint.h"
#include "../Core/Util.h"

SC_Tool::SC_Tool()
	: btnImage(nullptr)
	, btnUI(nullptr)
{
}


SC_Tool::~SC_Tool()
{
}

void SC_Tool::Init()
{
	UIAction action;
	action.Type = UIActionType::CUSTOM_FUNCTION;
	action.CustomFunction = bind(&SC_Tool::CreateObject2D, this);

	btnImage = new UI(UIType::SINGLE, "Tool", "Images/UI/Tool - Image.png");
	btnImage->SetAction(action);

	action.CustomFunction = bind(&SC_Tool::CreateUI, this);

	btnUI = new UI(UIType::SINGLE, "Tool", "Images/UI/Tool - UI.png");
	btnUI->SetPosition(50, 0, 0);
	btnUI->SetAction(action);
}

void SC_Tool::Update()
{
	if (SCENE->GetCurrentSceneName("Game") != "Editor")
	{
		btnImage->SetActive(false);
		btnUI->SetActive(false);
	}
	else
	{
		btnImage->SetActive(true);
		btnUI->SetActive(true);
	}
	btnImage->Update();
	btnUI->Update();
}

void SC_Tool::Render()
{
	btnImage->Render();
	btnUI->Render();
}

bool SC_Tool::Release()
{
	SAFE_DELETE(btnImage);
	SAFE_DELETE(btnUI);
	return true;
}

void SC_Tool::CreateObject2D()
{
	string openPath = Util::OpenFile(Util::FileType::IMAGE);
	if (openPath != "")
	{
		Object2D * object = new Object2D("Game", openPath);
		object->CreateBlueprint(BlueprintObjectType::IMAGE);
		object->SetPosition(640, 360, 0);

		dynamic_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetLayer()->AddObject(0, object);
	}
}

void SC_Tool::CreateUI()
{
}