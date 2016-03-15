#pragma once

#include "BasicIncludesAndDefines.h"

class VisibleObject
{

public:

	VisibleObject(){};
	~VisibleObject() {};

	void setShader(GLuint shaderProgram) { this->shaderProgram = shaderProgram;  }

	virtual void initialize() = 0;

	virtual void draw() = 0;

protected: 

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	GLuint textureObject;

	int numberOfIndices;

};

