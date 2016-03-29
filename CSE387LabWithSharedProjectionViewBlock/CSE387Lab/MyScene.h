#pragma once

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h"
#include "Cube.h"
#include "Sphere.h"

class MyScene : public VisibleObject
{
public:
	MyScene();

	~MyScene();

	void initialize();

	void draw();

	void update(float deltaTime);

	void resize(int windowWidth, int windowHeight);

protected:

	Cube cube;

	Sphere sphere;
	glm::mat4 sunTrans;

	Sphere moon;
	glm::mat4 moonTrans;

	Sphere earth;
	glm::mat4 earthTrans;
};

