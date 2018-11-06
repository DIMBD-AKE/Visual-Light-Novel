#pragma once
#include "../../Core/Components/GameObject.h"
#include "../../Core/json.hpp"

using namespace nlohmann;

enum class ElementType
{
	NONE,
	BACKGROUND,
	CHARACTER,
	STUFF
};

class IElement
{
public:
	ElementType Type;
	multimap<wstring, string> StateImage;

	static string TypeToString(ElementType type)
	{
		if (type == ElementType::BACKGROUND) return "BACKGROUND";
		if (type == ElementType::CHARACTER) return "CHARACTER";
		if (type == ElementType::STUFF) return "STUFF";
		return "";
	}

	static ElementType StringToType(string s)
	{
		if (s.compare("BACKGROUND") == 0) return ElementType::BACKGROUND;
		if (s.compare("CHARACTER") == 0) return ElementType::CHARACTER;
		if (s.compare("STUFF") == 0) return ElementType::STUFF;
		return ElementType::NONE;
	}
};

struct Background : public IElement
{
	Background() {
		Type = ElementType::BACKGROUND;
	}
};

struct Character : public IElement
{
	wstring Name;
	vector<wstring> Explain;

	Character() {
		Type = ElementType::CHARACTER;
	}
};

struct Stuff : public IElement
{
	wstring Name;
	vector<wstring> Explain;

	Stuff() {
		Type = ElementType::STUFF;
	}
};

class ElementObject : public GameObject
{
public:
	ElementObject(string window, string elementPath, string shaderPath = "Shaders/Texture2D.hlsl");
	virtual ~ElementObject();

	virtual void Update() override;
	virtual void Render() override;
	virtual Texture2D * GetTexture2D() override { return textures.begin()->second; }

	void SetState(wstring state);
	IElement * GetElement() { return element; }

	virtual bool IsOver() override;

	void Save();
	void Load(json& data);

private:
	multimap<wstring, Texture2D*> textures;
	Shader * shader;
	IElement * element;
	wstring stateName;

	string elementPath;
	string shaderPath;
};