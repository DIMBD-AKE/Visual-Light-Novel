#include "../stdafx.h"
#include "SC_Tool.h"
#include "../Engine/Object/Object.h"
#include "../Scenes/SC_Editor.h"
#include "../Engine/Layer.h"
#include "../Engine/Blueprint/Blueprint.h"
#include "../Engine/Blueprint/BP_Function.h"
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

	action.CustomFunction = bind(&SC_Tool::CreateBackground, this);
	toolUIObject[3] = new UIObject(UIObjectType::SINGLE, "Tool", "Images/UI/Tool - Background.png");
	toolUIObject[3]->SetPosition(0, 50, 0);
	toolUIObject[3]->SetAction(action);

	action.CustomFunction = bind(&SC_Tool::CreateContent, this);
	toolUIObject[4] = new UIObject(UIObjectType::SINGLE, "Tool", "Images/UI/Tool - Content.png");
	toolUIObject[4]->SetPosition(50, 50, 0);
	toolUIObject[4]->SetAction(action);
}

void SC_Tool::Update()
{
	string name = SCENE->GetCurrentSceneName("Game");
	if (name.compare("Preview") == 0)
	{
		for (int i = 0; i < TOOLCOUNT; i++)
			toolUIObject[i]->SetActive(false);
	}
	if (name.compare("Editor") == 0)
	{
		toolUIObject[0]->SetActive(true);
		toolUIObject[1]->SetActive(true);
		toolUIObject[2]->SetActive(true);
		if (static_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetScene() == GameScene::GAME)
		{
			toolUIObject[3]->SetActive(true);
			toolUIObject[4]->SetActive(true);
		}
		else
		{
			toolUIObject[3]->SetActive(false);
			toolUIObject[4]->SetActive(false);
		}
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
		object->CreateBlueprint(BlueprintObjectType::ELEMENT);
		object->SetPosition(640, 360, 0);

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
	string defaultPath = EngineIO::OpenFile(FileType::IMAGE, "기본 텍스쳐");
	string overPath = ""; 
	string pressPath = "";
	if (defaultPath.size() > 0)
	{
		overPath = EngineIO::OpenFile(FileType::IMAGE, "위에 있을 때 텍스쳐");
		pressPath = EngineIO::OpenFile(FileType::IMAGE, "눌렸을 때 텍스쳐");

		UIObject * object = new UIObject(UIObjectType::SINGLE, "Game", defaultPath, overPath, pressPath);
		object->CreateBlueprint(BlueprintObjectType::UIObject);
		auto * func = dynamic_cast<BP_Function*>(object->GetBlueprint()->Add(BlueprintType::FUNCTION));
		func->SetName("CLICK");
		object->SetPosition(640, 360, 0);

		dynamic_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetLayer()->AddObject(-10, object);
	}
}

void SC_Tool::CreateBackground()
{
	dynamic_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetLayer()->CreateBackground();
}

void SC_Tool::CreateContent()
{
	dynamic_cast<SC_Editor*>(SCENE->GetCurrentScene("Game"))->GetLayer()->CreateContent();
}