#pragma once
#include "../../Core/Components/GameObject.h"
#include "../../Core/json.hpp"

using namespace nlohmann;

class Object2D : public GameObject
{
public:
	Object2D(string window, string texturePath, string shaderPath = "Shaders/Texture2D.hlsl");
	virtual ~Object2D();

	virtual void Update() override;
	virtual void Render() override;

	virtual void ChangeTexture(string texturePath) override;
	virtual void ChangeShader(string shaderPath) override;

	virtual Texture2D * GetTexture2D() override { return texture2D; }

	void Save(json &data, int layer, int index);
	void Load(json& data);

private:
	Texture2D * texture2D;
	Shader * shader;

	string texturePath;
	string shaderPath;
};

