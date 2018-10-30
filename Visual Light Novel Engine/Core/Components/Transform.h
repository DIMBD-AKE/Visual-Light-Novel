#pragma once

class Transform
{
public:
	Transform() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), offset(0, 0, 0) {}
	virtual ~Transform() {}

protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 offset;

public:
	void SetPosition(D3DXVECTOR3 pos) { position = pos; }
	void SetPosition(float x, float y, float z) { position = D3DXVECTOR3(x, y, z); }
	void AddPosition(D3DXVECTOR3 pos) { position += pos; }
	void AddPosition(float x, float y, float z) { position += D3DXVECTOR3(x, y, z); }

	void SetRotation(D3DXVECTOR3 rot) { rotation = rot; }
	void SetRotation(float x, float y, float z) { rotation = D3DXVECTOR3(x, y, z); }
	void AddRotation(D3DXVECTOR3 rot) { rotation += rot; }
	void AddRotation(float x, float y, float z) { rotation += D3DXVECTOR3(x, y, z); }

	void SetScale(D3DXVECTOR3 scale) { this->scale = scale; }
	void SetScale(float x, float y, float z) { scale = D3DXVECTOR3(x, y, z); }
	void AddScale(D3DXVECTOR3 scale) { this->scale += scale; }
	void AddScale(float x, float y, float z) { scale += D3DXVECTOR3(x, y, z); }

	void SetOffset(D3DXVECTOR3 offset) { this->offset = offset; }
	void SetOffset(float x, float y, float z) { offset = D3DXVECTOR3(x, y, z); }
	void AddOffset(D3DXVECTOR3 offset) { this->offset += offset; }
	void AddOffset(float x, float y, float z) { offset += D3DXVECTOR3(x, y, z); }

	D3DXVECTOR3 GetPosition() { return position + offset; }
	D3DXVECTOR3 GetPosition(string window);
	D3DXVECTOR3 GetRotation() { return rotation; }
	D3DXVECTOR3 GetScale() { return scale; }
	D3DXVECTOR3 GetOffset() { return offset; }

	D3DXMATRIX GetWorldMatrix();
	D3DXMATRIX GetWorldMatrix(string window);
};

