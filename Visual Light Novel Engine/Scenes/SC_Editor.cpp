#include "../stdafx.h"
#include "SC_Editor.h"
#include "../Engine/Object/Object.h"
#include <commdlg.h>

SC_Editor::SC_Editor()
	: editCircle(nullptr)
	, editType(EditType::NONE)
{
}


SC_Editor::~SC_Editor()
{
}

void SC_Editor::Init()
{
	ChangeScene(GameScene::TITLE);
	// Anchor UIObject

	D3DXVECTOR3 anchorPos(35, 95, 0);
	UIObjectAction action;
	action.Type = UIObjectActionType::CUSTOM_FUNCTION;

	action.CustomFunction = bind(&SC_Editor::ChangeAnchor, this, VertexAnchor::LEFT_TOP);
	anchorUIObject[0] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Anchor Corner.png");
	anchorUIObject[0]->SetPosition(anchorPos);
	anchorUIObject[0]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	anchorUIObject[0]->SetAction(action);

	float size = anchorUIObject[0]->GetObject2D()->GetTexture2D()->GetDesc().Width;

	action.CustomFunction = bind(&SC_Editor::ChangeAnchor, this, VertexAnchor::RIGHT_TOP);
	anchorUIObject[1] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Anchor Corner.png");
	anchorUIObject[1]->SetPosition(anchorPos.x + size * 1.5, anchorPos.y, 0);
	anchorUIObject[1]->SetRotation(0, 0, -90);
	anchorUIObject[1]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	anchorUIObject[1]->SetAction(action);

	action.CustomFunction = bind(&SC_Editor::ChangeAnchor, this, VertexAnchor::LEFT_BOTTOM);
	anchorUIObject[2] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Anchor Corner.png");
	anchorUIObject[2]->SetPosition(anchorPos.x, anchorPos.y + size, 0);
	anchorUIObject[2]->SetRotation(0, 0, 90);
	anchorUIObject[2]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	anchorUIObject[2]->SetAction(action);

	action.CustomFunction = bind(&SC_Editor::ChangeAnchor, this, VertexAnchor::RIGHT_BOTTOM);
	anchorUIObject[3] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Anchor Corner.png");
	anchorUIObject[3]->SetPosition(anchorPos.x + size * 1.5, anchorPos.y + size, 0);
	anchorUIObject[3]->SetRotation(0, 0, 180);
	anchorUIObject[3]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	anchorUIObject[3]->SetAction(action);

	action.CustomFunction = bind(&SC_Editor::ChangeAnchor, this, VertexAnchor::CENTER);
	anchorUIObject[4] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Anchor Center.png");
	anchorUIObject[4]->SetPosition(anchorPos.x + size * 0.75, anchorPos.y + size * 0.5, 0);
	anchorUIObject[4]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	anchorUIObject[4]->SetAction(action);

	action.CustomFunction = bind(&SC_Editor::ChangeLayer, this, 1);
	layerUIObject[0] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Layer Arrow.png");
	layerUIObject[0]->SetPosition(170, 135, 0);
	layerUIObject[0]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	layerUIObject[0]->SetAction(action);

	action.CustomFunction = bind(&SC_Editor::ChangeLayer, this, -1);
	layerUIObject[1] = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Layer Arrow.png");
	layerUIObject[1]->SetPosition(220, 135, 0);
	layerUIObject[1]->SetRotation(0, 0, 180);
	layerUIObject[1]->GetObject2D()->SetAnchor(VertexAnchor::CENTER);
	layerUIObject[1]->SetAction(action);

	action.CustomFunction = bind(&SC_Editor::ChangeColor, this);
	colorUIObject = new UIObject(UIObjectType::SINGLE, "Info", "Images/UI/Info - Color.png");
	colorUIObject->SetPosition(22, 180, 0);
	colorUIObject->SetAction(action);

	// Edit Circle

	editCircle = new Object2D("Game", "Images/UI/Edit - Scale Rotation.png");

	vector<UIObjectList> list;
	UIObjectList listData;
	action.CustomFunction = bind(&SC_Editor::ChangeScene, this, GameScene::TITLE);
	listData.Action = action;
	listData.Text = L"Title";
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Editor::ChangeScene, this, GameScene::GAME);
	listData.Action = action;
	listData.Text = L"Game";
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Editor::ChangeScene, this, GameScene::OPTION);
	listData.Action = action;
	listData.Text = L"Option";
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Editor::ChangeScene, this, GameScene::CG);
	listData.Action = action;
	listData.Text = L"CG";
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Editor::ChangeScene, this, GameScene::SELECT);
	listData.Action = action;
	listData.Text = L"Select";
	list.push_back(listData);

	sceneUIObject = new UIObject(UIObjectType::LISTH, "Game", "Images/UI/Menu Button.png");
	sceneUIObject->SetList(list);
	sceneUIObject->SetPosition(240, 0, 0);
}

