#include "../stdafx.h"
#include "EngineIO.h"
#include "Layer.h"
#include "../Scenes/SC_Editor.h"
#include <fstream>
#include <iomanip>
#include "../Core/dirent.h"
#include "Object/Object.h"
#include "Blueprint/Blueprint.h"
#include "Blueprint/BP_Sequence.h"
#include "Blueprint/BP_Object.h"
#include "Blueprint/BP_Float.h"
#include "Blueprint/BP_Function.h"
#include "Blueprint/BP_UI.h"
#include "../Core/Util.h"


EngineIO::EngineIO()
{
}


EngineIO::~EngineIO()
{
}

void EngineIO::ProjectSave()
{
	auto sceneLayer = static_cast<SC_Editor*>(SCENE->GetScene("Editor"))->GetSceneLayer();
	CreateDirectory("Project", NULL);
	CreateDirectory("Project/Scenes", NULL);
	CreateDirectory("Project/Elements", NULL);
	CreateDirectory("Project/Objects", NULL);
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
				if (dynamic_cast<Object2D*>(layer.second[i]))
					dynamic_cast<Object2D*>(layer.second[i])->Save(data, layer.first, i);
				if (dynamic_cast<ElementObject*>(layer.second[i]))
					dynamic_cast<ElementObject*>(layer.second[i])->Save();
				if (dynamic_cast<UIObject*>(layer.second[i]))
					dynamic_cast<UIObject*>(layer.second[i])->Save(data, i);
			}
		}

		ofstream stream("Project/Scenes/" + sceneName + ".json");
		stream << setw(2) << data << endl;
	}
}

void EngineIO::ProjectLoad()
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
				LoadScene("Project/Scenes/" + fileName, sceneLayer);
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

void EngineIO::LoadScene(string path, OUT map<GameScene, Layer*>& sceneLayer)
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
							LoadBlueprint(object["BLUEPRINT"], bpList, obj);
						}

						sceneLayer[scene]->AddObject(layerIndex, obj);
					}
				}
			}
		}
	}

	if (data.find("UI") != data.end())
	{
		for (auto ui : data["UI"])
		{
			string texDefault = ui.value("TEXTURE_DEFAULT", "");
			string texOver = ui.value("TEXTURE_OVER", "");
			string texPress = ui.value("TEXTURE_PRESS", "");

			UIObject * uiObject = new UIObject(UIObjectType::SINGLE, "Game", texDefault, texOver, texPress);
			uiObject->CreateBlueprint(BlueprintObjectType::UIObject);
			uiObject->Load(ui);

			if (ui.find("BLUEPRINT") != ui.end())
			{
				BlueprintList * bpList = uiObject->GetBlueprint();
				LoadBlueprint(ui["BLUEPRINT"], bpList, uiObject);
			}

			sceneLayer[scene]->AddObject(-10, uiObject);
		}
	}
}

void EngineIO::LoadElement(string path, OUT IElement ** element)
{
	ifstream stream(path);
	json data;
	stream >> data;

	ElementType type = IElement::StringToType(data.value("TYPE", ""));

	if (type == ElementType::NONE) return;

	multimap<wstring, string> stateMap;
	for (auto state : data["STATE"])
	{
		wstring stateName = Util::S2W(state.value("NAME", ""));
		string path = state.value("PATH", "");
		stateMap.insert(make_pair(stateName, path));
	}

	if (type == ElementType::CHARACTER)
	{
		wstring name = Util::S2W(data.value("NAME", ""));
		wstring _explain = Util::S2W(data.value("EXPLAIN", ""));
		vector<wstring> explain;
		int pos;
		while ((pos = _explain.find(L"\r")) != wstring::npos)
		{
			wstring tok = _explain.substr(0, pos);
			explain.push_back(tok);
			_explain.erase(0, pos + 2);
		}

		Character * chr = new Character();
		chr->Name = name;
		chr->Explain = explain;
		chr->StateImage = stateMap;
		*element = chr;
		return;
	}

	if (type == ElementType::STUFF)
	{
		wstring name = Util::S2W(data.value("NAME", ""));
		wstring _explain = Util::S2W(data.value("EXPLAIN", ""));
		vector<wstring> explain;
		int pos;
		while ((pos = _explain.find(L"\r")) != wstring::npos)
		{
			wstring tok = _explain.substr(0, pos);
			explain.push_back(tok);
			_explain.erase(0, pos + 2);
		}

		Stuff * stuff = new Stuff();
		stuff->Name = name;
		stuff->Explain = explain;
		stuff->StateImage = stateMap;
		*element = stuff;
		return;
	}

	if (type == ElementType::BACKGROUND)
	{
		Background * bg = new Background();
		bg->StateImage = stateMap;
		*element = bg;
		return;
	}
}

void EngineIO::LoadBlueprint(json & data, BlueprintList * bpList, GameObject * orig)
{
	string bpOffset = data.value("OFFSET", "");
	bpList->SetOffset(Util::StringToVector3(bpOffset));
	bpList->Clear();

	vector<BlueprintLinkData> linkData;
	for (auto node : data["LIST"])
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
			static_cast<BP_Object*>(bpCreate)->SetOrigObject(orig);
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
		else if (bpType == BlueprintType::UI)
		{
			bpCreate = new BP_UI();
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

string EngineIO::OpenFile(FileType type, string title)
{
	char szFile[256];
	szFile[0] = '\0';

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = nullptr;
	if (type == FileType::IMAGE)
		ofn.lpstrFilter = "Image (*.png, *.jpg)\0*.png;*.jpg";
	if (type == FileType::JSON)
		ofn.lpstrFilter = "Json (*.json)\0*.json";
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrInitialDir = ".\\";
	ofn.lpstrTitle = title.c_str();

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