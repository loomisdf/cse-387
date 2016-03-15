#pragma once

#include "BasicIncludesAndDefines.h"

class VisibleObject
{

public:

	VisibleObject() :numberOfIndices(0){};
	~VisibleObject() {};

	// Set the shader program for this object
	void setShader(GLuint shaderProgram) { this->shaderProgram = shaderProgram;  }

	// Sets the texture object for this object
	void setTexture(GLuint textureObject) { this->textureObject = textureObject; }

	// Reads the texture from a file and creates an associated texture object
	GLuint createTextureObject(string textureFileName);

	// Set the modeling transformation that will be used when this object is rendered
	void setModelingTransformation(glm::mat4 modelingTransformation){ this->modelingTransformation = modelingTransformation; }

	// Initializes this object
	virtual void initialize() = 0;

	// "Renders" this object
	virtual void draw() = 0;

	// Material properties of the object (public so that is can be adjusted
	// outside the class)
	Material material;

protected: 

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	GLuint textureObject;

	int numberOfIndices;

	glm::mat4 modelingTransformation;

};

