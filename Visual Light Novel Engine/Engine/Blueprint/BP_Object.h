#pragma once
#include "Blueprint.h"

class BP_Object : public Blueprint
{
public:
	BP_Object();
	BP_Object(GameObject * object);
	virtual ~BP_Object();

	virtual void Load(json& data);
	virtual void Save(json &data, int layer, int objIndex, int bpIndex) override;
	virtual void Save(json &data, int bpIndex) override;
	virtual void SubUpdate() override;

	BlueprintObject GetObjectData() { return objectData; }
	void Preview();

	void SetOrigObject(GameObject * orig) { origObject = orig; }
	GameObject * GetOrigObject() { return origObject; }

private:
	BlueprintObject objectData;
	GameObject * origObject;
};

