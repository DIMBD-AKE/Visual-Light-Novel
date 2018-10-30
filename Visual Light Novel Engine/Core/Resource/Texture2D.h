#pragma once
#include "IResource.h"

class Texture2D : public IResource
{
public:
	Texture2D();
	virtual ~Texture2D();

	virtual void CreateResourceFromFile(string filePath, string window) override;

	void CreateTextureFromFile(string filePath, string window);

	ID3D11ShaderResourceView * GetSRV() { return srv; }
	D3D11_TEXTURE2D_DESC GetDesc() { return textureDesc; }
	string GetFileName() { return fileName; }

private:
	ID3D11ShaderResourceView * srv;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	string fileName;
};

