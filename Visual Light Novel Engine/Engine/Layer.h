#pragma once
#include "../Core/Components/GameObject.h"
#include "../Core/json.hpp"

using namespace nlohmann;

struct LayerObject
{
	int layer;
	GameObject * object;
	LayerObject() { layer = 0; object = nullptr; }
	LayerObject(int layer, GameObject * object)
	{
		this->layer = layer;
		this->object = object;
	}
};

class ContentObject;
class NameObject;
class BackgroundObject;

class Layer
{
public:
	Layer();
	~Layer();

	void AddObject(int layer, GameObject * object);
	void DeleteObject(GameObject * object);
	void ChangeLayer(int newLayer, LayerObject * object);

	LayerObject GetFront();
	map<int, vector<GameObject*>, greater<int>> * GetLayers() { return &layers; }

	void Update();
	void Render();
	void Start();

	void CreateContent();
	void CreateName();
	void CreateBackground();

	void Save(json& data);

private:
	map<int, vector<GameObject*>, greater<int>> layers;
	ContentObject * contentObject;
	NameObject * nameObject;
	BackgroundObject * bgObject;
};

