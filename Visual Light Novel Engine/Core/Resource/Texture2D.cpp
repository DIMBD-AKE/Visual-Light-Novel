#include "../../stdafx.h"
#include "Texture2D.h"


Texture2D::Texture2D()
	: srv(nullptr)
	, fileName("")
{
}

Texture2D::~Texture2D()
{
	SAFE_RELEASE(srv);
}

void Texture2D::CreateResourceFromFile(string filePath, string window)
{
	CreateTextureFromFile(filePath, window);
	this->filePath = filePath;
	this->window = window;
}

void * Texture2D::GetResource()
{
	return this;
}

void Texture2D::CreateTextureFromFile(string filePath, string window)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFileA
	(
		GRAPHICS->GetDevice(window),
		filePath.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	);
	assert(SUCCEEDED(hr));

	srv->GetDesc(&srvDesc);
	ID3D11Texture2D * texture;
	srv->GetResource(reinterpret_cast<ID3D11Resource**>(&texture));
	texture->GetDesc(&textureDesc);
	SAFE_RELEASE(texture);

	char path[256] = "";
	char * context = nullptr;
	strcat(path, filePath.c_str());
	char * tok = strtok_s(path, "//\\", &context);
	while (tok)
	{
		fileName = string(tok);
		tok = strtok_s(NULL, "//\\", &context);
	}
}