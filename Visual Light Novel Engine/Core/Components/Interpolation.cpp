#include "../../stdafx.h"
#include "Interpolation.h"



Interpolation::Interpolation()
{
}


Interpolation::~Interpolation()
{
}

void Interpolation::EaseIn(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration)
{
	this->v0 = v0;
	this->v1 = v1 - v0;
	this->duration = duration;
	this->time = 0;
	value = v0;
	type = ITPType::EaseIn;
}

void Interpolation::EaseOut(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration)
{
	this->v0 = v0;
	this->v1 = v1 - v0;
	this->duration = duration;
	this->time = 0;
	value = v0;
	type = ITPType::EaseOut;
}

void Interpolation::EaseInOut(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration)
{
	this->v0 = v0;
	this->v1 = v1 - v0;
	this->duration = duration;
	this->time = 0;
	value = v0;
	type = ITPType::EaseInOut;
}

void Interpolation::Linear(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration)
{
	this->v0 = v0;
	this->v1 = v1;
	this->duration = duration;
	this->time = 0;
	value = v0;
	type = ITPType::Linear;
}

void Interpolation::Bezier2(D3DXVECTOR4 v0, D3DXVECTOR4 v1, D3DXVECTOR4 v2, float duration)
{
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	this->duration = duration;
	this->time = 0;
	value = v0;
	type = ITPType::Bezier2;
}

void Interpolation::Bezier3(D3DXVECTOR4 v0, D3DXVECTOR4 v1, D3DXVECTOR4 v2, D3DXVECTOR4 v3, float duration)
{
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->duration = duration;
	this->time = 0;
	value = v0;
	type = ITPType::Bezier3;
}

float Interpolation::Elapse()
{
	float t = time / duration;
	if (t > 1) t = 1;

	if (type == ITPType::EaseIn)
	{
		value = v1 * t * t + v0;
	}
	if (type == ITPType::EaseOut)
	{
		value = -v1 * t * (t - 2) + v0;
	}
	if (type == ITPType::EaseInOut)
	{
		t = time / (duration / 2);
		if (t > 2) t = 2;
		if (t < 1)
			value = v1 / 2 * t * t + v0;
		else
		{
			t--;
			value = -v1 / 2 * (t * (t - 2) - 1) + v0;
		}
	}
	if (type == ITPType::Linear)
	{
		value = (1 - t) * v0 + t * v1;
	}
	if (type == ITPType::Bezier2)
	{
		value = pow(1 - t, 2) * v0 +
			2 * t *(1 - t) * v1 +
			pow(t, 2) * v2;
	}
	if (type == ITPType::Bezier3)
	{
		value = v0 * pow(1 - t, 3) +
			3 * v1 * t * pow(1 - t, 2) +
			3 * v2 * pow(t, 2) * (1 - t) +
			v3 * pow(t, 3);
	}

	time += PROFILER->GetDeltaTime();

	return t;
}
