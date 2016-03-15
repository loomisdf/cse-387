#pragma once

#include "VisibleObject.h"

class Cube : public VisibleObject
{
public:
	
	Cube();

	~Cube();

	void initialize();

	void draw();

	glm::vec3 rotation;
};

