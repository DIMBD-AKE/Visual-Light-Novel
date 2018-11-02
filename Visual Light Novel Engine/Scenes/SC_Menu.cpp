#include "../stdafx.h"
#include "SC_Menu.h"
#include "../Engine/UI.h"
#include "../Engine/json.hpp"
#include "../Engine/dirent.h"
#include "../Core/Util.h"
#include "SC_Editor.h"
#include "../Engine/Blueprint/Blueprint.h"
#include "../Engine/Blueprint/BP_Object.h"
#include "../Engine/Blueprint/BP_Float.h"
#include "../Engine/Blueprint/BP_Function.h"
#include <fstream>
#include <iomanip>

using namespace nlohmann;

SC_Menu::SC_Menu()
	: sideUI(nullptr)
{
	
}


SC_Menu::~SC_Menu()
{
}

void SC_Menu::Init()
{
	vector<UIList> list;
	UIList listData;
	UIAction action;
	action.Type = UIActionType::CHANGE_SCENE;
	action.ChangeSceneWindow = "Game";

	action.ChangeSceneName = "Preview";
	listData.Text = L"Preview";
	listData.Action = action;
	list.push_back(listData);
	action.ChangeSceneName = "Editor";
	listData.Text = L"Editor";
	listData.Action = action;
	list.push_back(listData);
	action.ChangeSceneName = "Element";
	listData.Text = L"Element";
	listData.Action = action;
	list.push_back(listData);

	action.Type = UIActionType::CUSTOM_FUNCTION;
	action.CustomFunction = bind(&SC_Menu::ProjectLoad, this);
	listData.Text = L"Load";
	listData.Action = action;
	list.push_back(listData);
	action.CustomFunction = bind(&SC_Menu::ProjectSave, this);
	listData.Text = L"Save";
	listData.Action = action;
	list.push_back(listData);

	sideUI = new UI(UIType::LISTV, "Menu", "Images/UI/Menu Button.png");
	sideUI->SetList(list);
}

void SC_Menu::Update()
{
	sideUI->Update();}

void SC_Menu::Render()
{
	sideUI->Render();
}

bool SC_Menu::Release()
{
	SAFE_DELETE(sideUI);
	return true;
}

void SC_Menu::ProjectSave()
{
	auto sceneLayer = static_cast<SC_Editor*>(SCENE->GetScene("Editor"))->GetSceneLayer();
	CreateDirectory("Project", NULL);
	CreateDirectory("Project/Scenes", NULL);
	CreateDirectory("Project/Elements", NULL);
	for (auto scene : sceneLayer)
	{
		string sceneName;
		if (scene.first == GameScene::TITLE) sceneName = "TITLE";
		if (scene.first == GameScene::GAME) sceneName = "GAME";
		if (scene.first == GameScene::CG) sceneName = "CG";
		if (scene.first == GameScene::OPTION) sceneName = "OPTION";
		if (scene.first == GameScene::SELECT) sceneName = "SELECT";

		json data;
		data["SCENE"] = sceneName;

		for (auto layer : *scene.second->GetLayers())
		{
			for (int i = 0; i < layer.second.size(); i++)
			{
				layer.second[i]->Save(data, layer.first, i);
			}
		}

		ofstream stream("Project/Scenes/" + sceneName + ".json");
		stream << setw(2) << data << endl;
	}
}

void SC_Menu::ProjectLoad()
{
	map<GameScene, Layer*> sceneLayer;

	DIR * dir = opendir("Project/Scenes/");
	dirent * ent;
	if (dir != nullptr)
	{
		int i = 0;
		while ((ent = readdir(dir)) != nullptr)
		{
			i++;
			if (i > 2)
			{
				string fileName = ent->d_name;
				SceneLoad("Project/Scenes/" + fileName, sceneLayer);
			}
		}
	}

	static_cast<SC_Editor*>(SCENE->GetScene("Editor"))->SetSceneLayer(sceneLayer);

	dir = opendir("Project/Elements/");
	ent;
	if (dir != nullptr)
	{
		int i = 0;
		while ((ent = readdir(dir)) != nullptr)
		{
			i++;
			if (i > 2)
			{
				string fileName = ent->d_name;
				
			}
		}
	}
}

void SC_Menu::SceneLoad(string path, map<GameScene, Layer*>& sceneLayer)
{
	ifstream stream(path);
	json data;
	stream >> data;

	GameScene scene;
	if (data.find("SCENE") != data.end())
	{
		string s = data.value("SCENE", "");
		if (s == "TITLE") scene = GameScene::TITLE;
		if (s == "GAME") scene = GameScene::GAME;
		if (s == "CG") scene = GameScene::CG;
		if (s == "OPTION") scene = GameScene::OPTION;
		if (s == "SELECT") scene = GameScene::SELECT;
	}

	if (!sceneLayer[scene])
		sceneLayer[scene] = new Layer();

	if (data.find("LAYER") != data.end())
	{
		for (auto layer = data["LAYER"].begin(); layer != data["LAYER"].end(); layer++)
		{
			int layerIndex = atoi(layer.key().c_str());
			for (auto element = layer.value().begin(); element != layer.value().end(); element++)
			{
				if (element.key() == "OBJECT")
				{
					for (auto object : element.value())
					{
						string shader = object.value("SHADER", "");
						string texture = object.value("TEXTURE", "");

						Object2D * obj = new Object2D("Game", texture, shader);
						obj->CreateBlueprint(BlueprintObjectType::IMAGE);
						obj->Load(object);

						if (object.find("BLUEPRINT") != object.end())
						{
							BlueprintList * bpList = obj->GetBlueprint();
							string bpOffset = object["BLUEPRINT"].value("OFFSET", "");
							bpList->SetOffset(Util::StringToVector3(bpOffset));
							bpList->Clear();

							vector<BlueprintLinkData> linkData;
							for (auto node : object["BLUEPRINT"]["LIST"])
							{
								string _bpPos = node["DATA"].value("POSITION", "");

								BlueprintType bpType = BPFunction::StringToType(node["DATA"].value("TYPE", ""));
								BlueprintSubType bpSubType = BPFunction::StringToSubType(node["DATA"].value("SUBTYPE", ""));

								BlueprintLinkData data;
								{
									if (node["DATA"].find("SUBDATA") != node["DATA"].end())
										for (auto subdata : node["DATA"]["SUBDATA"])
											data.subdata.push_back(subdata);
									data.data = node["DATA"].value("ID", 0);
									data.prev = node.value("PREV", 0);
									data.next = node.value("NEXT", 0);
								}

								Blueprint * bpCreate;
								if (bpType == BlueprintType::OBJECT)
								{
									bpCreate = new BP_Object();
									bpCreate->Load(node);
									static_cast<BP_Object*>(bpCreate)->SetOrigObject(obj);
									bpList->Add(bpCreate);
								}
								else if (bpType == BlueprintType::FLOAT)
								{
									bpCreate = new BP_Float();
									bpCreate->Load(node);
									bpList->Add(bpCreate);
								}
								else if (bpType == BlueprintType::FUNCTION)
								{
									bpCreate = new BP_Function();
									bpCreate->Load(node);
									bpList->Add(bpCreate);
								}
								else
									bpCreate = bpList->Add(bpType);

								bpCreate->SetID(data.data);
								bpCreate->GetObject2D()->SetPosition(Util::StringToVector3(_bpPos));
								bpCreate->SetSubType(bpSubType, data.subdata.size());

								linkData.push_back(data);
							}
							bpList->Link(linkData);
						}

						sceneLayer[scene]->AddObject(layerIndex, obj);
					}
				}
			}
		}
	}

	
}
