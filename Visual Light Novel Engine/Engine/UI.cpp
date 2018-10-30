#include "../stdafx.h"
#include "UI.h"
#include "../Core/Util.h"


UI::UI(UIType type, string window, string defaultImage, string overImage, string pressImage)
{
	this->window = window;
	texturePath[0] = defaultImage;
	texturePath[1] = overImage;
	texturePath[2] = pressImage;

	object2D = new Object2D(window, defaultImage);
	object2D->SetAnchor(ImageAnchor::LEFT_TOP);

	isOver = false;
	isActive = true;
	this->type = type;

	uiColor = D3DXVECTOR4(1, 1, 1, 1);
}

UI::~UI()
{
	SAFE_DELETE(object2D);
}

void UI::ChangeImage(string defaultImage, string overImage, string pressImage)
{
	texturePath[0] = defaultImage;
	texturePath[1] = overImage;
	texturePath[2] = pressImage;

	object2D->ChangeTexture(defaultImage);
}

void UI::Update()
{
	if (type == UIType::SINGLE)
	{
		object2D->SetPosition(position);
		object2D->SetRotation(rotation);
		object2D->SetScale(scale);

		object2D->Update();

		if (!isActive || IsDrag()) return;

		if (object2D->IsOver())
		{
			if (!isOver)
			{
				isOver = true;
				if (texturePath[1] != "")
					object2D->ChangeTexture(texturePath[1]);
				else
					object2D->SetColor(D3DXVECTOR4(uiColor.x * 0.8, uiColor.y * 0.8, uiColor.z * 0.8, 1));
			}
			if (INPUT->GetKeyDown(VK_LBUTTON))
			{
				if (texturePath[2] != "")
					object2D->ChangeTexture(texturePath[2]);
				else
					object2D->SetColor(D3DXVECTOR4(uiColor.x * 0.5, uiColor.y * 0.5, uiColor.z * 0.5, 1));
				Action(actionData);
			}
			if (INPUT->GetKeyUp(VK_LBUTTON))
			{
				if (texturePath[1] != "")
					object2D->ChangeTexture(texturePath[1]);
				else
					object2D->SetColor(D3DXVECTOR4(uiColor.x * 0.8, uiColor.y * 0.8, uiColor.z * 0.8, 1));
			}
		}
		else if (isOver)
		{
			isOver = false;
			object2D->ChangeTexture(texturePath[0]);
		}
		else
			object2D->SetColor(D3DXVECTOR4(uiColor.x, uiColor.y, uiColor.z, 1));
	}

	Drag();
}

void UI::Render()
{
	if (type == UIType::SINGLE)
		object2D->Render();

	if (type == UIType::LISTV || type == UIType::LISTH)
	{
		Render(-1);
	}
}

void UI::Render(int maxList)
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
		if (type == UIType::LISTV)
			pos.y += i * desc.Height;
		if (type == UIType::LISTH)
			pos.x += i * desc.Width;
		object2D->SetPosition(pos);

		D3DXVECTOR2 anchorPos;

		if (object2D->GetAnchor() == ImageAnchor::LEFT_TOP) 
			anchorPos = D3DXVECTOR2(pos.x + desc.Width / 2, pos.y + 8 * scale.y);
		if (object2D->GetAnchor() == ImageAnchor::RIGHT_TOP)
			anchorPos = D3DXVECTOR2(pos.x - desc.Width / 2, pos.y + 8 * scale.y);
		if (object2D->GetAnchor() == ImageAnchor::LEFT_BOTTOM)
			anchorPos = D3DXVECTOR2(pos.x + desc.Width / 2, pos.y + (8 * scale.y) - desc.Height);
		if (object2D->GetAnchor() == ImageAnchor::RIGHT_BOTTOM)
			anchorPos = D3DXVECTOR2(pos.x - desc.Width / 2, pos.y + (8 * scale.y) - desc.Height);
		WRITE->Text(listData[i].Text, anchorPos, window, 20 * scale.y, L"³ª´®½ºÄù¾î", D3DXCOLOR(0, 0, 0, 1));

		if (!isActive || IsDrag()) return;

		if (object2D->IsOver())
		{
			if (!isOver)
			{
				isOver = true;
				if (texturePath[1] != "")
					object2D->ChangeTexture(texturePath[1]);
				else
					object2D->SetColor(D3DXVECTOR4(uiColor.x * 0.8, uiColor.y * 0.8, uiColor.z * 0.8, 1));
			}
			if (INPUT->GetKeyDown(VK_LBUTTON))
			{
				if (texturePath[2] != "")
					object2D->ChangeTexture(texturePath[2]);
				else
					object2D->SetColor(D3DXVECTOR4(uiColor.x * 0.5, uiColor.y * 0.5, uiColor.z * 0.5, 1));
				Action(listData[i].Action);
			}
			if (INPUT->GetKeyUp(VK_LBUTTON))
			{
				if (texturePath[1] != "")
					object2D->ChangeTexture(texturePath[1]);
				else
					object2D->SetColor(D3DXVECTOR4(uiColor.x * 0.8, uiColor.y * 0.8, uiColor.z * 0.8, 1));
			}
		}
		else if (isOver)
		{
			isOver = false;
			object2D->ChangeTexture(texturePath[0]);
			object2D->SetColor(D3DXVECTOR4(uiColor.x, uiColor.y, uiColor.z, 1));
		}
		object2D->Render();
	}
}

void UI::SetActive(bool active)
{
	if (active && !isActive)
		object2D->SetColor(D3DXVECTOR4(1, 1, 1, 1));
	else if (!active && isActive)
		object2D->SetColor(D3DXVECTOR4(0.8, 0.8, 0.8, 1));
	isActive = active;
}

bool UI::IsOver()
{
	if (type == UIType::SINGLE)
		return object2D->IsOver();
	auto desc = object2D->GetTexture2D()->GetDesc();
	desc.Width *= scale.x;
	desc.Height *= scale.y;
	RECT rc;
	rc.left = position.x;
	rc.top = position.y;
	if (object2D->GetAnchor() == ImageAnchor::RIGHT_TOP) rc.left -= desc.Width;
	if (object2D->GetAnchor() == ImageAnchor::LEFT_BOTTOM) rc.top -= desc.Height;
	if (object2D->GetAnchor() == ImageAnchor::RIGHT_BOTTOM) { rc.left -= desc.Width; rc.top -= desc.Height; }
	if (type == UIType::LISTV)
	{
		rc.right = rc.left + desc.Width;
		rc.bottom = rc.top + desc.Height * listData.size();
	}
	if (type == UIType::LISTH)
	{
		rc.right = rc.left + desc.Width * listData.size();
		rc.bottom = rc.top + desc.Height;
	}
	if (PtInRect(&rc, INPUT->GetMousePos(window)) && IsFocus(window))
		return true;
	return false;
}

void UI::Action(UIAction data)
{
	if (data.Type == UIActionType::CHANGE_SCENE)
	{
		SCENE->ChangeScene(data.ChangeSceneName, data.ChangeSceneWindow);
	}
	if (data.Type == UIActionType::CUSTOM_FUNCTION)
	{
		data.CustomFunction();
	}
}
