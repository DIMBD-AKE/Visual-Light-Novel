#include "../../stdafx.h"
#include "BP_UI.h"
#include "../Object/Object.h"


BP_UI::BP_UI()
{
	type = BlueprintType::UI;
	object2D->SetColor(D3DXVECTOR4(227 / 255.0f, 252 / 255.0f, 191 / 255.0f, 1));
}


BP_UI::~BP_UI()
{
}

void BP_UI::Execute(void * data)
{
	if (subType == BlueprintSubType::UI_GAME_START)
	{

	}
	if (subType == BlueprintSubType::UI_GAME_END)
	{

	}
	if (subType == BlueprintSubType::UI_GAME_SAVE)
	{

	}
	if (subType == BlueprintSubType::UI_GAME_LOAD)
	{

	}
	if (subType == BlueprintSubType::UI_SCENE_CG)
	{

	}
	if (subType == BlueprintSubType::UI_SCENE_DIALOGUE)
	{

	}
	if (subType == BlueprintSubType::UI_SCENE_OPTION)
	{

	}
	if (subType == BlueprintSubType::UI_SCENE_SAVE)
	{

	}
	if (subType == BlueprintSubType::UI_SCENE_SELECT)
	{

	}
	if (subType == BlueprintSubType::UI_SCENE_TITLE)
	{

	}
}

void BP_UI::SubUpdate()
{
	wstring text = L"클릭해서 타입 지정";
	if (subType == BlueprintSubType::UI_GAME_START) text = L"Game Start";
	if (subType == BlueprintSubType::UI_GAME_END) text = L"Game End";
	if (subType == BlueprintSubType::UI_GAME_SAVE) text = L"Game Save";
	if (subType == BlueprintSubType::UI_GAME_LOAD) text = L"Game Load";
	if (subType == BlueprintSubType::UI_SCENE_CG) text = L"To CG";
	if (subType == BlueprintSubType::UI_SCENE_DIALOGUE) text = L"To Dialogue";
	if (subType == BlueprintSubType::UI_SCENE_OPTION) text = L"To Option";
	if (subType == BlueprintSubType::UI_SCENE_SAVE) text = L"To Save";
	if (subType == BlueprintSubType::UI_SCENE_SELECT) text = L"To Select";
	if (subType == BlueprintSubType::UI_SCENE_TITLE) text = L"To Title";
	WRITE->SetAlign(WriteAlign::CENTER);
	WRITE->Text(text, D3DXVECTOR2(object2D->GetPosition()), "Blueprint", 14, L"나눔스퀘어", D3DXCOLOR(0.5, 0.5, 0.5, 1));
}
