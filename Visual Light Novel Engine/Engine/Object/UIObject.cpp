#include "../../stdafx.h"
#include "UIObject.h"
#include "../../Core/Util.h"
#include "Object.h"


UIObject::UIObject(UIObjectType type, string window, string defaultImage, string overImage, string pressImage)
{
	this->window = window;
	texturePath[0] = defaultImage;
	texturePath[1] = overImage;
	texturePath[2] = pressImage;

	object2D = new Object2D(window, defaultImage);
	object2D->SetAnchor(VertexAnchor::LEFT_TOP);

	isOver = false;
	isActive = true;
	this->type = type;

	UIObjectColor = D3DXVECTOR4(1, 1, 1, 1);
}

UIObject::~UIObject()
{
	SAFE_DELETE(object2D);
}

void UIObject::ChangeImage(string defaultImage, string overImage, string pressImage)
{
	texturePath[0] = defaultImage;
	texturePath[1] = overImage;
	texturePath[2] = pressImage;

	object2D->ChangeTexture(defaultImage);
}

void UIObject::Update()
{
	if (type == UIObjectType::SINGLE)
	{
		object2D->SetPosition(position);
		object2D->SetRotation(rotation);
		object2D->SetScale(scale);

		object2D->Update();

		if (!isActive || IsDrag()) return;

		Detect(-1);
	}

	Drag();
}

void UIObject::Render()
{
	if (type == UIObjectType::SINGLE)
		object2D->Render();

	if (type == UIObjectType::LISTV || type == UIObjectType::LISTH)
	{
		Render(-1);
	}
}

void UIObject::Render(int maxList)
{
	object2D->SetRotation(rotation);
	object2D->SetScale(scale);
	auto desc = object2D->GetTexture2D()->GetDesc();
	desc.Width *= scale.x;
	desc.Height *= scale.y;
	WRITE->SetAlign(WriteAlign::CENTER);
	int count = maxList >= 0 ? Math::Clamp<int>(0, listData.size(), maxList) : listData.size();
	for (int i = 0; i < count; i++)
	{
		D3DXVECTOR3 pos = position;
		if (type == UIObjectType::LISTV)
			pos.y += i * desc.Height;
		if (type == UIObjectType::LISTH)
			pos.x += i * desc.Width;
		object2D->SetPosition(pos);

		D3DXVECTOR2 anchorPos;

		if (object2D->GetAnchor() == VertexAnchor::LEFT_TOP) 
			anchorPos = D3DXVECTOR2(pos.x + desc.Width / 2, pos.y + 8 * scale.y);
		if (object2D->GetAnchor() == VertexAnchor::RIGHT_TOP)
			anchorPos = D3DXVECTOR2(pos.x - desc.Width / 2, pos.y + 8 * scale.y);
		if (object2D->GetAnchor() == VertexAnchor::LEFT_BOTTOM)
			anchorPos = D3DXVECTOR2(pos.x + desc.Width / 2, pos.y + (8 * scale.y) - desc.Height);
		if (object2D->GetAnchor() == VertexAnchor::RIGHT_BOTTOM)
			anchorPos = D3DXVECTOR2(pos.x - desc.Width / 2, pos.y + (8 * scale.y) - desc.Height);
		WRITE->Text(listData[i].Text, anchorPos, window, 20 * scale.y, L"³ª´®½ºÄù¾î", D3DXCOLOR(0, 0, 0, 1));

		if (!isActive || IsDrag()) return;

		Detect(i);
	}
}

void UIObject::SetActive(bool active)
{
	if (active && !isActive)
		object2D->SetColor(D3DXVECTOR4(1, 1, 1, 1));
	else if (!active && isActive)
		object2D->SetColor(D3DXVECTOR4(0.5, 0.5, 0.5, 1));
	isActive = active;
}

bool UIObject::IsOver()
{
	if (type == UIObjectType::SINGLE)
		return object2D->IsOver();
	auto desc = object2D->GetTexture2D()->GetDesc();
	desc.Width *= scale.x;
	desc.Height *= scale.y;
	RECT rc;
	rc.left = position.x;
	rc.top = position.y;
	if (object2D->GetAnchor() == VertexAnchor::RIGHT_TOP) rc.left -= desc.Width;
	if (object2D->GetAnchor() == VertexAnchor::LEFT_BOTTOM) rc.top -= desc.Height;
	if (object2D->GetAnchor() == VertexAnchor::RIGHT_BOTTOM) { rc.left -= desc.Width; rc.top -= desc.Height; }
	if (type == UIObjectType::LISTV)
	{
		rc.right = rc.left + desc.Width;
		rc.bottom = rc.top + desc.Height * listData.size();
	}
	if (type == UIObjectType::LISTH)
	{
		rc.right = rc.left + desc.Width * listData.size();
		rc.bottom = rc.top + desc.Height;
	}
	if (PtInRect(&rc, INPUT->GetMousePos(window)) && IsFocus(window))
		return true;
	return false;
}

Texture2D * UIObject::GetTexture2D()
{
	return object2D->GetTexture2D();
}

void UIObject::Detect(int actionIndex)
{
	D3DXVECTOR4 color = UIObjectColor;
	if (object2D->IsOver())
	{
		if (!isOver)
		{
			isOver = true;
			if (texturePath[1] != "")
				object2D->ChangeTexture(texturePath[1]);
			else
			{
				color *= 0.8;
				object2D->SetColor(color);
			}
		}
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			if (texturePath[2] != "")
				object2D->ChangeTexture(texturePath[2]);
			else
			{
				color *= 0.5;
				object2D->SetColor(color);
			}
			if (actionIndex >= 0)
				Action(listData[actionIndex].Action);
			else
				Action(actionData);
		}
		if (INPUT->GetKeyUp(VK_LBUTTON))
		{
			if (texturePath[1] != "")
				object2D->ChangeTexture(texturePath[1]);
			else
			{
				color *= 0.8;
				object2D->SetColor(color);
			}
		}
	}
	else if (isOver)
	{
		isOver = false;
		object2D->ChangeTexture(texturePath[0]);
		object2D->SetColor(color);
	}
	object2D->Render();
}

void UIObject::Action(UIObjectAction data)
{
	if (data.Type == UIObjectActionType::CHANGE_SCENE)
	{
		SCENE->ChangeScene(data.ChangeSceneName, data.ChangeSceneWindow);
	}
	if (data.Type == UIObjectActionType::CUSTOM_FUNCTION)
	{
		data.CustomFunction();
	}
}
