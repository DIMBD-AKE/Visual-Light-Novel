#include "../../stdafx.h"
#include "BackgroundObject.h"
#include "../../Core/Util.h"


BackgroundObject::BackgroundObject()
{
}


BackgroundObject::~BackgroundObject()
{
}

void BackgroundObject::Update()
{
}

void BackgroundObject::Render()
{
}

Texture2D * BackgroundObject::GetTexture2D()
{
	return nullptr;
}

void BackgroundObject::Save(json & data)
{
#define THISPATH data["BACKGROUND"]

	THISPATH["POSITION"] = Util::VectorToString(position);
	THISPATH["ROTATION"] = Util::VectorToString(rotation);
	THISPATH["SCALE"] = Util::VectorToString(scale);
	THISPATH["COLOR"] = Util::VectorToString(color);
}
