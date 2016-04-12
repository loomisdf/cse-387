#pragma once

#include "BasicIncludesAndDefines.h"

class VisibleObject
{

public:

	VisibleObject();
	~VisibleObject() {};

	// Set the shader program for this object
	void setShader(GLuint shaderProgram) { this->shaderProgram = shaderProgram;  }

	// Set the modeling transformation that will be used when this object is rendered
	void setLocalTransformation(glm::mat4 localTrans)
	{ this->localTransformation = localTrans; }

	// Initializes this object
	virtual void initialize() = 0;

	// "Renders" this object
	virtual void draw();

	virtual bool update(float deltaTime); 

	void addChild(VisibleObject* visibleObject);

	VisibleObject* detachFromParent();

	bool detachAndDeleteChild(VisibleObject* child);

	glm::mat4 getParentWorldTransform();

	glm::mat4 getLocalTransformation();

	glm::mat4 getWorldTransformation();

	void reparent(VisibleObject* newChild);
	
	// Material properties of the object (public so that is can be adjusted
	// outside the class)
	Material material;

protected: 

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	int numberOfIndices;

	glm::mat4 localTransformation;

	vector<VisibleObject*> children;

	VisibleObject* parent;

};

