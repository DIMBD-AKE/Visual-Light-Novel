#pragma once
#include "Blueprint.h"

class BP_UI : public Blueprint
{
public:
	BP_UI();
	virtual ~BP_UI();

	virtual void Load(json& data);
	virtual void Save(json &data, int layer, int objIndex, int bpIndex) override;
	virtual void Save(json &data, int objIndex, int bpIndex) override;
	virtual void Save(json &data, int bpIndex) override;
	virtual void SubUpdate() override;
};

