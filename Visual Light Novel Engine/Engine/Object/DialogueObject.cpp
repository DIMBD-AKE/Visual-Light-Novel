#include "../../stdafx.h"
#include "DialogueObject.h"
#include "../../Core/Util.h"

ContentObject::ContentObject()
{
}

ContentObject::~ContentObject()
{
}

void ContentObject::Update()
{
}

void ContentObject::Render()
{
}

Texture2D * ContentObject::GetTexture2D()
{
	return nullptr;
}

void ContentObject::Save(json & data)
{
#define THISPATH data["DIALOGUE"]["CONTENT"]

	THISPATH["POSITION"] = Util::VectorToString(position);
	THISPATH["COLOR"] = Util::VectorToString(color);
}

NameObject::NameObject()
{
}

NameObject::~NameObject()
{
}

void NameObject::Update()
{
}

void NameObject::Render()
{
}

Texture2D * NameObject::GetTexture2D()
{
	return nullptr;
}

void NameObject::Save(json & data)
{
#define THISPATH data["DIALOGUE"]["NAME"]

	THISPATH["POSITION"] = Util::VectorToString(position);
	THISPATH["COLOR"] = Util::VectorToString(color);
}
