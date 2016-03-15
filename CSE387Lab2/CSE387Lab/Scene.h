#pragma once

#include "BasicIncludesAndDefines.h"

#include "BuildShader.h"


class Scene
{
public:
	Scene();

	~Scene();

	void initialize();

	void draw();

	void resize(int width, int height);

protected:

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	int numberOfIndices;
};

