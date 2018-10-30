#include "../../stdafx.h"
#include "Resources.h"


Resources::Resources()
{
}


Resources::~Resources()
{
}

void Resources::Destroy()
{
	for (auto type : resources)
		for (auto data : type.second)
			SAFE_DELETE(data.second);
}
