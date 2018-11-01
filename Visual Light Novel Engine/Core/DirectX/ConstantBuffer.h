#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer() : buffer(nullptr) {};
	~ConstantBuffer();

	template<typename T>
	void Create(string window);

	template<typename T>
	void Mapped(T& data);

	void SetVS(UINT startSlot, UINT numBuffers);
	void SetPS(UINT startSlot, UINT numBuffers);

private:
	ID3D11DeviceContext * context;
	ID3D11Buffer * buffer;
	string window;
};

template<typename T>
inline void ConstantBuffer::Create(string window)
{
	SAFE_RELEASE(buffer);
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(T);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	context = GRAPHICS->GetDeviceContext(window);
	this->window = window;

	HRESULT hr = GRAPHICS->GetDevice(window)->CreateBuffer(&bufferDesc, NULL, &buffer);
	assert(SUCCEEDED(hr));
}

template<typename T>
inline void ConstantBuffer::Mapped(T & data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	T * dataPtr;

	HRESULT hr = GRAPHICS->GetDeviceContext(window)->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	dataPtr = static_cast<T*>(mappedResource.pData);
	*dataPtr = data;

	GRAPHICS->GetDeviceContext(window)->Unmap(buffer, 0);
}
