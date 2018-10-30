#pragma once
#include "../Core/Components/GameObject.h"

enum class UIActionType
{
	NONE,
	CHANGE_SCENE,
	CUSTOM_FUNCTION
};

enum class UIType
{
	SINGLE,
	LISTV,
	LISTH
};

struct UIAction
{
	UIActionType Type;
	string ChangeSceneName;
	string ChangeSceneWindow;
	function<void()> CustomFunction;
};

struct UIList
{
	wstring Text;
	UIAction Action;
};

class UI : public GameObject
{
public:
	UI(UIType type, string window, string defaultImage, string overImage = "", string pressImage = "");
	~UI();

	void SetAction(UIAction data) { actionData = data; }
	void SetList(vector<UIList> data) { listData = data; }
	void ChangeImage(string defaultImage, string overImage = "", string pressImage = "");
	Object2D * GetObject2D() { return object2D; }

	virtual void Update() override;
	virtual void Render() override;
	void Render(int maxList);

	void SetUIColor(D3DXVECTOR4 color) { uiColor = color; }
	D3DXVECTOR4 GetUIColor() { return uiColor; }

	void SetActive(bool active);
	bool IsActive() { return isActive; }
	bool IsOver();

private:
	void Action(UIAction data);

private:
	bool isActive;
	bool isOver;
	UIType type;
	string window;
	string texturePath[3];
	D3DXVECTOR4 uiColor;
	UIAction actionData;
	vector<UIList> listData;
	Object2D * object2D;
};

