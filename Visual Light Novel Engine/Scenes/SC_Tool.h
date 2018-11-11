#pragma once

class UIObject;

#define TOOLCOUNT 5

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
	void CreateImage();
	void CreateCharacter();
	void CreateUIObject();
	void CreateBackground();
	void CreateContent();

private:
	UIObject * toolUIObject[TOOLCOUNT];
};

