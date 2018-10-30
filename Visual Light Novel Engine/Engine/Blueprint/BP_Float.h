#pragma once
#include "Blueprint.h"

class BP_Float : public Blueprint
{
public:
	BP_Float();
	virtual ~BP_Float(); 
	
	virtual void Load(json& data);
	virtual void SubUpdate() override;

	float GetValue() { return value; }
	void SetValue(float f) { value = f; }

private:
	float value;
};

