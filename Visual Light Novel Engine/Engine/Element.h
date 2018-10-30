#pragma once
#include "../stdafx.h"

struct BirthDay
{
	int Year, Month, Day;
};

struct Character
{
	string Name;
	string Description;
	string BloodType;
	map<string, string> StateImage;
	BirthDay BirthDay;
};

struct Stuff
{
	string Name;
	string Description;
	map<string, string> StateImage;
};

struct Background
{
	map<string, string> ImagePath;
};