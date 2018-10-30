#include "../../stdafx.h"
#include "BP_Float.h"



BP_Float::BP_Float()
	: value(0)
{
	type = BlueprintType::FLOAT;
	color = D3DXVECTOR4(1, 204 / 255.0f, 204 / 255.0f, 1);
	object2D->SetColor(color);
}


BP_Float::~BP_Float()
{
}

void BP_Float::Load(json & data)
{
	string floatValue = data["DATA"]["VALUE"].value("FLOAT", "");
	value = atof(floatValue.c_str());
}

void BP_Float::SubUpdate()
{
	WRITE->SetAlign(WriteAlign::CENTER);
	WRITE->Text(to_wstring(value), D3DXVECTOR2(object2D->GetPosition()), "Blueprint", 12, L"³ª´®½ºÄù¾î", D3DXCOLOR(0.5, 0.5, 0.5, 1));
}
