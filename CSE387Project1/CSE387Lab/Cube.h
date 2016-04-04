#pragma once

#include "VisibleObject.h"

#include "SharedMaterialProperties.h"

class Cube : public VisibleObject
{
public:
	
	Cube();

	~Cube();

	void initialize();

	void draw();
};

