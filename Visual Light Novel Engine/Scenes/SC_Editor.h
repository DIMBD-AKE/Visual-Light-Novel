#pragma once
#include "../Engine/Layer.h"

class UIObject;
class Object2D;
class ElementObject;

enum class GameScene
{
	TITLE,
	GAME,
	OPTION,
	CG,
	SELECT
};

class SC_Editor : public IScene
{
public:
	SC_Editor();
	~SC_Editor();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;

	GameObject * GetSelectObject() { return selectObject.object; }
	GameObject ** GetSelectObjectAddress() { return &selectObject.object; }
	Layer * GetLayer() { return objectLayer; }
	GameScene GetScene() { return currentScene; }

	void SetSceneLayer(map<GameScene, Layer*> sceneLayer);
	map<GameScene, Layer*>& GetSceneLayer() { return sceneLayer; }

private:
	enum class EditType
	{
		NONE,
		POSITION,
		ROTATION,
		SCALE,
	};

	bool SelectEditType();
	void ApplyEdit();

	void ChangeAnchor(VertexAnchor anchor) { selectObject.object->SetAnchor(anchor); }
	void ChangeLayer(int layer) { objectLayer->ChangeLayer(selectObject.layer + layer, &selectObject); }
	void ChangeColor();

	void InfoUpdate();
	void InfoRender();

	void HierarchyUpdate();

	void ChangeScene(GameScene scene);

	EditType editType;
	D3DXVECTOR2 startMouse;

private:
	Layer * objectLayer;
	LayerObject selectObject;
	Object2D * editCircle;

	map<GameScene, Layer*> sceneLayer;
	GameScene currentScene;

	// Scene
	UIObject * sceneUIObject;

	// Info
	UIObject * anchorUIObject[5] = { nullptr, };
	UIObject * layerUIObject[2] = { nullptr, };
	UIObject * colorUIObject = nullptr;
};

