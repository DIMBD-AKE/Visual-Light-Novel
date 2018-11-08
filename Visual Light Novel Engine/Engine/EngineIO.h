#pragma once
#include "../Core/json.hpp"

using namespace nlohmann;

class Layer;
class IElement;
class BlueprintList;
class GameObject;
enum class GameScene;
enum class ElementType;

enum class FileType
{
	IMAGE,
	JSON
};

class EngineIO
{
public:
	EngineIO();
	~EngineIO();

	static void ProjectSave();
	static void ProjectLoad();

	static void LoadScene(string path, OUT map<GameScene, Layer*>& sceneLayer);
	static void LoadElement(string path, OUT IElement ** element);

	static void LoadBlueprint(json& data, BlueprintList * bpList, GameObject * orig);

	static string OpenFile(FileType type, string title = "");
};

