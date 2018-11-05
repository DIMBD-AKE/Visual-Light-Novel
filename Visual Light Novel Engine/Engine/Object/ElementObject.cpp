#include "../../stdafx.h"
#include "ElementObject.h"
#include "../EngineIO.h"


ElementObject::ElementObject(string window, string elementPath, string shaderPath)
	: element(nullptr)
	, shader(nullptr)
{
	EngineIO::LoadElement(elementPath, &element);

	if (!element)
	{
		MessageBox(NULL, "엘리먼트 형식이 아닙니다!", "엘리먼트 오류!", MB_OK | MB_ICONERROR);
		return;
	}

	for (auto path : element->StateImage)
	{
		Texture2D * tex = RESOURCES->GetResource<Texture2D>(path.second, window);
		textures.insert(make_pair(path.first, tex));
	}

	stateName = textures.begin()->first;

	shader = RESOURCES->GetResource<Shader>(shaderPath, window);
	this->window = window;
	this->shaderPath = shaderPath;
	this->elementPath = elementPath;

	context = GRAPHICS->GetDeviceContext(window);

	CreateBuffers();
}

ElementObject::~ElementObject()
{
	SAFE_DELETE(shader);
}

void ElementObject::Update()
{
	Drag();
}

void ElementObject::Render()
{
	CAMERA->Render();
	RenderBuffers();

	for (auto tex : textures)
	{
		if (tex.first.compare(stateName) == 0)
		{
			shader->RenderShader
			(
				6,
				this,
				color,
				D3DXVECTOR4(tex.second->GetDesc().Width, tex.second->GetDesc().Height, scale.x, scale.y),
				tex.second->GetSRV(),
				window
			);
		}
	}
}

void ElementObject::SetState(wstring state)
{
	stateName = state;
}

bool ElementObject::IsOver()
{
	RECT rc;
	rc.left = position.x + offset.x + vertexRect.left * scale.x;
	rc.right = position.x + offset.x + vertexRect.right * scale.x;
	rc.top = position.y + offset.y + -vertexRect.top * scale.y;
	rc.bottom = position.y + offset.y + -vertexRect.bottom * scale.y;

	if (PtInRect(&rc, INPUT->GetMousePos(window)) && IsFocus(window))
		return true;

	return false;
}