#pragma once
#include "Transform.h"

class BlueprintList;
enum class BlueprintObjectType;

enum class VertexAnchor
{
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	CENTER
};

struct FRECT
{
	float left, right, top, bottom;
};

class GameObject : public Transform
{
public:
	GameObject();
	virtual ~GameObject();

	// Essential function
	virtual void		Update() = 0;
	virtual void		Render() = 0;
	virtual bool		IsOver();
	virtual void		ChangeTexture(string texturePath) {}
	virtual void		ChangeShader(string shaderPath) {}
	virtual Texture2D *	GetTexture2D() = 0;

	// Drag
	void Drag();
	void InitDrag();
	void CancelDrag() { isDrag = false; }
	bool IsDrag() { return isDrag; }

	// Get, Set
	void			SetAnchor(VertexAnchor anchor);
	VertexAnchor	GetAnchor() { return anchor; }

	D3DXVECTOR4		GetColor() { return color; }
	void			SetColor(D3DXVECTOR4 c) { color = c; }

	bool			GetActive() { return isActive; }
	virtual void	SetActive(bool active) { isActive = active; }

	string			GetWindow() { return window; }

	void			CreateBlueprint(BlueprintObjectType type);
	BlueprintList *	GetBlueprint() { return bpList; }

protected:
	void CreateBuffers();
	void RenderBuffers();
	void UpdateRect(VertexAnchor anchor);

protected:
	// Temp
	string					window;
	ID3D11DeviceContext *	context;

	// Object
	bool					isActive;
	D3DXVECTOR4				color;
	BlueprintList *			bpList;

	// Vertex
	VertexAnchor			anchor;
	FRECT					vertexRect;

	// Buffer
	VertexBuffer *			vertexBuffer;
	IndexBuffer *			indexBuffer;

private:
	// Drag
	bool			isDrag;
	D3DXVECTOR3		startPostion;
	D3DXVECTOR3		startMousePos;
};

