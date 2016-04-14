#pragma once

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h"
#include "Cube.h"
#include "Sphere.h"
#include "AssimpModel.h"

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

	void detachMoon();

	void reparentNewPlanet();

protected:

	GLuint perPixelShaderProgram;

	Cube cube;

	Sphere newPlanet;

	Sphere sphere;
	glm::mat4 sunTrans;

	Sphere moon;
	glm::mat4 moonTrans;

	Sphere earth;
	glm::mat4 earthTrans;

	glm::mat4 modelTrans;
	AssimpModel model;
};

