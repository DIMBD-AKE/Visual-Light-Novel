#pragma once

class IResource
{
public:
	virtual ~IResource() {}
	virtual void CreateResourceFromFile(string filePath, string window) = 0;
	virtual void * GetResource() = 0;

protected:
	string filePath;
	string window;
};