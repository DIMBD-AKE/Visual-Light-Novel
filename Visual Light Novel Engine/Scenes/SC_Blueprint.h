#pragma once
#include "../Core/System/Scene.h"
#include "../Engine/Blueprint/Blueprint.h"

class UI;

class SC_Blueprint : public IScene
{
public:
	SC_Blueprint();
	~SC_Blueprint();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool Release() override;

	int * GetDataIndex() { return &dataIndex; }

private:
	void CreateBlueprint(BlueprintType type);
	void ChangeBlueprintSubType(BlueprintSubType type, int dataSize);
	void SetSubData(int index);

private:
	UI * createUI;
	UI * seqListUI;
	UI * seqDataListUI;

	int dataIndex;

	Object2D ** selectObject;
};

