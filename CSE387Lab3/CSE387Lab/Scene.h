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

	void resize();

protected:

	void initializeVertexData();

	void initializeUniformData();

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	GLuint viewLocation;

	GLuint projectionLocation;

	GLuint modelLocation;

	GLuint colorLocation;

	int numberOfVertices;

	const GLuint sceneTransformBlockBindingPoint;
};

