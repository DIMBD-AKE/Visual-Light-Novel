#pragma once
#include "../Components/GameObject.h"
#include "../../Engine/json.hpp"

using namespace nlohmann;

enum class ImageAnchor
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

class Object2D : public GameObject
{
public:
	Object2D(string window, string texturePath, string shaderPath = "Shaders/Texture2D.hlsl");
	virtual ~Object2D();

	void Load(json& data);
	virtual void Update() override;
	virtual void Render() override;

	void ChangeTexture(string texturePath);
	void ChangeShader(string shaderPath);
	string GetTexturePath() { return texturePath; }
	string GetShaderPath() { return shaderPath; }
	Texture2D * GetTexture2D() { return texture2D; }

	void SetAnchor(ImageAnchor anchor);
	ImageAnchor GetAnchor() { return anchor; }

	void Save(json &data, int layer, int index);
	bool IsOver();

private:
	void CreateBuffers();
	void RenderBuffers();
	void UpdateRect(ImageAnchor anchor);
	bool PtInRectRot(RECT rc, float angle, POINT pt);

private:
	VertexBuffer * vertexBuffer;
	IndexBuffer * indexBuffer;

	Texture2D * texture2D;
	Shader * shader;
	FRECT vertexRect;

	string texturePath;
	string shaderPath;
	ImageAnchor anchor;
};

