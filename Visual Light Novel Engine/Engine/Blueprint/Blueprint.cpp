#include "../../stdafx.h"
#include "../../Scenes/SC_Editor.h"
#include "../../Scenes/SC_Blueprint.h"
#include "../../Core/Util.h"
#include "../Object/Object.h"
#include "Blueprint.h"
#include "BP_Sequence.h"
#include "BP_Object.h"
#include "BP_Float.h"
#include "BP_Function.h"
#include "BP_UI.h"


BlueprintList::BlueprintList(BlueprintObjectType type)
	: selectNode(nullptr)
	, elapse(0)
	, inputData("0")
	, type(type)
	, offset(0, 0, 0)
{
	shader = RESOURCES->GetResource<Shader>("Shaders/Line.hlsl", "Blueprint");
	vertexBuffer = new VertexBuffer();
	seq = new Sequence();
	CreateBuffer();
}


BlueprintList::~BlueprintList()
{
	for (auto bp : nodeList)
	{
		SAFE_DELETE(bp->data);
		SAFE_DELETE(bp);
	}
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(seq);
	SAFE_DELETE(shader);
}

Blueprint * BlueprintList::Add(BlueprintType type)
{
	Blueprint * data = nullptr;
	if (type == BlueprintType::SEQUENCE) data = new BP_Sequence();
	if (type == BlueprintType::OBJECT) data = new BP_Object(static_cast<SC_Editor*>(SCENE->GetScene("Editor"))->GetSelectObject());
	if (type == BlueprintType::FLOAT) data = new BP_Float();
	if (type == BlueprintType::FUNCTION) data = new BP_Function();
	if (type == BlueprintType::UI) data = new BP_UI();
	data->GetObject2D()->SetPosition(640, 360, 0);
	data->SetID(time(NULL) + timeGetTime());
	BlueprintNode * node = new BlueprintNode();
	node->data = data;
	node->prev = nullptr;
	node->next = nullptr;
	nodeList.push_back(node);
	cout << "[Blueprint] Create '" + BPFunction::TypeToString(type) + "'" << endl;
	return data;
}

Blueprint * BlueprintList::Add(Blueprint * bp)
{
	BlueprintNode * node = new BlueprintNode();
	node->data = bp;
	node->prev = nullptr;
	node->next = nullptr;
	nodeList.push_back(node);
	cout << "[Blueprint] Create '" + BPFunction::TypeToString(bp->GetType()) + "'" << endl;
	return bp;
}

void BlueprintList::Delete(BlueprintNode * node)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->data->GetID() == node->data->GetID())
		{
			if (nodeList[i]->next)
				nodeList[i]->next->prev = nullptr;
			if (nodeList[i]->prev)
				nodeList[i]->prev->next = nullptr;
			for (auto bp : nodeList)
				for (auto& data : bp->data->GetSubData())
					if (data.Data == nodeList[i]->data)
						data.Data = nullptr;
			SAFE_DELETE(nodeList[i]->data);
			SAFE_DELETE(nodeList[i]);
			nodeList.erase(nodeList.begin() + i);
			return;
		}
	}
}

void BlueprintList::Update()
{
	for (auto bp : nodeList)
	{
		D3DXVECTOR2 pos = D3DXVECTOR2(bp->data->GetObject2D()->GetPosition().x, bp->data->GetObject2D()->GetPosition().y);
		pos.y -= 24;

		D3DXCOLOR color = bp->data->GetColorAvg() > 0.5 ? D3DXCOLOR(0, 0, 0, 1) : D3DXCOLOR(1, 1, 1, 1);

		WRITE->SetAlign(WriteAlign::CENTER);
		if (bp->data->GetType() == BlueprintType::FUNCTION)
			WRITE->Text(L"Function", pos, "Blueprint", 20, L"³ª´®½ºÄù¾î", color);
		if (bp->data->GetType() == BlueprintType::SEQUENCE)
			WRITE->Text(L"Sequence", pos, "Blueprint", 20, L"³ª´®½ºÄù¾î", color);
		if (bp->data->GetType() == BlueprintType::OBJECT)
			WRITE->Text(L"Object", pos, "Blueprint", 20, L"³ª´®½ºÄù¾î", color);
		if (bp->data->GetType() == BlueprintType::FLOAT)
			WRITE->Text(L"Float", pos, "Blueprint", 20, L"³ª´®½ºÄù¾î", color);
		if (bp->data->GetType() == BlueprintType::UI)
			WRITE->Text(L"UI", pos, "Blueprint", 20, L"³ª´®½ºÄù¾î", color);
		
		bp->data->GetObject2D()->SetOffset(offset);

		if (bp->data->Update())
			break;
	}

	if (INPUT->GetKeyDown('X'))
		for (auto bp : nodeList)
			if (bp->data->GetObject2D()->IsOver())
			{
				Delete(bp);
				selectNode = nullptr;
				break;
			}

	for (int i = 0; i < 10; i++)
	{
		if (INPUT->GetKeyDown(i + 48))
			inputData += to_string(i);
	}
	for (int i = 'A'; i <= 'Z'; i++)
	{
		if (INPUT->GetKeyDown(i))
			inputData += i;
	}
	if (INPUT->GetKeyDown(VK_OEM_PERIOD))
		inputData += ".";

	if (INPUT->GetKeyDown(VK_SPACE))
		inputData += "_";

	Link();
	SetData();
	MouseDrag();

	elapse += PROFILER->GetDeltaTime() * 2;
}