void SC_Editor::Update()
{
	objectLayer->Update();
	sceneUIObject->Update();

	if (INPUT->GetKeyDown(VK_LBUTTON) && IsFocus("Game") && !sceneUIObject->IsOver())
	{
		if (!SelectEditType())
		{
			if (selectObject.object)
				selectObject.object->ChangeShader("Shaders/Texture2D.hlsl");
			selectObject = objectLayer->GetFront();
			if (selectObject.object)
			{
				editType = EditType::POSITION;
				selectObject.object->InitDrag();
				selectObject.object->ChangeShader("Shaders/Select.hlsl");
			}
		}
	}

	if (INPUT->GetKeyUp(VK_LBUTTON))
		editType = EditType::NONE;

	if (INPUT->GetKeyDown(VK_DELETE) && selectObject.object)
	{
		objectLayer->DeleteObject(selectObject.object);
		selectObject.object = nullptr;
	}

	if (INPUT->GetKeyDown('P'))
		objectLayer->Excute();

	InfoUpdate();
	HierarchyUpdate();
}

void SC_Editor::Render()
{
	objectLayer->Render();
	InfoRender();
	sceneUIObject->Render();
}

bool SC_Editor::Release()
{
	for (auto layer : sceneLayer)
		SAFE_DELETE(layer.second);
	for (int i = 0; i < 5; i++)
		SAFE_DELETE(anchorUIObject[i]);
	for (int i = 0; i < 2; i++)
		SAFE_DELETE(layerUIObject[i]);
	SAFE_DELETE(editCircle);
	SAFE_DELETE(colorUIObject);
	SAFE_DELETE(sceneUIObject);

	return true;
}

void SC_Editor::SetSceneLayer(map<GameScene, Layer*> sceneLayer)
{
	for (auto layer : this->sceneLayer)
		SAFE_DELETE(layer.second);

	this->sceneLayer = sceneLayer;

	if (this->sceneLayer.empty())
		ChangeScene(GameScene::TITLE);
	else
	{
		objectLayer = this->sceneLayer.begin()->second;
		selectObject.object = nullptr;
	}
}

bool SC_Editor::SelectEditType()
{
	if (!selectObject.object) return false;
	D3DXVECTOR2 mouse = D3DXVECTOR2(INPUT->GetMousePos("Game").x, INPUT->GetMousePos("Game").y);
	D3DXVECTOR2 pos = D3DXVECTOR2(selectObject.object->GetPosition().x, selectObject.object->GetPosition().y);
	if (D3DXVec2Length(&(pos - mouse)) < 15)
	{
		editType = EditType::POSITION;
		selectObject.object->InitDrag();
		return true;
	}
	if (D3DXVec2Length(&(pos - mouse)) < 60)
	{
		editType = EditType::SCALE;
		startMouse = D3DXVECTOR2(INPUT->GetMousePos("Game").x, INPUT->GetMousePos("Game").y);
		return true;
	}
	if (D3DXVec2Length(&(pos - mouse)) < 90 && D3DXVec2Length(&(pos - mouse)) > 60)
	{
		editType = EditType::ROTATION;
		startMouse = D3DXVECTOR2(INPUT->GetMousePos("Game").x, INPUT->GetMousePos("Game").y);
		return true;
	}
	return false;
}

