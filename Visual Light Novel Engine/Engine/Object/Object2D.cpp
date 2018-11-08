#include "../../stdafx.h"
#include "Object2D.h"
#include "../../Core/Util.h"
#include "../../Engine/Blueprint/Blueprint.h"


Object2D::Object2D(string window, string texturePath, string shaderPath)
	: texture2D(nullptr)
	, shader(nullptr)
{
	texture2D = RESOURCES->GetResource<Texture2D>(texturePath, window);
	shader = RESOURCES->GetResource<Shader>(shaderPath, window);
	this->window = window;
	context = GRAPHICS->GetDeviceContext(window);

	this->texturePath = texturePath;
	this->shaderPath = shaderPath;

	CreateBuffers();
}


Object2D::~Object2D()
{
	SAFE_DELETE(shader);
}

void Object2D::Load(json & data)
{
	string anchor = data.value("ANCHOR", "");
	string color = data.value("COLOR", "");
	string pos = data.value("POSITION", "");
	string rot = data.value("ROTATION", "");
	string scale = data.value("SCALE", "");

	VertexAnchor ac;
	if (anchor == "C") ac = VertexAnchor::CENTER;
	if (anchor == "LT") ac = VertexAnchor::LEFT_TOP;
	if (anchor == "RT") ac = VertexAnchor::RIGHT_TOP;
	if (anchor == "LB") ac = VertexAnchor::LEFT_BOTTOM;
	if (anchor == "RB") ac = VertexAnchor::RIGHT_BOTTOM;
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
	SetAnchor(anchor);
}

void Object2D::ChangeShader(string shaderPath)
{
	SAFE_DELETE(shader);
	shader = RESOURCES->GetResource<Shader>(shaderPath, window);
}

void Object2D::Save(json & data, int layer, int index)
{
#define THISPATH data["LAYER"][to_string(layer)]["OBJECT"][index]

	string anchorText;
	if (anchor == VertexAnchor::LEFT_TOP) anchorText = "LT";
	if (anchor == VertexAnchor::RIGHT_TOP) anchorText = "RT";
	if (anchor == VertexAnchor::LEFT_BOTTOM) anchorText = "LB";
	if (anchor == VertexAnchor::RIGHT_BOTTOM) anchorText = "RB";
	if (anchor == VertexAnchor::CENTER) anchorText = "C";

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