void BlueprintList::Render()
{
	for (auto bp : nodeList)
	{
		if (bp->next)
			LineRender(bp->data, bp->next->data);
	}
	if (selectNode && selectNode->data->GetType() == BlueprintType::SEQUENCE)
	{
		for (int i = 0; i < selectNode->data->GetSubData().size(); i++)
		{
			if (selectNode->data->GetSubData()[i].Data)
			{
				auto conv = static_cast<BP_Object*>(selectNode->data->GetSubData()[i].Data);
				LineRender(selectNode->data, conv, L"Data " + to_wstring(i + 1));
			}
		}
	}
	for (auto bp : nodeList)
		bp->data->Render();
}

void BlueprintList::Save(json &data, int layer, int objIndex)
{
	data["LAYER"][to_string(layer)]["OBJECT"][objIndex]["BLUEPRINT"]["OFFSET"] = Util::VectorToString(offset);
	for (int i = 0; i < nodeList.size(); i++)
	{
		auto node = nodeList[i];
		
		THISPATHO(layer, objIndex, i)["PREV"] = node->prev ? node->prev->data->GetID() : 0;
		THISPATHO(layer, objIndex, i)["NEXT"] = node->next ? node->next->data->GetID() : 0;

		for (int j = 0; j < node->data->GetSubData().size(); j++)
		{
			auto bp = static_cast<Blueprint*>(node->data->GetSubData()[j].Data);
			THISPATHO(layer, objIndex, i)["DATA"]["SUBDATA"][j] = bp ? bp->GetID() : 0;
		}

		node->data->Save(data, layer, objIndex, i);
	}
}

void BlueprintList::Save(json & data, int objIndex)
{
	data["UI"][objIndex]["BLUEPRINT"]["OFFSET"] = Util::VectorToString(offset);
	for (int i = 0; i < nodeList.size(); i++)
	{
		auto node = nodeList[i];

		THISPATHU(objIndex, i)["PREV"] = node->prev ? node->prev->data->GetID() : 0;
		THISPATHU(objIndex, i)["NEXT"] = node->next ? node->next->data->GetID() : 0;

		for (int j = 0; j < node->data->GetSubData().size(); j++)
		{
			auto bp = static_cast<Blueprint*>(node->data->GetSubData()[j].Data);
			THISPATHU(objIndex, i)["DATA"]["SUBDATA"][j] = bp ? bp->GetID() : 0;
		}

		node->data->Save(data, objIndex, i);
	}
}

void BlueprintList::Save(json & data)
{
	data["BLUEPRINT"]["OFFSET"] = Util::VectorToString(offset);
	for (int i = 0; i < nodeList.size(); i++)
	{
		auto node = nodeList[i];

		THISPATHE(i)["PREV"] = node->prev ? node->prev->data->GetID() : 0;
		THISPATHE(i)["NEXT"] = node->next ? node->next->data->GetID() : 0;

		for (int j = 0; j < node->data->GetSubData().size(); j++)
		{
			auto bp = static_cast<Blueprint*>(node->data->GetSubData()[j].Data);
			THISPATHE(i)["DATA"]["SUBDATA"][j] = bp ? bp->GetID() : 0;
		}

		node->data->Save(data, i);
	}
}

void BlueprintList::Clear()
{
	for (auto bp : nodeList)
	{
		SAFE_DELETE(bp->data);
		SAFE_DELETE(bp);
	}
	nodeList.clear();
}

BlueprintNode * BlueprintList::GetClickNode()
{
	if (INPUT->GetKeyDown(VK_LBUTTON))
		for (auto bp : nodeList)
			if (bp->data->GetObject2D()->IsOver())
				return bp;
	return nullptr;
}

