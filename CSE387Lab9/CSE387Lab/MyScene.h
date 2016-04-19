#pragma once

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h"
#include "Cube.h"
#include "Sphere.h"
#include "Model.h"
#include "SpinBehavior.h"


class MyScene : public VisibleObject
{
public:
	MyScene();

	~MyScene();

	virtual void initialize();

	void resize(int windowWidth, int windowHeight);

	void toggleLights(int light);

	void selectShader(int shaderProgramNumber);

	void setTextureMode(int mode);

protected:

	GLuint perPixelShaderProgram;

	Cube cube;

	Sphere sphere;
	glm::mat4 sunTrans;

	Sphere moon;
	glm::mat4 moonTrans;

	Sphere earth;
	glm::mat4 earthTrans;

	//Model model;
	glm::mat4 modelTrans;

};