void SC_Editor::ApplyEdit()
{
	D3DXVECTOR2 mouse = D3DXVECTOR2(INPUT->GetMousePos("Game").x, INPUT->GetMousePos("Game").y);
	D3DXVECTOR2 pos = D3DXVECTOR2(selectObject.object->GetPosition().x, selectObject.object->GetPosition().y);
	if (editType == EditType::SCALE)
	{
		float width = selectObject.object->GetTexture2D()->GetDesc().Width;
		float height = selectObject.object->GetTexture2D()->GetDesc().Height;
		float len = (D3DXVec2Length(&(pos - startMouse)) > D3DXVec2Length(&(pos - mouse))) ?
			-D3DXVec2Length(&(startMouse - mouse)) : D3DXVec2Length(&(startMouse - mouse));
		float value = selectObject.object->GetScale().x + ((width + len) / width - 1);

		INPUT->SetMousePos("Game", startMouse.x, startMouse.y);

		value = fabs(value);
		selectObject.object->SetScale(value, value, value);
	}
	if (editType == EditType::ROTATION)
	{
		D3DXVECTOR2 delta = mouse - pos;
		D3DXVec2Normalize(&delta, &delta);
		float angle = D3DXToDegree(acos(delta.x));
		if (mouse.y > pos.y)
			angle = 360 - angle;
		selectObject.object->SetRotation(0, 0, angle);
	}
}

void SC_Editor::ChangeColor()
{
	COLORREF custColor[16];

	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));

	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = SETTING->GetWindow("Info")->WindowHandle;
	cc.lpCustColors = (LPDWORD)custColor;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	ChooseColor(&cc);

	D3DXCOLOR color;
	color = D3DXCOLOR(cc.rgbResult);
	float temp = color.r;
	color.r = color.b;
	color.b = temp;
	color.a = 1;

	selectObject.object->SetColor(D3DXVECTOR4(color.r, color.g, color.b, color.a));
	colorUIObject->SetUIObjectColor(D3DXVECTOR4(color.r, color.g, color.b, color.a));
}

void SC_Editor::InfoUpdate()
{
	if (!selectObject.object) return;

	for (int i = 0; i < 5; i++)
		anchorUIObject[i]->Update();
	for (int i = 0; i < 2; i++)
		layerUIObject[i]->Update();

	colorUIObject->SetUIObjectColor(selectObject.object->GetColor());
	colorUIObject->Update();

	editCircle->SetPosition(selectObject.object->GetPosition());

	ApplyEdit();

	WRITE->SetAlign(WriteAlign::LEFT);

	if (editType == EditType::POSITION)
		WRITE->Text(L"X : " + to_wstring((int)selectObject.object->GetPosition().x) +
			L", Y : " + to_wstring((int)selectObject.object->GetPosition().y), D3DXVECTOR2(0, 0), "Info", 20, L"³ª´®½ºÄù¾î", D3DXCOLOR(0, 0.8, 0, 1));
	else
		WRITE->Text(L"X : " + to_wstring((int)selectObject.object->GetPosition().x) +
			L", Y : " + to_wstring((int)selectObject.object->GetPosition().y), D3DXVECTOR2(0, 0), "Info");

	if (editType == EditType::ROTATION)
		WRITE->Text(L"Rotation : " + to_wstring(selectObject.object->GetRotation().z), D3DXVECTOR2(0, 20), "Info", 20, L"³ª´®½ºÄù¾î", D3DXCOLOR(0, 0.8, 0, 1));
	else
		WRITE->Text(L"Rotation : " + to_wstring(selectObject.object->GetRotation().z), D3DXVECTOR2(0, 20), "Info");

	if (editType == EditType::SCALE)
		WRITE->Text(L"Scale : " + to_wstring(selectObject.object->GetScale().x), D3DXVECTOR2(0, 40), "Info", 20, L"³ª´®½ºÄù¾î", D3DXCOLOR(0, 0.8, 0, 1));
	else
		WRITE->Text(L"Scale : " + to_wstring(selectObject.object->GetScale().x), D3DXVECTOR2(0, 40), "Info");

	WRITE->Text(L"Layer : " + to_wstring(selectObject.layer), D3DXVECTOR2(145, 75), "Info");

	if (INPUT->GetKeyDown(VK_RETURN))
		editType = EditType::NONE;
}

