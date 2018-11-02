#pragma once
#include "Blueprint.h"

class BP_Object : public Blueprint
{
public:
	BP_Object();
	BP_Object(Object2D * object);
	virtual ~BP_Object();

	virtual void Load(json& data);
	virtual void Save(json &data, int layer, int objIndex, int bpIndex) override;
	virtual void SubUpdate() override;

	BlueprintObject GetObjectData() { return objectData; }
	void Preview();

	void SetOrigObject(Object2D * orig) { origObject = orig; }
	Object2D * GetOrigObject() { return origObject; }

private:
	BlueprintObject objectData;
	Object2D * origObject;
};

