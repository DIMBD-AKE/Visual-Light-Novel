#pragma once

struct VertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

struct VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

class VertexBuffer
{
public:
	VertexBuffer() : buffer(nullptr) {};
	~VertexBuffer();

	template <typename T>
	void Create(vector<T>& vertices, string window);

	template <typename T>
	void Mapped(vector<T>& vertices);

	void IASet();

private:
	ID3D11Buffer * buffer;
	string window;
	UINT stride;
};

template<typename T>
inline void VertexBuffer::Create(vector<T>& vertices, string window)
{
	SAFE_RELEASE(buffer);
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(T) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	stride = sizeof(T);
	this->window = window;

	HRESULT hr = GRAPHICS->GetDevice(window)->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer);
	assert(SUCCEEDED(hr));
}

template<typename T>
inline void VertexBuffer::Mapped(vector<T>& vertices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	T * verticesPtr;

	HRESULT hr = GRAPHICS->GetDeviceContext(window)->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	verticesPtr = static_cast<T*>(mappedResource.pData);
	memcpy(verticesPtr, (void*)vertices.data(), sizeof(T) * vertices.size());

	GRAPHICS->GetDeviceContext(window)->Unmap(buffer, 0);
}
