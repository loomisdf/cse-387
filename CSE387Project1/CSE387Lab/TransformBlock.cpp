#include "TransformBlock.h"


TransformBlock::TransformBlock()
{
}


TransformBlock::~TransformBlock()
{
}

void TransformBlock::setShader(GLuint shaderProgram) {
	this->shaderProgram = shaderProgram;
}

void TransformBlock::initialize() {
	transformBlockIndex = glGetUniformBlockIndex(shaderProgram, "TransformBlock");

	glGetActiveUniformBlockiv(shaderProgram, transformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &transformBlockSize);

	glUniformBlockBinding(shaderProgram, transformBlockIndex, transformBlockBindingPoint);

	GLuint uniformIndeces[3] = { 0 };
	GLint uniformOffsets[3] = { 0 };

	const GLchar * charStringNames[] = {
		"TransformBlock.modelMatrix",
		"TransformBlock.viewingMatrix",
		"TransformBlock.projectionMatrix",
	};

	// Get the indeces of the uniforms.
	glGetUniformIndices(shaderProgram, 3, (const GLchar **)charStringNames, uniformIndeces);

	// Get the offsets of the uniforms
	glGetActiveUniformsiv(shaderProgram, 3, uniformIndeces, GL_UNIFORM_OFFSET, uniformOffsets);

	cout << "uniform transformblock" << endl;
	for (int i = 0; i < 3; i++) {
		cout << uniformOffsets[i] << endl;
	}

	modelMatrixLoc = uniformOffsets[0];
	viewingMatrixLoc = uniformOffsets[1];
	projectionMatrixLoc = uniformOffsets[2];

	glGenBuffers(1, &transformBlockBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, transformBlockBuffer);

	glBufferData(GL_UNIFORM_BUFFER, transformBlockSize, NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, transformBlockBindingPoint, transformBlockBuffer);
}

void TransformBlock::setModelMatrix(glm::mat4 modelMatrix) {
	glBindBuffer(GL_UNIFORM_BUFFER, transformBlockBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, modelMatrixLoc, sizeof(glm::mat4), glm::value_ptr(modelMatrix));
}
void TransformBlock::setViewingMatrix(glm::mat4 viewingMatrix) {
	glBindBuffer(GL_UNIFORM_BUFFER, transformBlockBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, viewingMatrixLoc, sizeof(glm::mat4), glm::value_ptr(viewingMatrix));
}
void TransformBlock::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glBindBuffer(GL_UNIFORM_BUFFER, transformBlockBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, projectionMatrixLoc, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
}
