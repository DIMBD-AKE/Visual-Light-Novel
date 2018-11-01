#pragma once
#include "IResource.h"
#include "../Components/Transform.h"

struct ShaderBuffer
{
	D3DXMATRIX worldViewMatrix;
	D3DXVECTOR4 color;
	D3DXVECTOR4 sizeScale;
};

struct ShaderBufferCache
{
	Transform tf;
	D3DXVECTOR4 color;
	ID3D11DeviceContext * context;
};

class ConstantBuffer;

class Shader : public IResource
{
private:

public:
	Shader();
	virtual ~Shader();

	virtual void CreateResourceFromFile(string filePath, string window) override;
	virtual void * GetResource() override;

	void CreateShaderFromFile(string filePath, string window);
	void RenderShader(int indexCount, Transform * tf, D3DXVECTOR4 color, D3DXVECTOR4 sizeScale, ID3D11ShaderResourceView * texture, string window);

private:
	void SetShaderParameters(Transform * tf, D3DXVECTOR4 color, D3DXVECTOR4 sizeScale, ID3D11ShaderResourceView * texture, string window);
	
	ID3D11VertexShader * vertexShader;
	ID3D11PixelShader * pixelShader;
	ID3D11InputLayout * layout;
	ConstantBuffer * shaderBuffer;
	ID3D11SamplerState * sampleState;

	ShaderBufferCache cache;
};

