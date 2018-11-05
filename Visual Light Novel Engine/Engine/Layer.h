#pragma once
#include "../Core/Components/GameObject.h"

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
	void Excute();

private:
	map<int, vector<GameObject*>, greater<int>> layers;
};

