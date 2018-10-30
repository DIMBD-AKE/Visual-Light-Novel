#include "../stdafx.h"
#include "SC_Preview.h"
#include "../Engine/UI.h"
#include "../Engine/Layer.h"


SC_Preview::SC_Preview()
	: objectLayer(nullptr)
{
}


SC_Preview::~SC_Preview()
{
}

void SC_Preview::Init()
{
	objectLayer = new Layer();
}

void SC_Preview::Update()
{
	objectLayer->Update();
}

void SC_Preview::Render()
{
	objectLayer->Render();
}

bool SC_Preview::Release()
{
	SAFE_DELETE(objectLayer);
	return true;
}
