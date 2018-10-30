#include "../../stdafx.h"
#include "ConstantBuffer.h"



ConstantBuffer::~ConstantBuffer()
{
	SAFE_RELEASE(buffer);
}

void ConstantBuffer::SetVS(UINT startSlot, UINT numBuffers)
{
	GRAPHICS->GetDeviceContext(window)->VSSetConstantBuffers(startSlot, numBuffers, &buffer);
}

void ConstantBuffer::SetPS(UINT startSlot, UINT numBuffers)
{
	GRAPHICS->GetDeviceContext(window)->PSSetConstantBuffers(startSlot, numBuffers, &buffer);
}
