#pragma once

#include "VisibleObjectOrderedRender.h"

class Cube : public VisibleObjectOrderedRender
{
public:
	
	Cube();

	~Cube();

	virtual void initialize();

	virtual bool update(float deltaTime);

};

