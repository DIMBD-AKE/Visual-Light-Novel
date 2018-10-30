#include "../../stdafx.h"
#include "IndexBuffer.h"



IndexBuffer::~IndexBuffer()
{
	SAFE_RELEASE(buffer);
}

void IndexBuffer::Create(vector<ULONG>& indices, string window)
{
	SAFE_RELEASE(buffer);
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	this->window = window;

	HRESULT hr = GRAPHICS->GetDevice(window)->CreateBuffer(&indexBufferDesc, &indexData, &buffer);
	assert(SUCCEEDED(hr));
}

void IndexBuffer::IASet()
{
	GRAPHICS->GetDeviceContext(window)->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
