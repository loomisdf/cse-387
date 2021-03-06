#include "VisibleObject.h"

VisibleObject::VisibleObject()
	:parent(NULL), numberOfIndices(0), localTransformation(glm::mat4(1.0f))
{

}

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

void VisibleObject::draw()
{
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->draw();
	}

	checkOpenGLErrors("VisibleObject::draw");
}

void VisibleObject::initialize() {
	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->initialize();
	}
}

bool VisibleObject::update(float deltatime) {
	for (int i = children.size() - 1; i >= 0; i--) {
		if (!children[i]->update(deltatime)) {
			delete children[i];
			children.erase(children.begin() + i);
		}
	}
	return true;
}

VisibleObject* VisibleObject::detachFromParent() {
	for (unsigned int i = 0; i < parent->children.size(); i++) {
		if (parent->children[i] == this) {
			parent->children.erase(parent->children.begin() + i);
			this->parent = NULL;
			return this;
		}
	}
	return NULL;
}

bool VisibleObject::detachAndDeleteChild(VisibleObject* child) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			delete children[i];
			return true;
		}
	}
	return false;
}

void VisibleObject::reparent(VisibleObject* newChild) {
	glm::mat4 oldWorldTransform = newChild->getWorldTransformation();
	newChild->detachFromParent();
	newChild->localTransformation = glm::inverse(this->getWorldTransformation()) * oldWorldTransform;
	this->addChild(newChild);
}