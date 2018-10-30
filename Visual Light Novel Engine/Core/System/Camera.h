#pragma once

class Camera
{
	SINGLETON(Camera)

public:
	void Render();

private:
	SYNTHESIZE(D3DXVECTOR3, position, Position)
	SYNTHESIZE(D3DXVECTOR3, rotation, Rotation)
	SYNTHESIZE(D3DXMATRIX, viewMatrix, ViewMatrix)
};

#define CAMERA Camera::GetInstance()