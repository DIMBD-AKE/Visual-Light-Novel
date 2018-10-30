#include "../../stdafx.h"
#include "Camera.h"


Camera::Camera()
	: position(0, 0, 0)
	, rotation(0, 0, 0)
{
}


Camera::~Camera()
{
}

void Camera::Render()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0), lookAt = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX rotationMatrix;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(rotation.y), D3DXToRadian(rotation.x), D3DXToRadian(rotation.z));

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}
