#pragma once
#include "Behavior.h"
class OrbitBehavior :
	public Behavior
{
public:
	OrbitBehavior(
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat radius = 1.0f,
		GLfloat rate = 1.0f);

	~OrbitBehavior();

	virtual void update(float elapsedTimeSeconds);

protected:
	GLfloat angle;

	glm::vec3 axis;

	GLfloat radius;

	GLfloat rate;
};

