#include "../../stdafx.h"
#include "DirectWrite.h"


DirectWrite::DirectWrite()
	: align(WriteAlign::LEFT)
{
	ZeroMemory(&cache, sizeof(WriteCache));
}


DirectWrite::~DirectWrite()
{
	
}

void DirectWrite::BeginDraw()
{
	for(auto data : writes)
		data.second.WriteDeviceContext->BeginDraw();
}

void DirectWrite::EndDraw()
{
	for (auto data : writes)
	{
		HRESULT hr = data.second.WriteDeviceContext->EndDraw();
		assert(SUCCEEDED(hr));
	}
}

void DirectWrite::Init(string window)
{
	Write write;

	// Create Factory

	write.WriteFactory = nullptr;
	HRESULT hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		_uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&write.WriteFactory)
	);
	assert(SUCCEEDED(hr));

	ID2D1Factory1 * factory = nullptr;
	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_NONE;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	assert(SUCCEEDED(hr));

	IDXGIDevice * dxgiDevice = nullptr;
	hr = GRAPHICS->GetDevice(window)->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(hr));

	IDXGISurface * dxgiSurface = nullptr;
	hr = GRAPHICS->GetSwapChain(window)->GetBuffer(0, _uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgiSurface));
	assert(SUCCEEDED(hr));

	hr = factory->CreateDevice(dxgiDevice, &write.WriteDevice);
	assert(SUCCEEDED(hr));

	hr = write.WriteDevice->CreateDeviceContext
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&write.WriteDeviceContext
	);
	assert(SUCCEEDED(hr));

	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bitmapProperties.dpiX = 96;
	bitmapProperties.dpiY = 96;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.colorContext = nullptr;

	hr = write.WriteDeviceContext->CreateBitmapFromDxgiSurface
	(
		dxgiSurface,
		&bitmapProperties,
		&write.TargetBitmap
	);
	assert(SUCCEEDED(hr));

	write.WriteDeviceContext->SetTarget(write.TargetBitmap);

	SAFE_RELEASE(dxgiSurface);
	SAFE_RELEASE(dxgiDevice);

	writes[window] = write;
}

void DirectWrite::Text(wstring text, D3DXVECTOR2 position, string window, float fontSize, wstring fontName, D3DXCOLOR fontColor, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
{
	float length = fontSize * text.size();

	D2D1_RECT_F range;
	range.left = position.x;
	range.top = position.y;
	range.bottom = position.y + fontSize;
	range.right = position.x + length;

	auto format = RegistFont(fontName, fontSize, fontWeight, fontStyle, fontStretch, window);
	auto brush = RegistBrush(fontColor, window);

	if (align == WriteAlign::LEFT)
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (align == WriteAlign::CENTER)
	{
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		range.left -= length / 2;
		range.right = position.x + length / 2;
	}

	ID2D1DeviceContext * context = nullptr;

	if (cache.Window.compare(window) || !cache.WriteDeviceContext)
	{
		cache.WriteDeviceContext = writes[window].WriteDeviceContext;
		cache.Window = window;
	}

	context = cache.WriteDeviceContext;

	cache.WriteDeviceContext->DrawText
	(
		text.c_str(),
		text.size(),
		format,
		range,
		brush
	);
}

void DirectWrite::Destroy()
{
	for (auto data : writes)
	{
		SAFE_RELEASE(data.second.WriteDevice);
		SAFE_RELEASE(data.second.WriteDeviceContext);
		SAFE_RELEASE(data.second.TargetBitmap);
		SAFE_RELEASE(data.second.WriteFactory);

		for (auto temp : data.second.Fonts)
			SAFE_RELEASE(temp.second);

		for (auto temp : data.second.Brushes)
			SAFE_RELEASE(temp.second);
	}
}

IDWriteTextFormat * DirectWrite::RegistFont(wstring fontName, float fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, string window)
{
	Font font;
	font.Name = fontName;
	font.Size = fontSize;
	font.Weight = fontWeight;
	font.Style = fontStyle;
	font.Stretch = fontStretch;

	if (cache.Font.first == font && cache.Font.second && cache.Window.compare(window) == 0)
		return cache.Font.second;

	IDWriteTextFormat * format = FindFormat(font, window);

	if (!format)
	{
		HRESULT hr = writes[window].WriteFactory->CreateTextFormat
		(
			fontName.c_str(),
			nullptr,
			font.Weight,
			font.Style,
			font.Stretch,
			font.Size,
			L"ko",
			&format
		);
		assert(SUCCEEDED(hr));

		writes[window].Fonts.push_back(make_pair(font, format));
	}

	cache.Font.first = font;
	cache.Font.second = format;

	return format;
}

ID2D1SolidColorBrush * DirectWrite::RegistBrush(D3DXCOLOR color, string window)
{
	if (cache.Brush.first == color && cache.Brush.second && cache.Window.compare(window) == 0)
		return cache.Brush.second;

	ID2D1SolidColorBrush * brush = FindBrush(color, window);

	if (!brush)
	{
		D2D1::ColorF brushColor(color.r, color.g, color.b);
		HRESULT hr = writes[window].WriteDeviceContext->CreateSolidColorBrush(brushColor, &brush);
		assert(SUCCEEDED(hr));

		writes[window].Brushes.push_back(make_pair(color, brush));
	}

	cache.Brush.first = color;
	cache.Brush.second = brush;

	return brush;
}

ID2D1SolidColorBrush * DirectWrite::FindBrush(D3DXCOLOR color, string window)
{
	for (auto temp : writes[window].Brushes)
	{
		if (temp.first == color)
			return temp.second;
	}
	return nullptr;
}

IDWriteTextFormat * DirectWrite::FindFormat(Font font, string window)
{
	for (auto temp : writes[window].Fonts)
	{
		if (temp.first == font)
			return temp.second;
	}
	return nullptr;
}
