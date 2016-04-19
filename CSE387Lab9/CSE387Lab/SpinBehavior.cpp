#include "SpinBehavior.h"


SpinBehavior::SpinBehavior(glm::vec3 position, glm::vec3 axis, GLfloat rate)
	: position(position), axis(axis), rate(rate), angle(0)
{

}




void SpinBehavior::update(float elapsedTimeSeconds)
{
	cout << "SpinBehavior::update "<< endl;

	angle += elapsedTimeSeconds * rate;

	glm::mat4 rotation = glm::rotate(angle, axis);

	glm::mat4 positionTranslate = glm::translate(position);

	target->setLocalTransformation(positionTranslate * rotation);
}
