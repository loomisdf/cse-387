#pragma once

#include "BasicIncludesAndDefines.h"

#include "BuildShader.h"

#include "VisibleObject.h"
#include "Cube.h"


class MyScene : public VisibleObject
{
public:
	MyScene();

	~MyScene();

	void initialize();

	void draw();

	void resize();

	void setRotation(glm::vec3 rotation);

protected:

	GLuint projectionMatrixLocation;
	glm::mat4 projectionTransformation;

	GLuint viewingMatrixLocation;
	glm::mat4 viewingTransformation;
	
	Cube cube;

};

