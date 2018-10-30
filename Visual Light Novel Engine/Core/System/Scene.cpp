#include "../../stdafx.h"
#include "Scene.h"


Scene::Scene()
{

}

Scene::~Scene()
{
	
}

void Scene::Update()
{
	for (auto scene : currentScene)
	{
		if (scene.second)
			scene.second->Update();
	}
}

void Scene::Render()
{
	for (auto scene : currentScene)
	{
		if (scene.second)
			scene.second->Render();
	}
}

void Scene::Destroy()
{
	for (auto scene : scenes)
	{
		scene.second->Release();
		SAFE_DELETE(scene.second);
	}
}

bool Scene::AddScene(IScene * scene, string name)
{
	if (scenes.find(name) != scenes.end())
		return false;

	scene->Init();
	scenes.insert(make_pair(name, scene));
	return true;
}

bool Scene::DeleteScene(string name)
{
	if (scenes.find(name) == scenes.end())
		return false;

	if (scenes[name]->Release())
	{
		SAFE_DELETE(scenes[name]);
		scenes.erase(name);
		return true;
	}

	return false;
}

bool Scene::ReleaseScene(string name)
{
	if (scenes.find(name) == scenes.end())
		return false;

	return scenes[name]->Release();
}

bool Scene::ChangeScene(string name, string window, bool keep)
{
	if (scenes.find(name) == scenes.end())
		return false;

	if (!keep)
	{
		if (currentScene[window])
			SAFE_RELEASE(currentScene[window]);
		currentScene[window] = scenes[name];
		currentScene[window]->Init();
		currentSceneName[window] = name;
	}
	else
	{
		currentScene[window] = scenes[name];
		currentSceneName[window] = name;
	}

	return true;
}

IScene * Scene::GetScene(string name)
{
	if (scenes.find(name) == scenes.end())
		return nullptr;
	return scenes[name];
}

IScene * Scene::GetCurrentScene(string window)
{
	if (currentSceneName.find(window) == currentSceneName.end())
		return nullptr;
	return currentScene[window];
}

string Scene::GetCurrentSceneName(string window)
{
	if (currentSceneName.find(window) == currentSceneName.end())
		return "";
	return currentSceneName[window];
}
