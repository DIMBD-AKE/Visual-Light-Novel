#pragma once

class Interpolation
{
public:
	Interpolation();
	~Interpolation();

	void EaseIn(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration);
	void EaseOut(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration);
	void EaseInOut(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration);

	void Linear(D3DXVECTOR4 v0, D3DXVECTOR4 v1, float duration);

	void Bezier2(D3DXVECTOR4 v0, D3DXVECTOR4 v1, D3DXVECTOR4 v2, float duration);
	void Bezier3(D3DXVECTOR4 v0, D3DXVECTOR4 v1, D3DXVECTOR4 v2, D3DXVECTOR4 v3, float duration);

	float Elapse();

	D3DXVECTOR4 GetValue() { return value; }

private:
	enum class ITPType
	{
		EaseIn,
		EaseOut,
		EaseInOut,
		Linear,
		Bezier2,
		Bezier3
	};

private:
	ITPType type;
	float time, duration;
	D3DXVECTOR4 value, v0, v1, v2, v3;
};

