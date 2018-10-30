#pragma once
#include "Transform.h"

class BlueprintList;
enum class BlueprintObjectType;

class GameObject : public Transform
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update() {}
	virtual void Render() = 0;

	void Drag();
	void InitDrag();
	void CancelDrag() { isDrag = false; }

	bool IsDrag() { return isDrag; }

	D3DXVECTOR4 GetColor() { return color; }
	void SetColor(D3DXVECTOR4 c) { color = c; }

	bool GetActive() { return isActive; }
	void SetActive(bool active) { isActive = active; }

	string GetWindow() { return window; }

	void CreateBlueprint(BlueprintObjectType type);
	BlueprintList * GetBlueprint() { return bpList; }

protected:
	D3DXVECTOR4 color;
	bool isActive;
	string window;
	BlueprintList * bpList;

private:
	// Drag
	bool isDrag;
	D3DXVECTOR3 startPostion;
	D3DXVECTOR3 startMousePos;
};

