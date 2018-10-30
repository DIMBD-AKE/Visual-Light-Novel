#pragma once
#include "../Resource/IResource.h"
#include "../Resource/Texture2D.h"
#include "../Resource/Shader.h"

enum class ResourceType
{
	TEXTURE,
	SHADER,
	UNKNOWN
};

class Resources
{
	SINGLETON(Resources);

public:
	template<typename T> void AddResource(string filePath, string window);
	template<typename T> T * GetResource(string filePath, string window);

	void Destroy();

private:
	template<typename T> ResourceType GetResourceType();
	map<ResourceType, map<string, IResource*>> resources;
};

#define RESOURCES Resources::GetInstance()

template<typename T>
inline void Resources::AddResource(string filePath, string window)
{
	ResourceType type = GetResourceType<T>();

	if (resources[type].find(filePath + window) != resources[type].end())
		return;

	IResource * resource;
	if (type == ResourceType::TEXTURE) resource = new Texture2D();
	if (type == ResourceType::SHADER) resource = new Shader();

	resource->CreateResourceFromFile(filePath, window);
	resources[type][filePath + window] = resource;
}

template<typename T>
inline T * Resources::GetResource(string filePath, string window)
{
	ResourceType type = GetResourceType<T>();

	if (resources[type].find(filePath + window) == resources[type].end())
		AddResource<T>(filePath, window);

	return dynamic_cast<T*>(resources[type][filePath + window]);
}

template<typename T>
inline ResourceType Resources::GetResourceType()
{
	ResourceType type = ResourceType::UNKNOWN;

	if (is_same<T, Texture2D>::value) type = ResourceType::TEXTURE;
	if (is_same<T, Shader>::value) type = ResourceType::SHADER;

	return type;
}