void BlueprintList::Excute(string funcName)
{
	BlueprintNode * head = nullptr;

	for (auto node : nodeList)
		if (node->data->GetType() == BlueprintType::FUNCTION &&
			static_cast<BP_Function*>(node->data)->GetName().compare(funcName) == 0)
				head = node;

	seq->Clear();

	while (head)
	{
		if (head->data->GetType() == BlueprintType::SEQUENCE)
			head->data->Execute(seq);
		head = head->next;
	}
}

void BlueprintList::ExcuteUpdate(GameObject * gameobject)
{
	seq->Play(gameobject);
}

void BlueprintList::Link(vector<BlueprintLinkData> linkData)
{
	for (auto data : linkData)
	{
		BlueprintNode * node = FindNode(data.data);
		node->next = FindNode(data.next);
		node->prev = FindNode(data.prev);
		for (int i = 0; i < data.subdata.size(); i++)
		{
			auto find = FindNode(data.subdata[i]);
			node->data->GetSubData()[i].Data = find ? find->data : nullptr;
		}
	}
}

void BlueprintList::CreateBuffer()
{
	vector<VertexColor> vertices(2);

	vertexBuffer->Create(vertices, "Blueprint");
}

void BlueprintList::Link()
{
	if (selectNode)
	{
		float scale = 1 + sin(elapse * 2) * 0.05f;
		selectNode->data->GetObject2D()->SetScale(scale, scale, scale);
	}

	if (INPUT->GetKeyDown(VK_RBUTTON))
	{
		if (selectNode) selectNode->data->GetObject2D()->SetScale(1, 1, 1);
		selectNode = nullptr;
		inputData = "0";
		for (auto bp : nodeList)
		{
			bp->data->GetObject2D()->SetRotation(0, 0, 0);
			if (bp->data->GetObject2D()->IsOver())
			{
				selectNode = bp;
				if (selectNode->data->GetType() == BlueprintType::OBJECT)
					static_cast<BP_Object*>(selectNode->data)->Preview();
				return;
			}
		}
	}

	if (INPUT->GetKeyUp(VK_RBUTTON) && selectNode)
	{
		for (auto bp : nodeList)
		{
			if (bp->data->GetObject2D()->IsOver())
			{
				if (selectNode == bp)
					return;
				if (bp->prev)
					bp->prev->next = nullptr;
				bp->prev = selectNode;
				if (selectNode->next)
					selectNode->next->prev = nullptr;
				selectNode->next = bp;
				cout << "[Blueprint] Link " + to_string(selectNode->data->GetID()) + " to " + to_string(bp->data->GetID()) << endl;
				return;
			}
		}
		if (selectNode->next)
			selectNode->next->prev = nullptr;
		selectNode->next = nullptr;
		cout << "[Blueprint] Unlink " + to_string(selectNode->data->GetID()) << endl;
	}
}

void BlueprintList::SetData()
{
	int * dataIndex = static_cast<SC_Blueprint*>(SCENE->GetScene("Blueprint"))->GetDataIndex();

	if (selectNode)
	{
		if (*dataIndex >= 0)
			for (auto bp : nodeList)
				if (bp->data->GetType() == selectNode->data->GetSubData()[*dataIndex].Type)
				{
					float angle = sin(elapse * 2) * 10;
					bp->data->GetObject2D()->SetRotation(0, 0, angle);
				}
		if (selectNode->data->GetType() == BlueprintType::FLOAT)
			static_cast<BP_Float*>(selectNode->data)->SetValue(atof(inputData.c_str()));
		if (selectNode->data->GetType() == BlueprintType::FUNCTION)
			static_cast<BP_Function*>(selectNode->data)->SetName(inputData.substr(1, inputData.size()));
	}
	else
		*dataIndex = -1;

	if (INPUT->GetKeyDown(VK_LBUTTON) && *dataIndex >= 0)
	{
		if (selectNode)
		{
			auto node = GetClickNode();
			auto type = selectNode->data->GetSubData()[*dataIndex].Type;
			if (node && node->data->GetType() == type)
			{
				void * data = nullptr;
				if (type == BlueprintType::OBJECT) data = static_cast<BP_Object*>(node->data);
				if (type == BlueprintType::FLOAT) data = static_cast<BP_Float*>(node->data);
				selectNode->data->GetSubData()[*dataIndex].Data = data;
				cout << "[Blueprint] Set " + BPFunction::TypeToString(type) + " is " + to_string(node->data->GetID()) << endl;
			}
			else
				selectNode->data->GetSubData()[*dataIndex].Data = nullptr;
			for (auto bp : nodeList)
				bp->data->GetObject2D()->SetRotation(0, 0, 0);
		}
		*dataIndex = -1;
	}
}

