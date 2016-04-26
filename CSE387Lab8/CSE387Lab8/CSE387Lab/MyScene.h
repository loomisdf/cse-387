#pragma once

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h"
#include "Cube.h"
#include "Sphere.h"
#include "AssimpModel.h"
#include "SpinBehavior.h"
#include "OrbitBehavior.h"
#include "WaypointBehavior.h"
#include "Camera.h"

class MyScene : public VisibleObject
{
public:
	MyScene();

	~MyScene();

	virtual void initialize();

	virtual bool update(float deltaTime);

	void resize(int windowWidth, int windowHeight);

	void toggleLights(int light);

	void selectShader(int shaderProgramNumber);

	void setTextureMode(int mode);

	virtual void draw();

protected:

	GLuint perPixelShaderProgram;
};

