#include "../../stdafx.h"
#include "Graphics.h"


Graphics::Graphics()
	: gpuMemorySize(0)
	, gpuDescription(L"")
	, numerator(0)
	, denominator(0)
{
	
}


Graphics::~Graphics()
{
	for (auto g : graphics)
	{
		SAFE_RELEASE(g.second.DeviceContext);
		SAFE_RELEASE(g.second.DepthStencilView);
		SAFE_RELEASE(g.second.RenderTargetView);
		SAFE_RELEASE(g.second.SwapChain);

		ULONG ul = g.second.Device->Release();
		assert(ul == 0 && "해제되지 않은 메모리 존재");
	}
}


void Graphics::Init(Setting * setting)
{
	Graphic graphic;
	ZeroMemory(&graphic, sizeof(Graphic));

	graphic.WindowSetting = setting;
	graphic.ClearColor = 0xFF555566;

	// Check Device

	IDXGIFactory * factory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	assert(SUCCEEDED(hr));

	IDXGIAdapter * adapter = nullptr;
	hr = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(hr));

	IDXGIOutput * adapterOutput = nullptr;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(hr));

	UINT modeCount;
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&modeCount,
		nullptr
	);
	assert(SUCCEEDED(hr));

	DXGI_MODE_DESC * displayModeList = new DXGI_MODE_DESC[modeCount];
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&modeCount,
		displayModeList
	);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < modeCount; i++)
	{
		bool isCheck = true;
		isCheck &= displayModeList[i].Width == setting->Width;
		isCheck &= displayModeList[i].Height == setting->Height;

		if (isCheck == true)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));

	hr = adapter->GetDesc(&adapterDesc);
	assert(SUCCEEDED(hr));

	gpuMemorySize = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	gpuDescription = adapterDesc.Description;

	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	// Create Swapchain

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (setting->IsVSync == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = setting->WindowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !setting->IsFullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		9,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&graphic.SwapChain,
		&graphic.Device,
		nullptr,
		&graphic.DeviceContext
	);
	assert(SUCCEEDED(hr));

	ResizeClient(setting->Width, setting->Height, graphic);
}

void Graphics::ResizeClient(UINT width, UINT height, Graphic & graphic)
{
	assert(graphic.Device);
	assert(graphic.DeviceContext);
	assert(graphic.SwapChain);

	SAFE_RELEASE(graphic.DepthStencilView);
	SAFE_RELEASE(graphic.RenderTargetView);

	HRESULT hr = graphic.SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));

	// Create Backbuffer

	ID3D11Texture2D* backBuffer = nullptr;
	hr = graphic.SwapChain->GetBuffer
	(
		0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)
	);
	assert(SUCCEEDED(hr));

	hr = graphic.Device->CreateRenderTargetView(backBuffer, nullptr, &graphic.RenderTargetView);
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(backBuffer);

	// Create Depth & Stencil buffer

	ID3D11Texture2D* depthStencilBuffer = nullptr;

	D3D11_TEXTURE2D_DESC dsbDesc;
	ZeroMemory(&dsbDesc, sizeof(D3D11_TEXTURE2D_DESC));
	dsbDesc.Width = width;
	dsbDesc.Height = height;
	dsbDesc.MipLevels = 1;
	dsbDesc.ArraySize = 1;
	dsbDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsbDesc.SampleDesc.Count = 1;
	dsbDesc.SampleDesc.Quality = 0;
	dsbDesc.Usage = D3D11_USAGE_DEFAULT;
	dsbDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.CPUAccessFlags = 0;
	dsbDesc.MiscFlags = 0;

	hr = graphic.Device->CreateTexture2D(&dsbDesc, nullptr, &depthStencilBuffer);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = graphic.Device->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &graphic.DepthStencilView);
	assert(SUCCEEDED(hr));

	graphic.DeviceContext->OMSetRenderTargets(1, &graphic.RenderTargetView, graphic.DepthStencilView);

	SAFE_RELEASE(depthStencilBuffer);

	// Set Viewport

	graphic.Viewport.TopLeftX = 0;
	graphic.Viewport.TopLeftY = 0;
	graphic.Viewport.Width = static_cast<float>(graphic.WindowSetting->Width);
	graphic.Viewport.Height = static_cast<float>(graphic.WindowSetting->Height);
	graphic.Viewport.MinDepth = 0.0f;
	graphic.Viewport.MaxDepth = 1.0f;

	graphic.WindowSetting->Viewport = graphic.Viewport;

	graphic.DeviceContext->RSSetViewports(1, &graphic.Viewport);

	// Set Depth Stencil State

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ID3D11DepthStencilState* dsState = nullptr;

	hr = graphic.Device->CreateDepthStencilState(&dsDesc, &dsState);
	assert(SUCCEEDED(hr));

	graphic.DeviceContext->OMSetDepthStencilState(dsState, 0);

	SAFE_RELEASE(dsState);

	// Set Blend State

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	ID3D11BlendState * blendState;

	hr = graphic.Device->CreateBlendState(&blendDesc, &blendState);
	assert(SUCCEEDED(hr));

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	graphic.DeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	SAFE_RELEASE(blendState);

	// Create Projection

	if (graphic.WindowSetting->IsOrtho)
		D3DXMatrixOrthoLH
		(
			&graphic.ProjectionMatrix,
			graphic.WindowSetting->Width,
			graphic.WindowSetting->Height,
			0, 
			1000
		);
	else
		D3DXMatrixPerspectiveFovLH
		(
			&graphic.ProjectionMatrix,
			graphic.WindowSetting->FOV,
			graphic.WindowSetting->Width / graphic.WindowSetting->Height,
			0,
			1000
		);

	graphics[graphic.WindowSetting->SettingName] = graphic;
}

void Graphics::BeginScene()
{
	for (auto graphic : graphics)
	{
		assert(graphic.second.DeviceContext != nullptr);
		assert(graphic.second.SwapChain != nullptr);

		graphic.second.DeviceContext->OMSetRenderTargets
		(
			1,
			&graphic.second.RenderTargetView,
			graphic.second.DepthStencilView
		);

		graphic.second.DeviceContext->ClearRenderTargetView
		(
			graphic.second.RenderTargetView,
			graphic.second.ClearColor
		);

		graphic.second.DeviceContext->ClearDepthStencilView
		(
			graphic.second.DepthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
		);
	}
}

void Graphics::EndScene()
{
	for (auto graphic : graphics)
	{
		HRESULT hr = graphic.second.SwapChain->Present(graphic.second.WindowSetting->IsVSync ? 1 : 0, 0);
		assert(SUCCEEDED(hr));
	}
}

ID3D11Device * Graphics::GetDevice(string window)
{
	if (graphics.find(window) == graphics.end())
		return nullptr;
	return graphics[window].Device;
}

ID3D11DeviceContext * Graphics::GetDeviceContext(string window)
{
	if (graphics.find(window) == graphics.end())
		return nullptr;
	return graphics[window].DeviceContext;
}

IDXGISwapChain * Graphics::GetSwapChain(string window)
{
	if (graphics.find(window) == graphics.end())
		return nullptr;
	return graphics[window].SwapChain;
}

D3DXMATRIX Graphics::GetProjectionMatrix(string window)
{
	if (graphics.find(window) == graphics.end())
	{
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		return identity;
	}
	return graphics[window].ProjectionMatrix;
}
