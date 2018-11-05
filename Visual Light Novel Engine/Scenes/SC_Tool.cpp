#include "../stdafx.h"
#include "SC_Tool.h"
#include "../Engine/Object/Object.h"
#include "../Scenes/SC_Editor.h"
#include "../Engine/Layer.h"
#include "../Engine/Blueprint/Blueprint.h"
#include "../Core/Util.h"
#include "../Engine/EngineIO.h"

SC_Tool::SC_Tool()
{
}


SC_Tool::~SC_Tool()
{
}

void SC_Tool::Init()
{
	UIObjectAction action;
	action.Type = UIObjectActionType::CUSTOM_FUNCTION;

	action.CustomFunction = bind(&SC_Tool::CreateCharacter, this);
	toolUIObject[0] = new UIObject(UIObjectType::SINGLE, "Tool", "Images/UI/Tool - Character.png");
	toolUIObject[0]->SetAction(action);

	action.CustomFunction = bind(&SC_Tool::CreateImage, this);
	toolUIObject[1] = new UIObject(UIObjectType::SINGLE, "Tool", "Images/UI/Tool - Image.png");
	toolUIObject[1]->SetPosition(50, 0, 0);
	toolUIObject[1]->SetAction(action);

	action.CustomFunction = bind(&SC_Tool::CreateUIObject, this);
	toolUIObject[2] = new UIObject(UIObjectType::SINGLE, "Tool", "Images/UI/Tool - UI.png");
	toolUIObject[2]->SetPosition(100, 0, 0);
	toolUIObject[2]->SetAction(action);
}

void SC_Tool::Update()
{
	string name = SCENE->GetCurrentSceneName("Game");
	if (name.compare("Preview") == 0)
	{
		toolUIObject[0]->SetActive(false);
		toolUIObject[1]->SetActive(false);
		toolUIObject[2]->SetActive(false);
	}
	if (name.compare("Editor") == 0)
	{
		toolUIObject[0]->SetActive(true);
		toolUIObject[1]->SetActive(true);
		toolUIObject[2]->SetActive(true);
	}

	for (int i = 0; i < TOOLCOUNT; i++)
		toolUIObject[i]->Update();
}

void SC_Tool::Render()
{
	for (int i = 0; i < TOOLCOUNT; i++)
		toolUIObject[i]->Render();
}

bool SC_Tool::Release()
{
	for (int i = 0; i < TOOLCOUNT; i++)
		SAFE_DELETE(toolUIObject[i]);
	return true;
}

void SC_Tool::CreateImage()
{
	string openPath = EngineIO::OpenFile(FileType::IMAGE);
	if (openPath != "")
	{
		Object2D * object = new Object2D("Game", openPath);
		object->CreateBlueprint(BlueprintObjectType::IMAGE);
		object->SetPosition(640, 360, 0);

		dynamic_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetLayer()->AddObject(0, object);
	}
}

void SC_Tool::CreateCharacter()
{
	string openPath = EngineIO::OpenFile(FileType::JSON);
	if (openPath != "")
	{
		ElementObject * object = new ElementObject("Game", openPath);
		object->SetPosition(640, 360, 0);
		object->CreateBlueprint(BlueprintObjectType::ELEMENT);

		if (!object->GetElement())
		{
			SAFE_DELETE(object);
			return;
		}

		dynamic_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetLayer()->AddObject(0, object);
		
	}
}

void SC_Tool::CreateUIObject()
{
}