void SC_Editor::InfoRender()
{
	if (!selectObject.object) return;

	for (int i = 0; i < 5; i++)
		anchorUIObject[i]->Render();
	for (int i = 0; i < 2; i++)
		layerUIObject[i]->Render();
	if (editCircle)
		editCircle->Render();
	colorUIObject->Render();
}

void SC_Editor::HierarchyUpdate()
{
	WRITE->SetAlign(WriteAlign::LEFT);
	auto layers = objectLayer->GetLayers();
	int objCount = -1;
	int layerCount = -1;
	float layerFontSize = 28;
	float objFontSize = 20;
	float deltaSize = layerFontSize - objFontSize;
	float spaceSize = 10;
	for (auto& layer : *layers)
	{
		if (layer.second.size() > 0)
		{
			layerCount++;
			objCount++;
			WRITE->Text(L"Layer " + to_wstring(layer.first), D3DXVECTOR2(0, objFontSize * objCount + (deltaSize + spaceSize) * layerCount), "Hierarchy", layerFontSize);
		}
		for (auto& object : layer.second)
		{
			objCount++;
			wstring ws;
			string name = object->GetTexture2D()->GetFileName();
			ws.assign(name.begin(), name.end());

			D3DXCOLOR resultColor = D3DXCOLOR(1, 1, 1, 1);
			
			RECT rc;
			rc.left = 0;
			rc.top = objFontSize * objCount + (deltaSize + spaceSize) * layerCount + deltaSize;
			rc.right = 250;
			rc.bottom = rc.top + objFontSize;
			
			if (PtInRect(&rc, INPUT->GetMousePos("Hierarchy")))
			{
				resultColor = D3DXCOLOR(0.5, 0.5, 0.5, 1);
				if (INPUT->GetKeyDown(VK_LBUTTON))
				{
					if (selectObject.object)
					{
						selectObject.object->ChangeShader("Shaders/Texture2D.hlsl");
						selectObject.object->SetColor(D3DXVECTOR4(1, 1, 1, 1));
					}
					selectObject.layer = layer.first;
					selectObject.object = object;
					selectObject.object->ChangeShader("Shaders/Select.hlsl");
				}
			}

			if (object == selectObject.object)
				resultColor = D3DXCOLOR(0, 0.8, 0, 1);

			WRITE->Text(ws, D3DXVECTOR2(0, objFontSize * objCount + (deltaSize + spaceSize) * layerCount + deltaSize), "Hierarchy", objFontSize, L"³ª´®½ºÄù¾î", resultColor);
		}
	}
}

void SC_Editor::ChangeScene(GameScene scene)
{
	if (sceneLayer.find(scene) == sceneLayer.end())
		sceneLayer[scene] = new Layer();

	objectLayer = sceneLayer[scene];
	if (selectObject.object)
	{
		selectObject.object->ChangeShader("Shaders/Texture2D.hlsl");
		selectObject.object->SetColor(D3DXVECTOR4(1, 1, 1, 1));
	}
	selectObject.object = nullptr;
}
