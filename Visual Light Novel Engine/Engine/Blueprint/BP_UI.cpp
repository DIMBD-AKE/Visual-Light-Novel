#include "../../stdafx.h"
#include "BP_UI.h"
#include "../Object/Object.h"


BP_UI::BP_UI()
{
	type = BlueprintType::UI;
	object2D->SetColor(D3DXVECTOR4(227 / 255.0f, 252 / 255.0f, 191 / 255.0f, 1));
}


BP_UI::~BP_UI()
{
}

void BP_UI::Load(json & data)
{
}

void BP_UI::Save(json & data, int layer, int objIndex, int bpIndex)
{
	Blueprint::Save(data, layer, objIndex, bpIndex);
}

void BP_UI::Save(json & data, int objIndex, int bpIndex)
{
	Blueprint::Save(data, objIndex, bpIndex);
}

void BP_UI::Save(json & data, int bpIndex)
{
	Blueprint::Save(data, bpIndex);
}

void BP_UI::SubUpdate()
{
}
