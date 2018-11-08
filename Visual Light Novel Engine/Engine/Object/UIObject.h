#pragma once
#include "../../Core/Components/GameObject.h"
#include "../../Core/json.hpp"

using namespace nlohmann;

enum class UIObjectActionType
{
	NONE,
	CHANGE_SCENE,
	CUSTOM_FUNCTION,
	BLUEPRINT
};

enum class UIObjectType
{
	SINGLE,
	LISTV,
	LISTH
};

struct UIObjectAction
{
	UIObjectActionType Type;
	string ChangeSceneName;
	string ChangeSceneWindow;
	function<void()> CustomFunction;
};

struct UIObjectList
{
	wstring Text;
	UIObjectAction Action;
};

class Object2D;

class UIObject : public GameObject
{
public:
	UIObject(UIObjectType type, string window, string defaultImage, string overImage = "", string pressImage = "");
	~UIObject();

	void SetAction(UIObjectAction data) { actionData = data; }
	void SetList(vector<UIObjectList> data) { listData = data; }
	void ChangeImage(string defaultImage, string overImage = "", string pressImage = "");
	Object2D * GetObject2D() { return object2D; }

	virtual void Update() override;
	virtual void Render() override;
	void Render(int maxList);

	virtual void ChangeShader(string shaderPath) override;

	void SetUIObjectColor(D3DXVECTOR4 color) { UIObjectColor = color; }
	D3DXVECTOR4 GetUIObjectColor() { return UIObjectColor; }

	virtual void SetActive(bool active) override;

	virtual bool IsOver() override;
	virtual Texture2D * GetTexture2D() override;

	virtual void SetAnchor(VertexAnchor anchor) override;

	void Save(json &data, int index);
	void Load(json& data);

private:
	void Detect(int actionIndex);
	void Action(UIObjectAction data);

private:
	bool isOver;
	UIObjectType type;
	string texturePath[3];
	D3DXVECTOR4 UIObjectColor;
	UIObjectAction actionData;
	vector<UIObjectList> listData;
	Object2D * object2D;
};

