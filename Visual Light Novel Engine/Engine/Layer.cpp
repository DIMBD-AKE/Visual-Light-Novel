#include "../stdafx.h"
#include "Layer.h"
#include "Blueprint\Blueprint.h"
#include "Object\Object.h"


Layer::Layer()
	: contentObject(nullptr)
	, nameObject(nullptr)
	, bgObject(nullptr)
{
}


Layer::~Layer()
{
	for (auto layer : layers)
		for (auto object : layer.second)
			SAFE_DELETE(object);
	SAFE_DELETE(contentObject);
	SAFE_DELETE(nameObject);
	SAFE_DELETE(bgObject);
}

void Layer::AddObject(int layer, GameObject * object)
{
	layers[layer].push_back(object);
}

void Layer::DeleteObject(GameObject * object)
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
			if (object->GetBlueprint())
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

void Layer::Start()
{
	for (auto layer : layers)
	{
		for (auto object : layer.second)
		{
			if (object->GetBlueprint())
				object->GetBlueprint()->Excute("START");
		}
	}
}

void Layer::CreateContent()
{
	SAFE_DELETE(contentObject);
	contentObject = new ContentObject();
}

void Layer::CreateName()
{
	SAFE_DELETE(nameObject);
	nameObject = new NameObject();
}

void Layer::CreateBackground()
{
	SAFE_DELETE(bgObject);
	bgObject = new BackgroundObject();
}

void Layer::Save(json & data)
{
	for (auto layer : layers)
	{
		for (int i = 0; i < layer.second.size(); i++)
		{
			if (dynamic_cast<Object2D*>(layer.second[i]))
				dynamic_cast<Object2D*>(layer.second[i])->Save(data, layer.first, i);
			if (dynamic_cast<ElementObject*>(layer.second[i]))
				dynamic_cast<ElementObject*>(layer.second[i])->Save();
			if (dynamic_cast<UIObject*>(layer.second[i]))
				dynamic_cast<UIObject*>(layer.second[i])->Save(data, i);
		}
	}
	if (contentObject)
		contentObject->Save(data);
	if (nameObject)
		nameObject->Save(data);
	if (bgObject)
		bgObject->Save(data);
}
