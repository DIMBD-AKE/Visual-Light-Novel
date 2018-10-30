#pragma once
#include "../Core/Components/GameObject.h"

struct LayerObject
{
	int layer;
	Object2D * object;
	LayerObject() { layer = 0; object = nullptr; }
	LayerObject(int layer, Object2D * object)
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

	void AddObject(int layer, Object2D * object);
	void DeleteObject(Object2D * object);
	void ChangeLayer(int newLayer, LayerObject * object);

	LayerObject GetFront();
	map<int, vector<Object2D*>, greater<int>> * GetLayers() { return &layers; }

	void Update();
	void Render();
	void Excute();

private:
	map<int, vector<Object2D*>, greater<int>> layers;
};

