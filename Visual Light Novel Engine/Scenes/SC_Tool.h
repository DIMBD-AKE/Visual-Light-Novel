#pragma once

class UI;

class SC_Tool : public IScene
{
public:
	SC_Tool();
	~SC_Tool();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;

private:
	void CreateObject2D();
	void CreateUI();

private:
	UI * btnImage;
	UI * btnUI;
};

