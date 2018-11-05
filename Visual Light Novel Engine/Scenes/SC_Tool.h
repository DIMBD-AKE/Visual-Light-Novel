#pragma once

class UIObject;

#define TOOLCOUNT 3

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

private:
	UIObject * toolUIObject[TOOLCOUNT];
};

