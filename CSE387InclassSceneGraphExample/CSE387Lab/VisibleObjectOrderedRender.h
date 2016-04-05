#pragma once
#include "VisibleObject.h"
class VisibleObjectOrderedRender :
	public VisibleObject
{
public:
	VisibleObjectOrderedRender();
	~VisibleObjectOrderedRender();

	void draw();
};

