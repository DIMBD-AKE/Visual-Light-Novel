#pragma once
#include "IResource.h"

struct ShaderBuffer
{
	D3DXMATRIX worldViewMatrix;
	D3DXVECTOR4 color;
	D3DXVECTOR4 sizeScale;
};

class ConstantBuffer;

class Shader : public IResource
{
private:

public:
	Shader();
	virtual ~Shader();

	virtual void CreateResourceFromFile(string filePath, string window) override;

	void CreateShaderFromFile(string filePath, string window);
	void SetShaderParameters(D3DXMATRIX wvp, D3DXVECTOR4 color, D3DXVECTOR4 sizeScale, ID3D11ShaderResourceView * texture, string window);
	void RenderShader(int indexCount, D3DXMATRIX wvp, D3DXVECTOR4 color, D3DXVECTOR4 sizeScale, ID3D11ShaderResourceView * texture, string window);

private:
	ID3D11VertexShader * vertexShader;
	ID3D11PixelShader * pixelShader;
	ID3D11InputLayout * layout;
	ConstantBuffer * shaderBuffer;
	ID3D11SamplerState * sampleState;
};

