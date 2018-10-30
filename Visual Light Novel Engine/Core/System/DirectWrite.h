#pragma once

struct Font
{
	wstring Name;
	float Size;
	DWRITE_FONT_WEIGHT Weight;
	DWRITE_FONT_STYLE Style;
	DWRITE_FONT_STRETCH Stretch;

	bool operator==(const Font& font)
	{
		bool ret = true;
		ret &= Size == font.Size;
		ret &= Weight == font.Weight;
		ret &= Style == font.Style;
		ret &= Stretch == font.Stretch;
		return ret;
	}
};

struct Write
{
	ID2D1Device * WriteDevice;
	ID2D1DeviceContext * WriteDeviceContext;
	ID2D1Bitmap1 * TargetBitmap;
	IDWriteFactory * WriteFactory;

	vector<pair<Font, IDWriteTextFormat*>> Fonts;
	vector<pair<D3DXCOLOR, ID2D1SolidColorBrush*>> Brushes;
};

enum class WriteAlign
{
	LEFT,
	CENTER
};

class DirectWrite
{
	SINGLETON(DirectWrite)

public:
	void BeginDraw();
	void EndDraw();

	void Init(string window);

	void Text
	(
		wstring text, 
		D3DXVECTOR2 position,
		string window,
		float fontSize = 20.0f,
		wstring fontName = L"³ª´®½ºÄù¾î",
		D3DXCOLOR fontColor = D3DXCOLOR(1, 1, 1, 1),
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL
	);
	void SetAlign(WriteAlign align) { this->align = align; }

	void Destroy();

private:
	IDWriteTextFormat* RegistFont
	(
		wstring fontName,
		float fontSize,
		DWRITE_FONT_WEIGHT fontWeight,
		DWRITE_FONT_STYLE fontStyle,
		DWRITE_FONT_STRETCH fontStretch,
		string window
	);
	IDWriteTextFormat* FindFormat(Font font, string window);

	ID2D1SolidColorBrush * RegistBrush(D3DXCOLOR color, string window);
	ID2D1SolidColorBrush * FindBrush(D3DXCOLOR color, string window);

	WriteAlign align;

private:
	
	map<string, Write> writes;
};

#define WRITE DirectWrite::GetInstance()