#include "BasicIncludesAndDefines.h"
#pragma once
class TransformBlock
{
public:
	TransformBlock();
	~TransformBlock();

	void setShader(GLuint shaderProgram);

	GLuint shaderProgram;

	GLint transformBlockIndex;
	GLint transformBlockSize;
	GLint transformBlockBindingPoint = 4;

	GLint modelMatrixLoc;
	//GLint normalModelMatrixLoc;
	GLint viewingMatrixLoc;
	GLint projectionMatrixLoc;

	GLuint transformBlockBuffer;

	void setModelMatrix(glm::mat4 modelMatrix);
	//void setnNormalModelMatrix(glm::mat4 normalModelMatrix);
	void setViewingMatrix(glm::mat4 viewingMatrix);
	void setProjectionMatrix(glm::mat4 projectionMatrix);

	void initialize();

	static bool bufferMade;
};

