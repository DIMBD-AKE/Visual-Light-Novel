#include "../stdafx.h"
#include "SC_Blueprint.h"
#include "../Engine/UI.h"
#include "SC_Editor.h"
#include "../Engine/Blueprint/BP_Object.h"

SC_Blueprint::SC_Blueprint()
	: selectObject(nullptr)
	, createUI(nullptr)
	, seqListUI(nullptr)
	, seqDataListUI(nullptr)
	, dataIndex(-1)
{
}


SC_Blueprint::~SC_Blueprint()
{
}

void SC_Blueprint::Init()
{
	selectObject = static_cast<SC_Editor*>(SCENE->GetScene("Editor"))->GetSelectObjectAddress();

	vector<UIList> list;
	UIList listData;
	UIAction action;
	action.Type = UIActionType::CUSTOM_FUNCTION;

	action.CustomFunction = bind(&SC_Blueprint::CreateBlueprint, this, BlueprintType::FUNCTION);
	listData.Text = L"Function";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::CreateBlueprint, this, BlueprintType::OBJECT);
	listData.Text = L"Object Data";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::CreateBlueprint, this, BlueprintType::FLOAT);
	listData.Text = L"Float";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::CreateBlueprint, this, BlueprintType::SEQUENCE);
	listData.Text = L"Sequence";
	listData.Action = action;
	list.push_back(listData);
	createUI = new UI(UIType::LISTH, "Blueprint", "Images/UI/Menu Button.png");
	createUI->SetList(list);
	createUI->SetAction(action);

	list.clear();
	action.CustomFunction = bind(&SC_Blueprint::ChangeBlueprintSubType, this, BlueprintSubType::SEQUENCE_QUEUE_EASEIN, 4);
	listData.Text = L"Ease In";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::ChangeBlueprintSubType, this, BlueprintSubType::SEQUENCE_QUEUE_EASEOUT, 4);
	listData.Text = L"Ease Out";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::ChangeBlueprintSubType, this, BlueprintSubType::SEQUENCE_QUEUE_EASEINOUT, 4);
	listData.Text = L"Ease In Out";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::ChangeBlueprintSubType, this, BlueprintSubType::SEQUENCE_QUEUE_LINEAR, 4);
	listData.Text = L"Linear";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::ChangeBlueprintSubType, this, BlueprintSubType::SEQUENCE_QUEUE_BEZIER2, 5);
	listData.Text = L"Bezier2";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::ChangeBlueprintSubType, this, BlueprintSubType::SEQUENCE_QUEUE_BEZIER3, 6);
	listData.Text = L"Bezier3";
	listData.Action = action;
	list.push_back(listData);
	seqListUI = new UI(UIType::LISTV, "Blueprint", "Images/UI/Menu Button.png");
	seqListUI->SetList(list);
	seqListUI->SetPosition(0, 100, 0);

	list.clear();
	action.CustomFunction = bind(&SC_Blueprint::SetSubData, this, 0);
	listData.Text = L"Delay";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::SetSubData, this, 1);
	listData.Text = L"Duration";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::SetSubData, this, 2);
	listData.Text = L"Object 0";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::SetSubData, this, 3);
	listData.Text = L"Object 1";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::SetSubData, this, 4);
	listData.Text = L"Object 2";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Blueprint::SetSubData, this, 5);
	listData.Text = L"Object 3";
	listData.Action = action;
	list.push_back(listData);
	seqDataListUI = new UI(UIType::LISTV, "Blueprint", "Images/UI/Menu Button.png");
	seqDataListUI->SetList(list);
	seqDataListUI->GetObject2D()->SetAnchor(ImageAnchor::RIGHT_TOP);
	seqDataListUI->SetPosition(1280, 100, 0);
}

void SC_Blueprint::Update()
{
	if (!*selectObject) return;

	createUI->Update();

	(*selectObject)->GetBlueprint()->Update();
	if ((*selectObject)->GetBlueprint()->GetSelectNode())
	{
		if ((*selectObject)->GetBlueprint()->GetSelectNode()->data->GetType() == BlueprintType::SEQUENCE)
		{
			seqListUI->Update();
			seqDataListUI->Update();
		}
	}
}

void SC_Blueprint::Render()
{
	if (!*selectObject) return;

	createUI->Render();

	(*selectObject)->GetBlueprint()->Render();
	BlueprintNode * selNode = (*selectObject)->GetBlueprint()->GetSelectNode();
	if (selNode)
	{
		if (selNode->data->GetType() == BlueprintType::SEQUENCE)
		{
			seqListUI->Render();
			seqDataListUI->Render(selNode->data->GetSubData().size());
		}
	}
}

bool SC_Blueprint::Release()
{
	SAFE_DELETE(createUI);
	SAFE_DELETE(seqListUI);
	SAFE_DELETE(seqDataListUI);
	return true;
}

void SC_Blueprint::CreateBlueprint(BlueprintType type)
{
	if (!*selectObject) return;

	(*selectObject)->GetBlueprint()->Add(type);
}

void SC_Blueprint::ChangeBlueprintSubType(BlueprintSubType type, int dataSize)
{
	(*selectObject)->GetBlueprint()->GetSelectNode()->data->SetSubType(type, dataSize);
}

void SC_Blueprint::SetSubData(int index)
{
	dataIndex = index;
}
