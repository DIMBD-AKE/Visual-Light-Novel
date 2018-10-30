#pragma once

class UI;

class SC_Element : public IScene
{
public:
	SC_Element();
	~SC_Element();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;

private:
	void Test();

private:
	UI * sideUI;
};

