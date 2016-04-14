#pragma once

#include "BasicIncludesAndDefines.h"
#include "Behavior.h"

class VisibleObject
{

public:

	friend class Behavior;

	VisibleObject();
	~VisibleObject();

	// Set the shader program for this object
	void setShader(GLuint shaderProgram) { this->shaderProgram = shaderProgram;  }

	// Set the modeling transformation that will be used when this object is rendered
	void setLocalTransformation(glm::mat4 localTrans)
	{ this->localTransformation = localTrans; }

	// Initializes this object
	virtual void initialize() = 0;

	// "Renders" this object
	virtual void draw();
	 
	// Allows Behaviors to update this object
	virtual bool update(float deltaTime); 

	void addChild(VisibleObject* visibleObject);

	VisibleObject* detachFromParent();

	glm::mat4 getParentWorldTransform();

	glm::mat4 getLocalTransformation();

	glm::mat4 getWorldTransformation();

	void reparent(VisibleObject* newChild);
	
	// Add a Behavior to this VisibleObject
	virtual void addBehavior(Behavior* behavior);

	// Remove a Behavior object for this VisibleObject
	virtual Behavior* removeBehavior(Behavior* behavior);
	
	// Material properties of the object (public so that is can be adjusted
	// outside the class)
	Material material;

	glm::mat4 scale;

protected: 

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	GLuint VBO;

	int numberOfIndices;

	glm::mat4 localTransformation;

	vector<VisibleObject*> children;

	vector<Behavior*> behaviors;

	VisibleObject* parent;

};

