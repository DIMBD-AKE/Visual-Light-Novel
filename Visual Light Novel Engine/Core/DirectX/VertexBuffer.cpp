#include "../../stdafx.h"
#include "VertexBuffer.h"



VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(buffer);
}

void VertexBuffer::IASet()
{
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}
