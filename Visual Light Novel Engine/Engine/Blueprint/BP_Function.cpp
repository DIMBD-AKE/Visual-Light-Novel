#include "../../stdafx.h"
#include "BP_Function.h"
#include "../Object/Object2D.h"



BP_Function::BP_Function()
{
	type = BlueprintType::FUNCTION;
	object2D->SetColor(D3DXVECTOR4(247 / 255.0f, 216 / 255.0f, 1, 1));
}


BP_Function::~BP_Function()
{
}

void BP_Function::Load(json & data)
{
	name = data["DATA"]["VALUE"].value("NAME", "");
}

void BP_Function::Save(json & data, int layer, int objIndex, int bpIndex)
{
	Blueprint::Save(data, layer, objIndex, bpIndex);
	THISPATHO(layer, objIndex, bpIndex)["DATA"]["VALUE"]["NAME"] = name;
}

void BP_Function::Save(json & data, int bpIndex)
{
	Blueprint::Save(data, bpIndex);
	THISPATHE(bpIndex)["DATA"]["VALUE"]["NAME"] = name;
}

void BP_Function::SubUpdate()
{
	wstring conv;
	conv.assign(name.begin(), name.end());
	WRITE->SetAlign(WriteAlign::CENTER);
	WRITE->Text(conv, D3DXVECTOR2(object2D->GetPosition()), "Blueprint", 12, L"³ª´®½ºÄù¾î", D3DXCOLOR(0.5, 0.5, 0.5, 1));
}
