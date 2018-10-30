#pragma once

class Main : public IScene
{
public:
	Main();
	virtual ~Main();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;
};

