#pragma once
#include "Blueprint.h"

class BP_Function : public Blueprint
{
public:
	BP_Function();
	~BP_Function();

	virtual void Load(json& data);
	virtual void Save(json &data, int layer, int objIndex, int bpIndex) override;
	virtual void Save(json &data, int bpIndex) override;
	virtual void SubUpdate() override;

	void SetName(string s) { name = s; }
	string GetName() { return name; }

private:
	string name;
};

