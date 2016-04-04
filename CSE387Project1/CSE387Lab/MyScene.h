#pragma once

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h"
#include "Cube.h"
#include "Sphere.h"
#include "Lights.h"
#include "TransformBlock.h"

enum ShaderProgram {PerVertex = 0, PerPixel};

class MyScene : public VisibleObject
{
public:
	MyScene();

	~MyScene();

	void initialize();

	void draw();

	void update(float deltaTime);

	void resize(int windowWidth, int windowHeight);

	void enableLight(string lightName);

	void setShader(ShaderProgram shader);

	void setTextureMode(TextureMode textureMode);

	TransformBlock transformBlock;

	SharedMaterialProperties sharedMaterialPropterties;

protected:

	GLuint projectionMatrixLocation;
	glm::mat4 projectionTransformation;

	GLuint viewingMatrixLocation;
	glm::mat4 viewingTransformation;
	
	Cube cube;
	glm::mat4 cubeTrans;

	Sphere sphere;
	glm::mat4 sunTrans;

	Sphere moon;
	glm::mat4 moonTrans;

	Sphere earth;
	glm::mat4 earthTrans;

	Lights lights;

	GLuint shaderProgram_PV;
	GLuint shaderProgram_PP;
};