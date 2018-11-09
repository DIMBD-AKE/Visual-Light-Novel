#pragma once
#include "Blueprint.h"

class BP_UI : public Blueprint
{
public:
	BP_UI();
	virtual ~BP_UI();

	virtual void Execute(void * data = nullptr) override;

	virtual void SubUpdate() override;
};

