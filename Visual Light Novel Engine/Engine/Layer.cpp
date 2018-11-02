#include "../stdafx.h"
#include "Layer.h"
#include "Blueprint\Blueprint.h"


Layer::Layer()
{
}


Layer::~Layer()
{
	for (auto layer : layers)
		for (auto object : layer.second)
			SAFE_DELETE(object);
}

void Layer::AddObject(int layer, Object2D * object)
{
	layers[layer].push_back(object);
}

void Layer::DeleteObject(Object2D * object)
{
	for (auto& layer : layers)
	{
		for (int i = 0; i < layer.second.size(); i++)
		{
			if (object == layer.second[i])
			{
				SAFE_DELETE(layer.second[i]);
				layer.second.erase(layer.second.begin() + i);
				return;
			}
		}
	}
}

void Layer::ChangeLayer(int newLayer, LayerObject * object)
{
	for (auto& layer : layers)
	{
		for (int i = 0; i < layer.second.size(); i++)
		{
			if (object->object == layer.second[i])
			{
				layer.second.erase(layer.second.begin() + i);
				layers[newLayer].push_back(object->object);
				object->layer = newLayer;
				return;
			}
		}
	}
}

LayerObject Layer::GetFront()
{
	for (auto layer = layers.rbegin(); layer != layers.rend(); layer++)
	{
		for (int i = layer->second.size() - 1; i >= 0; i--)
		{
			if (layer->second[i]->IsOver())
				return LayerObject(layer->first, layer->second[i]);
		}
	}
	return LayerObject(0, nullptr);
}

void Layer::Update()
{
	for (auto layer : layers)
	{
		for (auto object : layer.second)
		{
			object->Update();
			object->GetBlueprint()->ExcuteUpdate(object);
		}
	}
}

void Layer::Render()
{
	for (auto layer : layers)
	{
		for (auto object : layer.second)
		{
			object->Render();
		}
	}
}

void Layer::Excute()
{
	/*for (auto layer : layers)
	{
		for (auto object : layer.second)
		{
			object->GetBlueprint()->Excute();
		}
	}*/
}
