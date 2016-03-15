#pragma once

#include "VisibleObject.h"

class Sphere : 	public VisibleObject
{

public:

	Sphere(float Radius = 1.0f, int slices = 16, int stacks = 16);

	~Sphere();

	void initialize();

	void draw();

protected:

	void initializeSphereTop();
	void initializeSphereBody();
	void initializeSphereBottom();

	vector<pntVertexData> v; // vertex positions

	vector<unsigned int> indices; // indices

	GLuint currentIndex;

	float radius;
	int stacks;
	int slices;

	// Step in z and radius as stacks are drawn.
	double z0, z1;
	double r0, r1;

	float stackStep;
	float sliceStep;

};

