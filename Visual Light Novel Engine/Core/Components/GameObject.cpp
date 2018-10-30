#include "../../stdafx.h"
#include "GameObject.h"
#include "../../Engine/Blueprint/Blueprint.h"

GameObject::GameObject()
	: isDrag(false)
	, bpList(nullptr)
	, color(1, 1, 1, 1)
{
}

GameObject::~GameObject()
{
	SAFE_DELETE(bpList);
}

void GameObject::Drag()
{
	if (!isDrag) return;

	auto point = INPUT->GetMousePos(window);
	D3DXVECTOR3 pos = D3DXVECTOR3(point.x, point.y, 0);

	if (INPUT->GetKey(VK_LBUTTON))
	{
		D3DXVECTOR3 movePos = pos - startMousePos;
		position = startPostion + movePos;
	}

	if (INPUT->GetKeyUp(VK_LBUTTON))
		isDrag = false;
}

void GameObject::InitDrag()
{
	if (!IsFocus(window)) return;
	isDrag = true;
	auto point = INPUT->GetMousePos(window);
	startMousePos = D3DXVECTOR3(point.x, point.y, 0);;
	RECT rect;
	GetWindowRect(SETTING->GetWindow(window)->WindowHandle, &rect);
	startPostion = position;
}

void GameObject::CreateBlueprint(BlueprintObjectType type)
{
	bpList = new BlueprintList(type);
}
