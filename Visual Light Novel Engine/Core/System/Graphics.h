#pragma once

struct Graphic
{
	ID3D11Device * Device;
	ID3D11DeviceContext * DeviceContext;
	IDXGISwapChain * SwapChain;

	D3DXCOLOR ClearColor;
	D3D11_VIEWPORT Viewport;
	D3DXMATRIX ProjectionMatrix;
	ID3D11DepthStencilView * DepthStencilView;
	ID3D11RenderTargetView * RenderTargetView;

	Setting * WindowSetting;
};

class Graphics
{
	SINGLETON(Graphics)

public:
	void Init(Setting * setting);

	void ResizeClient(UINT width, UINT height, Graphic & graphic);
	void BeginScene();
	void EndScene();

	ID3D11Device * GetDevice(string window);
	ID3D11DeviceContext * GetDeviceContext(string window);
	IDXGISwapChain * GetSwapChain(string window);
	D3DXMATRIX GetProjectionMatrix(string window);

private:
	UINT gpuMemorySize;
	wstring gpuDescription;
	UINT numerator;
	UINT denominator;

	map<string, Graphic> graphics;
};

#define GRAPHICS Graphics::GetInstance()