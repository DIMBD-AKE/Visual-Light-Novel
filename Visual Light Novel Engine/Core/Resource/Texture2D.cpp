#include "../../stdafx.h"
#include "Texture2D.h"
#include "../Util.h"


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
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
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

	fileName = Util::GetFileName(const_cast<char*>(filePath.c_str()));
}