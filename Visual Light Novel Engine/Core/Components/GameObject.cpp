#include "../../stdafx.h"
#include "GameObject.h"
#include "../../Engine/Blueprint/Blueprint.h"

GameObject::GameObject()
	: isDrag(false)
	, bpList(nullptr)
	, context(nullptr)
	, indexBuffer(nullptr)
	, vertexBuffer(nullptr)
	, color(1, 1, 1, 1)
{
}

GameObject::~GameObject()
{
	SAFE_DELETE(bpList);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

bool GameObject::IsOver()
{
	RECT rc;
	rc.left = position.x + offset.x + vertexRect.left * scale.x;
	rc.right = position.x + offset.x + vertexRect.right * scale.x;
	rc.top = position.y + offset.y + -vertexRect.top * scale.y;
	rc.bottom = position.y + offset.y + -vertexRect.bottom * scale.y;

	if (PtInRect(&rc, INPUT->GetMousePos(window)) && IsFocus(window))
		return true;

	return false;
}

void GameObject::SetAnchor(VertexAnchor anchor)
{
	vector<VertexTexture> vertices(6);

	UpdateRect(anchor);

	vertices[0].position = D3DXVECTOR3(vertexRect.left, vertexRect.bottom, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(vertexRect.left, vertexRect.top, 0.0f);
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(vertexRect.right, vertexRect.top, 0.0f);
	vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(vertexRect.left, vertexRect.bottom, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(vertexRect.right, vertexRect.top, 0.0f);
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(vertexRect.right, vertexRect.bottom, 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertexBuffer->Mapped<VertexTexture>(vertices);
}

void GameObject::Drag()
{
	if (!isDrag) return;

	auto point = INPUT->GetMousePos(window);
	D3DXVECTOR3 pos = D3DXVECTOR3(point.x, point.y, 0);

	if (INPUT->GetKey(VK_LBUTTON))
	{
		D3DXVECTOR3 movePos = pos - startMousePos;
		position = startPostion + movePos;
	}

	if (INPUT->GetKeyUp(VK_LBUTTON))
		isDrag = false;
}

void GameObject::InitDrag()
{
	if (!IsFocus(window)) return;
	isDrag = true;
	auto point = INPUT->GetMousePos(window);
	startMousePos = D3DXVECTOR3(point.x, point.y, 0);;
	RECT rect;
	GetWindowRect(SETTING->GetWindow(window)->WindowHandle, &rect);
	startPostion = position;
}

void GameObject::CreateBlueprint(BlueprintObjectType type)
{
	bpList = new BlueprintList(type);
}

void GameObject::CreateBuffers()
{
	vector<VertexTexture> vertices(6);
	vector<ULONG> indices(6);

	vertexBuffer = new VertexBuffer();
	indexBuffer = new IndexBuffer();

	UpdateRect(VertexAnchor::CENTER);

	vertices[0].position = D3DXVECTOR3(vertexRect.left, vertexRect.bottom, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(vertexRect.left, vertexRect.top, 0.0f);
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(vertexRect.right, vertexRect.top, 0.0f);
	vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(vertexRect.left, vertexRect.bottom, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(vertexRect.right, vertexRect.top, 0.0f);
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(vertexRect.right, vertexRect.bottom, 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertexBuffer->Create<VertexTexture>(vertices, window);

	for (int i = 0; i < indices.size(); i++)
		indices[i] = i;

	indexBuffer->Create(indices, window);
}

void GameObject::RenderBuffers()
{
	CAMERA->Render();
	vertexBuffer->IASet();
	indexBuffer->IASet();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GameObject::UpdateRect(VertexAnchor anchor)
{
	if (anchor == VertexAnchor::LEFT_TOP)
	{
		vertexRect.left = 0;
		vertexRect.top = 0;
		vertexRect.right = GetTexture2D()->GetDesc().Width;
		vertexRect.bottom = GetTexture2D()->GetDesc().Height * -1.0f;
	}
	if (anchor == VertexAnchor::RIGHT_TOP)
	{
		vertexRect.left = GetTexture2D()->GetDesc().Width * -1.0f;
		vertexRect.top = 0;
		vertexRect.right = 0;
		vertexRect.bottom = GetTexture2D()->GetDesc().Height * -1.0f;
	}
	if (anchor == VertexAnchor::LEFT_BOTTOM)
	{
		vertexRect.left = 0;
		vertexRect.top = GetTexture2D()->GetDesc().Height;
		vertexRect.right = GetTexture2D()->GetDesc().Width;
		vertexRect.bottom = 0;
	}
	if (anchor == VertexAnchor::RIGHT_BOTTOM)
	{
		vertexRect.left = GetTexture2D()->GetDesc().Width * -1.0f;
		vertexRect.top = GetTexture2D()->GetDesc().Height;
		vertexRect.right = 0;
		vertexRect.bottom = 0;
	}
	if (anchor == VertexAnchor::CENTER)
	{
		vertexRect.left = GetTexture2D()->GetDesc().Width / -2.0f;
		vertexRect.top = GetTexture2D()->GetDesc().Height / 2.0f;
		vertexRect.right = GetTexture2D()->GetDesc().Width / 2.0f;
		vertexRect.bottom = GetTexture2D()->GetDesc().Height / -2.0f;
	}
	this->anchor = anchor;
}
