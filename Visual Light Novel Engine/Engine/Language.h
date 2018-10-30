#pragma once
class Language
{
	SINGLETON(Language)

public:
	void ReadLang(string filePath);
	string GetLang(string type, string value);

private:
	map<string, map<string, string>> langData;
};

#define LANG Language::GetInstance()