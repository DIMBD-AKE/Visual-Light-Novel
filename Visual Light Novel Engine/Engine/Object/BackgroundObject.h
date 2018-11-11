#pragma once
#include "../../Core/Components/GameObject.h"
#include "../../Core/json.hpp"

using namespace nlohmann;

class BackgroundObject : public GameObject
{
public:
	BackgroundObject();
	virtual ~BackgroundObject();

	virtual void		Update() override;
	virtual void		Render() override;
	virtual Texture2D *	GetTexture2D() override;

	void Save(json& data);
};

