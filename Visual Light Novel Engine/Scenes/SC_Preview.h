#pragma once

class Layer;

class SC_Preview : public IScene
{
public:
	SC_Preview();
	~SC_Preview();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;

private:
	Layer * objectLayer;
};

