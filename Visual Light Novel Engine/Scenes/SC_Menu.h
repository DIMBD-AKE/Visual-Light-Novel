#pragma once

class UI;
class Layer;
enum class GameScene;

class SC_Menu : public IScene
{
public:
	SC_Menu();
	virtual ~SC_Menu();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;

private:
	void ProjectSave();
	void ProjectLoad();
	void SceneLoad(string path, map<GameScene, Layer*>& sceneLayer);

	UI * sideUI;
};

