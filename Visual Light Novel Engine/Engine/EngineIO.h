#pragma once

class Layer;
class IElement;
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

	static string OpenFile(FileType type);
};

