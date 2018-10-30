#pragma once
#include <commdlg.h>

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

	enum class FileType
	{
		IMAGE
	};

	static string OpenFile(FileType type)
	{
		char szFile[256];
		szFile[0] = '\0';

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = nullptr;
		if (type == FileType::IMAGE)
			ofn.lpstrFilter = "Image (*.png, *.jpg)\0*.png;*.jpg";
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrInitialDir = ".\\";

		char curPath[256];
		GetCurrentDirectory(256, curPath);
		HRESULT hr = GetOpenFileName(&ofn);
		assert(SUCCEEDED(hr));

		if (szFile[0] != '\0')
		{
			SetCurrentDirectory(curPath);
			string path(szFile);
			path = path.substr(strlen(curPath) + 1, path.size());
			return path;
		}

		return "";
	}
}