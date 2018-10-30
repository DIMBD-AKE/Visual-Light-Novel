#pragma once

class IScene
{
public:
	IScene() {}
	virtual ~IScene() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool Release() = 0;
};

class Scene
{
	SINGLETON(Scene)

public:
	void Update();
	void Render();
	void Destroy();

	bool AddScene(IScene * scene, string name);
	bool DeleteScene(string name);
	bool ReleaseScene(string name);
	bool ChangeScene(string name, string window, bool keep = true);

	IScene * GetScene(string name);
	IScene * GetCurrentScene(string window);
	string GetCurrentSceneName(string window);

private:
	map<string, IScene*> scenes;
	map<string, IScene*> currentScene;
	map<string, string> currentSceneName;
};

#define SCENE Scene::GetInstance()