#include "../../stdafx.h"
#include "Shader.h"



Shader::Shader()
	: vertexShader(nullptr)
	, pixelShader(nullptr)
	, layout(nullptr)
	, shaderBuffer(nullptr)
	, sampleState(nullptr)
{
	shaderBuffer = new ConstantBuffer();
}


Shader::~Shader()
{
	SAFE_DELETE(shaderBuffer);
	SAFE_RELEASE(layout);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(sampleState);
}

void Shader::CreateResourceFromFile(string filePath, string window)
{
	this->filePath = filePath;
	this->window = window;
}

void * Shader::GetResource()
{
	Shader * res = new Shader();
	res->CreateShaderFromFile(filePath, window);
	return res;
}

void Shader::CreateShaderFromFile(string filePath, string window)
{
	ID3D10Blob * vertexShaderBuffer;
	ID3D10Blob * pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	D3D11_SAMPLER_DESC samplerDesc;
	UINT numElements;

	// Load Shader

	HRESULT hr = D3DX11CompileFromFileA
	(
		filePath.c_str(),
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexShaderBuffer,
		NULL,
		NULL
	);
	assert(SUCCEEDED(hr));

	hr = D3DX11CompileFromFileA
	(
		filePath.c_str(),
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&pixelShaderBuffer,
		NULL,
		NULL
	);
	assert(SUCCEEDED(hr));

	// Create Shader

	hr = GRAPHICS->GetDevice(window)->CreateVertexShader
	(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		NULL,
		&vertexShader
	);
	assert(SUCCEEDED(hr));

	hr = GRAPHICS->GetDevice(window)->CreatePixelShader
	(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&pixelShader
	);
	assert(SUCCEEDED(hr));

	// Create Layout

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	if (filePath != "Shaders/Line.hlsl")
	{
		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;
	}
	else
	{
		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;
	}

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hr = GRAPHICS->GetDevice(window)->CreateInputLayout
	(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&layout
	);
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	shaderBuffer->Create<ShaderBuffer>(window);

	// Create Sample Description

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = GRAPHICS->GetDevice(window)->CreateSamplerState(&samplerDesc, &sampleState);
	assert(SUCCEEDED(hr));

	cache.context = GRAPHICS->GetDeviceContext(window);
}

void Shader::SetShaderParameters(Transform * tf, D3DXVECTOR4 color, D3DXVECTOR4 sizeScale, ID3D11ShaderResourceView * texture, string window)
{
	bool isNull = !tf;
	if (!tf)
		tf = &Transform();
	if (cache.tf != *tf || D3DXVec4LengthSq(&(cache.color - color)) > 0.01f)
	{
		D3DXMATRIX wvp = tf->GetWorldMatrix(window);
		if (isNull)
			D3DXMatrixIdentity(&wvp);
		wvp *= CAMERA->GetViewMatrix();
		wvp *= GRAPHICS->GetProjectionMatrix(window);

		D3DXMatrixTranspose(&wvp, &wvp);

		ShaderBuffer data;
		data.worldViewMatrix = wvp;
		data.color = color;
		data.sizeScale = sizeScale;

		shaderBuffer->Mapped<ShaderBuffer>(data);

		cache.tf = *tf;
		cache.color = color;
	}
	shaderBuffer->SetVS(0, 1);
	shaderBuffer->SetPS(0, 1);

	cache.context->PSSetShaderResources(0, 1, &texture);
}

void Shader::RenderShader(int indexCount, Transform * tf, D3DXVECTOR4 color, D3DXVECTOR4 sizeScale, ID3D11ShaderResourceView * texture, string window)
{
	SetShaderParameters(tf, color, sizeScale, texture, window);

	cache.context->IASetInputLayout(layout);
	cache.context->VSSetShader(vertexShader, NULL, 0);
	cache.context->PSSetShader(pixelShader, NULL, 0);

	cache.context->DrawIndexed(indexCount, 0, 0);
}
