#pragma once

class UIObject;
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

	UIObject * sideUIObject;
};

