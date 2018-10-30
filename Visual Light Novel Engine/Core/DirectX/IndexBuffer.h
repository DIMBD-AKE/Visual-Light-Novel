#pragma once
class IndexBuffer
{
public:
	IndexBuffer() : buffer(nullptr) {};
	~IndexBuffer();

	void Create(vector<ULONG>& indices, string window);
	void IASet();

private:
	ID3D11Buffer * buffer;
	string window;
};

