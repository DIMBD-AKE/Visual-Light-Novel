#include "../../stdafx.h"
#include "ConstantBuffer.h"



ConstantBuffer::~ConstantBuffer()
{
	SAFE_RELEASE(buffer);
}

void ConstantBuffer::SetVS(UINT startSlot, UINT numBuffers)
{
	context->VSSetConstantBuffers(startSlot, numBuffers, &buffer);
}

void ConstantBuffer::SetPS(UINT startSlot, UINT numBuffers)
{
	context->PSSetConstantBuffers(startSlot, numBuffers, &buffer);
}
