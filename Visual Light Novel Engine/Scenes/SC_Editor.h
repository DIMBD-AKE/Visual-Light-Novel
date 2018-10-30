#pragma once
#include "../Engine/Layer.h"

class UI;

enum class GameScene
{
	TITLE,
	GAME,
	OPTION,
	CG,
	SELECT,
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

	Object2D * GetSelectObject() { return selectObject.object; }
	Object2D ** GetSelectObjectAddress() { return &selectObject.object; }
	Layer * GetLayer() { return objectLayer; }

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

	void ChangeAnchor(ImageAnchor anchor) { selectObject.object->SetAnchor(anchor); }
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

	// Scene
	UI * sceneUI;

	// Info
	UI * anchorUI[5] = { nullptr, };
	UI * layerUI[2] = { nullptr, };
	UI * colorUI = nullptr;
};

