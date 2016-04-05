#include "VisibleObject.h"

VisibleObject::VisibleObject() 
	:parent(NULL), numberOfIndices(0)
{

}

void VisibleObject::addChild(VisibleObject* child) 
{
	children.push_back(child);

	child->parent = this;
}

glm::mat4 VisibleObject::getParentWorldTransform()
{
	// If parent is null return the identitity matrix
	if (parent == NULL) {
		return getLocalTransformation();
	}
	else {
		return parent->getParentWorldTransform() * parent->getLocalTransformation();
	}
}

glm::mat4 VisibleObject::getWorldTransformation() 
{
	return getParentWorldTransform() * modelingTransformation;
}

glm::mat4 VisibleObject::getLocalTransformation()
{
	return modelingTransformation;
}

void VisibleObject::draw() 
{
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->draw();
	}
}