#pragma once
#include "Blueprint.h"

class UIObject;

class BP_Sequence : public Blueprint
{
public:
	BP_Sequence();
	virtual ~BP_Sequence();

	virtual void SubUpdate() override;

	virtual void Execute(void * data = nullptr) override;
	virtual void SetSubType(BlueprintSubType type, int dataSize) override;

private:
	void CreateInterpolation(Interpolation * itp, float duration, D3DXVECTOR4 v0, D3DXVECTOR4 v1, D3DXVECTOR4 v2, D3DXVECTOR4 v3);
	void FixedRotation(D3DXVECTOR4& rot);
};

