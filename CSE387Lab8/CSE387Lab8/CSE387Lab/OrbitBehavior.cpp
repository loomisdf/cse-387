#include "OrbitBehavior.h"


OrbitBehavior::OrbitBehavior(glm::vec3 axis, GLfloat radius, GLfloat rate)
	:axis(axis), radius(radius), rate(rate), angle(0)
{
}

OrbitBehavior::~OrbitBehavior()
{
}

void OrbitBehavior::update(float elapsedTimeSeconds) {
	angle += elapsedTimeSeconds * rate;

	glm::mat4 rotation = glm::rotate(angle, axis);

	glm::mat4 translation = glm::translate(glm::vec3(radius, 0.0f, 0.0f));

	target->setLocalTransformation(rotation * translation);
}
