#pragma once
#include "../stdafx.h"

struct Background
{
	map<string, string> ImagePath;
};

struct Character
{
	string Name;
	string Description;
	map<string, string> StateImage;

	Character(string name, string desc)
		: Name(name), Description(desc) {}
};

struct Stuff
{
	string Name;
	string Description;
	map<string, string> StateImage;

	Stuff(string name, string desc)
		: Name(name), Description(desc) {}
};