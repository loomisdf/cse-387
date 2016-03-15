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

	void resize(int windowWidth, int windowHeight);

protected:

	GLuint projectionMatrixLocation;
	glm::mat4 projectionTransformation;

	GLuint viewingMatrixLocation;
	glm::mat4 viewingTransformation;
	
	Cube cube;

	Sphere sphere;
};

