#include "../stdafx.h"
#include "SC_Element.h"
#include "../Engine/UI.h"


SC_Element::SC_Element()
	: sideUI(nullptr)
{
}


SC_Element::~SC_Element()
{
}

void SC_Element::Init()
{
	vector<UIList> list;
	UIList listData;
	UIAction action;
	action.Type = UIActionType::CUSTOM_FUNCTION;
	
	action.CustomFunction = bind(&SC_Element::Test, this);
	listData.Text = L"캐릭터";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Element::Test, this);
	listData.Text = L"물건";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Element::Test, this);
	listData.Text = L"배경";
	listData.Action = action;
	list.push_back(listData);

	sideUI = new UI(UIType::LISTV, "Game", "Images/UI/Menu Button.png");
	sideUI->SetList(list);
	sideUI->SetScale(1.2, 1.2, 1);
}

void SC_Element::Update()
{
	sideUI->Update();
}

void SC_Element::Render()
{
	sideUI->Render();
}

bool SC_Element::Release()
{
	SAFE_DELETE(sideUI);

	return true;
}

void SC_Element::Test()
{
	int a = 0;
}