void BlueprintList::LineRender(Blueprint * from, Blueprint * to, wstring comment)
{
	vector<VertexColor> vertices(2);

	float startColor = Math::Clamp<float>(0.3, 1, cos(elapse));
	float endColor = Math::Clamp<float>(0.3, 1, sin(elapse));

	vertices[0].position = from->GetObject2D()->GetPosition("Blueprint");
	vertices[0].color = D3DXCOLOR(startColor, startColor, startColor, 1);
	vertices[1].position = to->GetObject2D()->GetPosition("Blueprint");
	vertices[1].color = D3DXCOLOR(endColor, endColor, endColor, 1);

	if (comment != L"")
	{
		D3DXVECTOR2 pos = D3DXVECTOR2(from->GetObject2D()->GetPosition());
		pos += (D3DXVECTOR2(to->GetObject2D()->GetPosition()) - D3DXVECTOR2(from->GetObject2D()->GetPosition())) / 2;
		WRITE->SetAlign(WriteAlign::CENTER);
		WRITE->Text(comment, pos, "Blueprint");
	}

	vertexBuffer->Mapped(vertices);
	vertexBuffer->IASet();

	shader->RenderShader(vertices.size(), nullptr, D3DXVECTOR4(1, 1, 1, 1), D3DXVECTOR4(1, 1, 1, 1), nullptr, "Blueprint");

	GRAPHICS->GetDeviceContext("Blueprint")->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	GRAPHICS->GetDeviceContext("Blueprint")->Draw(vertices.size(), 0);
}

void BlueprintList::MouseDrag()
{
	D3DXVECTOR3 pos = D3DXVECTOR3(INPUT->GetMousePos("Blueprint").x, 
		INPUT->GetMousePos("Blueprint").y, 0);
	if (INPUT->GetKeyDown(VK_MBUTTON))
	{
		startMousePos = pos;
		startOffset = offset;
	}

	if (INPUT->GetKey(VK_MBUTTON))
	{
		D3DXVECTOR3 movePos = pos - startMousePos;
		offset = startOffset + movePos;
	}
}

BlueprintNode * BlueprintList::FindNode(UINT id)
{
	for (auto& node : nodeList)
		if (node->data->GetID() == id)
			return node;
	return nullptr;
}

Blueprint::Blueprint()
{
	object2D = new Object2D("Blueprint", "Images/UI/Blueprint.png");
	subType = BlueprintSubType::NONE;
}

Blueprint::~Blueprint()
{
	SAFE_DELETE(object2D);
}

bool Blueprint::Update()
{
	SubUpdate();
	object2D->Update();

	if (INPUT->GetKeyDown(VK_LBUTTON) && object2D->IsOver())
	{
		object2D->InitDrag();
		return true;
	}
	return false;
}

void Blueprint::Render()
{
	SubRender();
	object2D->Render();
}

void Blueprint::Save(json & data, int layer, int objIndex, int bpIndex)
{
	THISPATHO(layer, objIndex, bpIndex)["DATA"]["ID"] = id;
	THISPATHO(layer, objIndex, bpIndex)["DATA"]["TYPE"] = BPFunction::TypeToString(type);
	THISPATHO(layer, objIndex, bpIndex)["DATA"]["SUBTYPE"] = BPFunction::SubTypeToString(subType);
	THISPATHO(layer, objIndex, bpIndex)["DATA"]["POSITION"] = Util::VectorToString(object2D->GetPosition() - object2D->GetOffset());
}

void Blueprint::Save(json & data, int objIndex, int bpIndex)
{
	THISPATHU(objIndex, bpIndex)["DATA"]["ID"] = id;
	THISPATHU(objIndex, bpIndex)["DATA"]["TYPE"] = BPFunction::TypeToString(type);
	THISPATHU(objIndex, bpIndex)["DATA"]["SUBTYPE"] = BPFunction::SubTypeToString(subType);
	THISPATHU(objIndex, bpIndex)["DATA"]["POSITION"] = Util::VectorToString(object2D->GetPosition() - object2D->GetOffset());
}

void Blueprint::Save(json & data, int bpIndex)
{
	THISPATHE(bpIndex)["DATA"]["ID"] = id;
	THISPATHE(bpIndex)["DATA"]["TYPE"] = BPFunction::TypeToString(type);
	THISPATHE(bpIndex)["DATA"]["SUBTYPE"] = BPFunction::SubTypeToString(subType);
	THISPATHE(bpIndex)["DATA"]["POSITION"] = Util::VectorToString(object2D->GetPosition() - object2D->GetOffset());
}

float Blueprint::GetColorAvg()
{
	D3DXVECTOR4 color = object2D->GetColor();
	return (color.x + color.y + color.z) / 3.0f;
}

void Blueprint::SetSubType(BlueprintSubType type, int dataSize)
{
	subType = type;
	subData.clear();
	subData.resize(dataSize);
}