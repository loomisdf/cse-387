#pragma once
#include "VisibleObject.h"
class VisbleObjectIndexedRender :
	public VisibleObject
{
public:
	VisbleObjectIndexedRender();
	~VisbleObjectIndexedRender();

	virtual void draw();

	GLuint IBO;
};

