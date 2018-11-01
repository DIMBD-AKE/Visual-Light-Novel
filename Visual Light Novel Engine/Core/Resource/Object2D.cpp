#include "../../stdafx.h"
#include "Object2D.h"
#include "../Util.h"
#include "../../Engine/Blueprint/Blueprint.h"


Object2D::Object2D(string window, string texturePath, string shaderPath)
	: vertexBuffer(nullptr)
	, indexBuffer(nullptr)
	, texture2D(nullptr)
	, shader(nullptr)
{
	texture2D = RESOURCES->GetResource<Texture2D>(texturePath, window);
	shader = RESOURCES->GetResource<Shader>(shaderPath, window);
	this->window = window;
	this->texturePath = texturePath;
	this->shaderPath = shaderPath;

	CreateBuffers();
}


Object2D::~Object2D()
{
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(shader);
}

void Object2D::Load(json & data)
{
	string anchor = data.value("ANCHOR", "");
	string color = data.value("COLOR", "");
	string pos = data.value("POSITION", "");
	string rot = data.value("ROTATION", "");
	string scale = data.value("SCALE", "");

	ImageAnchor ac;
	if (anchor == "C") ac = ImageAnchor::CENTER;
	if (anchor == "LT") ac = ImageAnchor::LEFT_TOP;
	if (anchor == "RT") ac = ImageAnchor::RIGHT_TOP;
	if (anchor == "LB") ac = ImageAnchor::LEFT_BOTTOM;
	if (anchor == "RB") ac = ImageAnchor::RIGHT_BOTTOM;
	SetAnchor(ac);

	position = Util::StringToVector3(pos);
	rotation = Util::StringToVector3(rot);
	this->scale = Util::StringToVector3(scale);
	this->color = Util::StringToVector4(color);
}

void Object2D::Update()
{
	Drag();
}

void Object2D::Render()
{
	CAMERA->Render();
	RenderBuffers();

	shader->RenderShader
	(
		6,
		this,
		color,
		D3DXVECTOR4(texture2D->GetDesc().Width, texture2D->GetDesc().Height, scale.x, scale.y),
		texture2D->GetSRV(),
		window
	);
}

void Object2D::ChangeTexture(string texturePath)
{
	texture2D = RESOURCES->GetResource<Texture2D>(texturePath, window);
	this->texturePath = texturePath;
}

void Object2D::ChangeShader(string shaderPath)
{
	SAFE_DELETE(shader);
	shader = RESOURCES->GetResource<Shader>(shaderPath, window);
	this->shaderPath = shaderPath;
}

void Object2D::SetAnchor(ImageAnchor anchor)
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

void Object2D::Save(json & data, int layer, int index)
{
#define THISPATH data["LAYER"][to_string(layer)]["OBJECT"][index]

	string anchorText;
	if (anchor == ImageAnchor::LEFT_TOP) anchorText = "LT";
	if (anchor == ImageAnchor::RIGHT_TOP) anchorText = "RT";
	if (anchor == ImageAnchor::LEFT_BOTTOM) anchorText = "LB";
	if (anchor == ImageAnchor::RIGHT_BOTTOM) anchorText = "RB";
	if (anchor == ImageAnchor::CENTER) anchorText = "C";

	// Shader, Texture, Transform
	THISPATH["TEXTURE"] = texturePath;
	THISPATH["SHADER"] = "Shaders/Texture2D.hlsl";
	THISPATH["ANCHOR"] = anchorText;
	THISPATH["POSITION"] = Util::VectorToString(position);
	THISPATH["ROTATION"] = Util::VectorToString(rotation);
	THISPATH["SCALE"] = Util::VectorToString(scale);
	THISPATH["COLOR"] = Util::VectorToString(color);

	// Blueprint
	bpList->Save(data, layer, index);
}

bool Object2D::IsOver()
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

void Object2D::CreateBuffers()
{
	vector<VertexTexture> vertices(6);
	vector<ULONG> indices(6);

	vertexBuffer = new VertexBuffer();
	indexBuffer = new IndexBuffer();

	UpdateRect(ImageAnchor::CENTER);

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

void Object2D::RenderBuffers()
{
	vertexBuffer->IASet();
	indexBuffer->IASet();
	GRAPHICS->GetDeviceContext(window)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object2D::UpdateRect(ImageAnchor anchor)
{
	if (anchor == ImageAnchor::LEFT_TOP)
	{
		vertexRect.left = 0;
		vertexRect.top = 0;
		vertexRect.right = texture2D->GetDesc().Width;
		vertexRect.bottom = texture2D->GetDesc().Height * -1.0f;
	}
	if (anchor == ImageAnchor::RIGHT_TOP)
	{
		vertexRect.left = texture2D->GetDesc().Width * -1.0f;
		vertexRect.top = 0;
		vertexRect.right = 0;
		vertexRect.bottom = texture2D->GetDesc().Height * -1.0f;
	}
	if (anchor == ImageAnchor::LEFT_BOTTOM)
	{
		vertexRect.left = 0;
		vertexRect.top = texture2D->GetDesc().Height;
		vertexRect.right = texture2D->GetDesc().Width;
		vertexRect.bottom = 0;
	}
	if (anchor == ImageAnchor::RIGHT_BOTTOM)
	{
		vertexRect.left = texture2D->GetDesc().Width * -1.0f;
		vertexRect.top = texture2D->GetDesc().Height;
		vertexRect.right = 0;
		vertexRect.bottom = 0;
	}
	if (anchor == ImageAnchor::CENTER)
	{
		vertexRect.left = texture2D->GetDesc().Width / -2.0f;
		vertexRect.top = texture2D->GetDesc().Height / 2.0f;
		vertexRect.right = texture2D->GetDesc().Width / 2.0f;
		vertexRect.bottom = texture2D->GetDesc().Height / -2.0f;
	}
	this->anchor = anchor;
}

bool Object2D::PtInRectRot(RECT rc, float angle, POINT pt)
{
	return false;
}
