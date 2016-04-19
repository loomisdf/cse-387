#include "VisibleObject.h"

VisibleObject::VisibleObject()
	:parent(NULL), numberOfIndices(0),
	localTransformation(glm::mat4(1.0f)),
	scale(glm::mat4(1.0f))
{

}

VisibleObject::~VisibleObject()
{

	cout << "VisibleObject::~VisibleObject" << endl;

	for (VisibleObject* obj : children) {

		delete obj;
	}

	children.clear();

	glDeleteVertexArrays(1, &vertexArrayObject);

	glDeleteBuffers(1, &VBO);
	

};

void VisibleObject::addChild(VisibleObject* visibleObject)
{
	children.push_back(visibleObject);

	visibleObject->parent = this;

}

glm::mat4 VisibleObject::getParentWorldTransform()
{
	if (parent == NULL) {

		return getLocalTransformation();
	}
	else {

		return parent->getParentWorldTransform() * parent->getLocalTransformation();

	}

}

glm::mat4 VisibleObject::getLocalTransformation()
{

	return localTransformation;

}

glm::mat4 VisibleObject::getWorldTransformation() // Modeling trans
{
	return getParentWorldTransform() * localTransformation;

}

void VisibleObject::initialize()
{
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->initialize();
	}

	checkOpenGLErrors("VisibleObject::initialize");

}

void VisibleObject::draw()
{
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->draw();
	}

	checkOpenGLErrors("VisibleObject::draw");
}


bool VisibleObject::update(float deltaTime)
{

	for (Behavior* behavior : behaviors) {

		behavior->update(deltaTime);

	}

	size_t i = 0;

	while (i < children.size()) {

		if (!children[i]->update(deltaTime)) {

			delete children[i];

			children.erase(children.begin() + i);
		}
		else {
			i++;
		}

	}

	checkOpenGLErrors("VisibleObject::update");

	return true;
}

// Add a Behavior for this object and set the Behavior
// target to this object.
void VisibleObject::addBehavior(Behavior* behavior)
{
	behaviors.push_back(behavior);

	behavior->target = this;

} // end addBehavior

// Remove the Behavior object for this object
// Untested method
Behavior* VisibleObject::removeBehavior(Behavior* behavior)
{
	for (unsigned int i = 0; i < behaviors.size(); i++)
	{
		if (behaviors[i] == behavior) {

			behaviors.erase(behaviors.begin() + i);

			return behavior;
		}
	}

	return NULL; // Behaviors was not found

} // end removeBehavior