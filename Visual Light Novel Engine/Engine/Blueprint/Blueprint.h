#pragma once
#include "../../Core/Components/GameObject.h"
#include "../../Core/Components/Sequence.h"
#include "../json.hpp"

using namespace nlohmann;

class Blueprint;

enum class BlueprintType
{
	BEGIN,
	OBJECT,
	FLOAT,
	SEQUENCE
};

enum class BlueprintSubType
{
	NONE,
	SEQUENCE_QUEUE_EASEIN,
	SEQUENCE_QUEUE_EASEOUT,
	SEQUENCE_QUEUE_EASEINOUT,
	SEQUENCE_QUEUE_LINEAR,
	SEQUENCE_QUEUE_BEZIER2,
	SEQUENCE_QUEUE_BEZIER3
};

enum class BlueprintObjectType
{
	IMAGE,
	UI
};

struct BlueprintObject
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 Scale;
	D3DXVECTOR4 Color;
};

struct BlueprintSubData
{
	void * Data;
	BlueprintType Type;
};

struct BlueprintNode
{
	Blueprint * data;
	BlueprintNode * next;
	BlueprintNode * prev;
};

struct BlueprintLinkData
{
	UINT data;
	vector<UINT> subdata;
	UINT next;
	UINT prev;
};

class Blueprint
{
public:
	Blueprint();
	virtual ~Blueprint();
	bool Update();
	void Render();

	virtual void Load(json& data) {};

	void SetID(UINT id) { this->id = id; }
	UINT GetID() { return id; }
	
	virtual void Execute(void * data = nullptr) {};

	Object2D * GetObject2D() { return object2D; }
	BlueprintType GetType() { return type; }
	float GetColorAvg() { return (color.x + color.y + color.z) / 3.0f; }

	BlueprintSubType GetSubType() { return subType; }
	virtual void SetSubType(BlueprintSubType type, int dataSize);

	vector<BlueprintSubData>& GetSubData() { return subData; }

protected:
	virtual void SubUpdate() {};
	virtual void SubRender() {};

	UINT id;

	Object2D * object2D;
	D3DXVECTOR4 color;
	BlueprintType type;
	BlueprintSubType subType;

	vector<BlueprintSubData> subData;
};

class BlueprintList
{
public:
	BlueprintList(BlueprintObjectType type);
	~BlueprintList();

	Blueprint * Add(BlueprintType type);
	Blueprint * Add(Blueprint * bp);
	void Delete(BlueprintNode * node);

	void Update();
	void Render();
	void Save(json &data, int layer, int index);
	void Clear();

	BlueprintObjectType GetType() { return type; }
	BlueprintNode * GetSelectNode() { return selectNode; }
	BlueprintNode * GetClickNode();

	void SetOffset(D3DXVECTOR3 offset) { this->offset = offset; }
	D3DXVECTOR3 GetOffset() { return offset; }
	
	void Excute();
	void ExcuteUpdate(GameObject * gameobject);

	void Link(vector<BlueprintLinkData> linkData);

private:
	void CreateBuffer();
	void Link();
	void SetData();
	void LineRender(Blueprint * from, Blueprint * to, wstring comment = L"");
	void MouseDrag();
	BlueprintNode * FindNode(UINT id);

	VertexBuffer * vertexBuffer;
	Shader * shader;

	float elapse;

	// 블루프린트
	string inputData;
	BlueprintObjectType type;
	BlueprintNode * selectNode;

	Sequence * seq;

	// 드래그
	D3DXVECTOR3 offset;
	D3DXVECTOR3 startOffset;
	D3DXVECTOR3 startMousePos;

private:
	vector<BlueprintNode*> nodeList;
};