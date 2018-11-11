#pragma once
#include "../../Core/Components/GameObject.h"
#include "../../Core/json.hpp"

using namespace nlohmann;

class ContentObject : public GameObject
{
public:
	ContentObject();
	virtual ~ContentObject();

	virtual void		Update() override;
	virtual void		Render() override;
	virtual Texture2D *	GetTexture2D() override;

	void Save(json& data);
};

class NameObject : public GameObject
{
public:
	NameObject();
	virtual ~NameObject();

	virtual void		Update() override;
	virtual void		Render() override;
	virtual Texture2D *	GetTexture2D() override;

	void Save(json& data);
};