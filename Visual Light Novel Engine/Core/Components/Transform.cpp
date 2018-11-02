#include "../../stdafx.h"
#include "Transform.h"

bool Transform::operator==(Transform & tf)
{
	if (D3DXVec3LengthSq(&(GetPosition() - tf.GetPosition())) > FLT_EPSILON) return false;
	if (D3DXVec3LengthSq(&(GetRotation() - tf.GetRotation())) > FLT_EPSILON) return false;
	if (D3DXVec3LengthSq(&(GetScale() - tf.GetScale())) > FLT_EPSILON) return false;
	return true;
}

bool Transform::operator!=(Transform & tf)
{
	return !(*this == tf);
}

D3DXVECTOR3 Transform::GetPosition(string window)
{
	D3DXVECTOR3 pos = position + offset;
	pos.x -= SETTING->GetWindow(window)->Width / 2.0f;
	pos.y = -pos.y + SETTING->GetWindow(window)->Height / 2.0f;
	return pos;
}

D3DXMATRIX Transform::GetWorldMatrix()
{
	D3DXMATRIX s, r, t;
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&r, D3DXToRadian(rotation.y), D3DXToRadian(rotation.x), D3DXToRadian(rotation.z));
	D3DXMatrixTranslation(&t, position.x + offset.x, position.y + offset.y, position.z + offset.z);

	return s * r * t;
}

D3DXMATRIX Transform::GetWorldMatrix(string window)
{
	D3DXMATRIX s, r, t;
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&r, D3DXToRadian(rotation.y), D3DXToRadian(rotation.x), D3DXToRadian(rotation.z));
	D3DXMatrixTranslation(&t, (position.x + offset.x) - SETTING->GetWindow(window)->Width / 2.0f,
		-(position.y + offset.y) + SETTING->GetWindow(window)->Height / 2.0f, position.z + offset.z);

	return s * r * t;
}