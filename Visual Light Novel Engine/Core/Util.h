#pragma once
#include <commdlg.h>
#include <locale>
#include <codecvt>

namespace Math
{
	template<typename T>
	static T Clamp(T min, T max, T value)
	{
		if (value > max) value = max;
		if (value < min) value = min;
		return value;
	}
};

namespace Util
{
	static string VectorToString(D3DXVECTOR2 v)
	{
		return to_string(v.x) + ", " + to_string(v.y);
	}

	static string VectorToString(D3DXVECTOR3 v)
	{
		return to_string(v.x) + ", " + to_string(v.y) + ", " + to_string(v.z);
	}

	static string VectorToString(D3DXVECTOR4 v)
	{
		return to_string(v.x) + ", " + to_string(v.y) + ", " + to_string(v.z) + ", " + to_string(v.w);
	}

	static D3DXVECTOR2 StringToVector2(string s)
	{
		D3DXVECTOR2 v;
		sscanf_s(s.c_str(), "%f, %f", &v.x, &v.y);
		return v;
	}

	static D3DXVECTOR3 StringToVector3(string s)
	{
		D3DXVECTOR3 v;
		sscanf_s(s.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);
		return v;
	}

	static D3DXVECTOR4 StringToVector4(string s)
	{
		D3DXVECTOR4 v;
		sscanf_s(s.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.w);
		return v;
	}

	static wstring S2W(string s)
	{
		wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(s);
	}
}