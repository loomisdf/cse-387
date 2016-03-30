#include "Lights.h"

GLint Lights::blockIndex;
GLint Lights::blockSize;
GLint Lights::bindingPoint = 2;

GLuint Lights::lightBuffer;
bool Lights::bufferMapped = false;

GLint Lights::ambientColorLoc[3];
GLint Lights::diffuseColorLoc[3];
GLint Lights::specularColorLoc[3];
GLint Lights::positionOrDirectionLoc[3];
GLint Lights::spotDirectionLoc[3];
GLint Lights::isSpotLoc[3];
GLint Lights::spotCutoffCosLoc[3];
GLint Lights::spotExponentLoc[3];
GLint Lights::enabledLoc[3];

Lights::Lights()
{
	// Set light defaults
	ambientColor[0] = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
	diffuseColor[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	specularColor[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	positionOrDirection[0] = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f);
	spotDirection[0] = glm::vec3(0.0f, -1.0f, 0.0f);
	isSpot[0] =	0; 
	spotCutoffCos[0] = glm::cos(glm::radians(10.0f)); 
	spotExponent[0] = 0.9f; 
	enabled[0] = 1;

	ambientColor[1] = glm::vec4(0.15f, 0.15f, 0.15, 1.0f);
	diffuseColor[1] = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
	specularColor[1] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	positionOrDirection[1] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	spotDirection[1] = glm::vec3(0.0f, -1.0f, 0.0f);
	isSpot[1] = 0; 
	spotCutoffCos[1] = glm::cos(glm::radians(10.0f));
	spotExponent[1] = 0.9f;
	enabled[1] = 1;

	ambientColor[2] = glm::vec4(0.15f, 0.15f, 0.15, 1.0f);
	diffuseColor[2] = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	specularColor[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	positionOrDirection[2] = glm::vec4(0.0f, 0.0f, 12.0f, 1.0f);
	spotDirection[2] = glm::vec3(0.0f, 0.0f, -1.0f);
	isSpot[2] = 1;
	spotCutoffCos[2] = glm::cos(glm::radians(10.0f));
	spotExponent[2] = 0.9f;
	enabled[2] = 1;
}


Lights::~Lights()
{
}

void Lights::initialize() {
	setUniformIndex();
	setOffsets();
	if (!bufferMapped) {
		bufferMapped = true;
		setBuffer();
	}

	
}

void Lights::setUniformIndex() {
	blockIndex = glGetUniformBlockIndex(shaderProgram, "LightBlock");

	glGetActiveUniformBlockiv(shaderProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);
}

void Lights::setOffsets() {
	GLuint uniformIndeces[9] = { 0 };
	GLint uniformOffsets[9] = { 0 };

	const GLchar * charStringNames0[] = { 
		"light1.ambientColor",
		"light1.diffuseColor",
		"light1.specularColor",
		"light1.positionOrDirection",
		"light1.spotDirection",
		"light1.isSpot",
		"light1.spotCutoffCos",
		"light1.spotExponent",
		"light1.enabled"
	};

	const GLchar * charStringNames1[] = { 
		"light2.ambientColor",
		"light2.diffuseColor",
		"light2.specularColor",
		"light2.positionOrDirection",
		"light2.spotDirection",
		"light2.isSpot",
		"light2.spotCutoffCos",
		"light2.spotExponent",
		"light2.enabled"
	};

	const GLchar * charStringNames2[] = { 
		"light3.ambientColor",
		"light3.diffuseColor",
		"light3.specularColor",
		"light3.positionOrDirection",
		"light3.spotDirection",
		"light3.isSpot",
		"light3.spotCutoffCos",
		"light3.spotExponent",
		"light3.enabled"
	};

	glGetUniformIndices(shaderProgram, 9, (const GLchar **)charStringNames0, uniformIndeces);

	glGetActiveUniformsiv(shaderProgram, 9, uniformIndeces, GL_UNIFORM_OFFSET, uniformOffsets);

	

	ambientColorLoc[0] = uniformOffsets[0];
	diffuseColorLoc[0] = uniformOffsets[1];
	specularColorLoc[0] = uniformOffsets[2];
	positionOrDirectionLoc[0] = uniformOffsets[3];
	spotDirectionLoc[0] = uniformOffsets[4];
	isSpotLoc[0] = uniformOffsets[5];
	spotCutoffCosLoc[0] = uniformOffsets[6];
	spotExponentLoc[0] = uniformOffsets[7];
	enabledLoc[0] = uniformOffsets[8];



	// Set offsets for light 2
	glGetUniformIndices(shaderProgram, 9, (const GLchar **)charStringNames1, uniformIndeces);

	glGetActiveUniformsiv(shaderProgram, 9, uniformIndeces, GL_UNIFORM_OFFSET, uniformOffsets);

	/*cout << "uniform offsets for light 2" << endl;
	for (int i = 0; i < 9; i++) {
		cout << uniformOffsets[i] << endl;
	}*/

	ambientColorLoc[1] = uniformOffsets[0];
	diffuseColorLoc[1] = uniformOffsets[1];
	specularColorLoc[1] = uniformOffsets[2];
	positionOrDirectionLoc[1] = uniformOffsets[3];
	spotDirectionLoc[1] = uniformOffsets[4];
	isSpotLoc[1] = uniformOffsets[5];
	spotCutoffCosLoc[1] = uniformOffsets[6];
	spotExponentLoc[1] = uniformOffsets[7];
	enabledLoc[1] = uniformOffsets[8];


	// Set offsets for light 3
	glGetUniformIndices(shaderProgram, 9, (const GLchar **)charStringNames2, uniformIndeces);

	glGetActiveUniformsiv(shaderProgram, 9, uniformIndeces, GL_UNIFORM_OFFSET, uniformOffsets);

	/*cout << "uniform offsets for light 3" << endl;
	for (int i = 0; i < 9; i++) {
		cout << uniformOffsets[i] << endl;
	}*/

	ambientColorLoc[2] = uniformOffsets[0];
	diffuseColorLoc[2] = uniformOffsets[1];
	specularColorLoc[2] = uniformOffsets[2];
	positionOrDirectionLoc[2] = uniformOffsets[3];
	spotDirectionLoc[2] = uniformOffsets[4];
	isSpotLoc[2] = uniformOffsets[5];
	spotCutoffCosLoc[2] = uniformOffsets[6];
	spotExponentLoc[2] = uniformOffsets[7];
	enabledLoc[2] = uniformOffsets[8];
}

void Lights::setBuffer() {
	glGenBuffers(1, &lightBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);

	cout << "Light Buffer ID " << lightBuffer << endl;

	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, lightBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
}

void Lights::setLightDefaults() {
	// Set values for light 1
	setAmbientColor(ambientColor[0], LightNum1);
	setDiffuseColor(diffuseColor[0], LightNum1);
	setSpecularColor(specularColor[0], LightNum1);
	setPositionOrDirection(positionOrDirection[0], LightNum1);
	setSpotDirection(spotDirection[0], LightNum1);
	setIsSpot(isSpot[0], LightNum1);
	setSpotCutoffCos(spotCutoffCos[0], LightNum1);
	setSpotExponent(spotExponent[0], LightNum1);
	setEnabled(enabled[0], LightNum1);

	// Set values for light 2
	setAmbientColor(ambientColor[1], LightNum2);
	setDiffuseColor(diffuseColor[1], LightNum2);
	setSpecularColor(specularColor[1], LightNum2);
	setPositionOrDirection(positionOrDirection[1], LightNum2);
	setSpotDirection(spotDirection[1], LightNum2);
	setIsSpot(isSpot[1], LightNum2);
	setSpotCutoffCos(spotCutoffCos[1], LightNum2);
	setSpotExponent(spotExponent[1], LightNum2);
	setEnabled(enabled[1], LightNum2);

	// Set values for light 3
	setAmbientColor(ambientColor[2], LightNum3);
	setDiffuseColor(diffuseColor[2], LightNum3);
	setSpecularColor(specularColor[2], LightNum3);
	setPositionOrDirection(positionOrDirection[2], LightNum3);
	setSpotDirection(spotDirection[2], LightNum3);
	setIsSpot(isSpot[2], LightNum3);
	setSpotCutoffCos(spotCutoffCos[2], LightNum3);
	setSpotExponent(spotExponent[2], LightNum3);
	setEnabled(enabled[2], LightNum3);
}

void Lights::setAmbientColor(glm::vec4 ambientColor, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->ambientColor[0] = ambientColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, ambientColorLoc[0], sizeof(glm::vec4), glm::value_ptr(ambientColor));
		break;

	case(1):
		this->ambientColor[1] = ambientColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, ambientColorLoc[1], sizeof(glm::vec4), glm::value_ptr(ambientColor));
		break;

	case(2):
		this->ambientColor[2] = ambientColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, ambientColorLoc[2], sizeof(glm::vec4), glm::value_ptr(ambientColor));
		break;
	}
}

void Lights::setDiffuseColor(glm::vec4 diffuseColor, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->diffuseColor[0] = diffuseColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseColorLoc[0], sizeof(glm::vec4), glm::value_ptr(diffuseColor));
		break;

	case(1):
		this->diffuseColor[1] = diffuseColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseColorLoc[1], sizeof(glm::vec4), glm::value_ptr(diffuseColor));
		break;

	case(2):
		this->diffuseColor[2] = diffuseColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseColorLoc[2], sizeof(glm::vec4), glm::value_ptr(diffuseColor));
		break;
	}
}

void Lights::setSpecularColor(glm::vec4 specularColor, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->specularColor[0] = specularColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, specularColorLoc[0], sizeof(glm::vec4), glm::value_ptr(specularColor));
		break;

	case(1):
		this->specularColor[1] = specularColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, specularColorLoc[1], sizeof(glm::vec4), glm::value_ptr(specularColor));
		break;

	case(2):
		this->specularColor[2] = specularColor;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, specularColorLoc[2], sizeof(glm::vec4), glm::value_ptr(specularColor));
		break;
	}
}

void Lights::setPositionOrDirection(glm::vec4 positionOrDirection, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->positionOrDirection[0] = positionOrDirection;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, positionOrDirectionLoc[0], sizeof(glm::vec4), glm::value_ptr(positionOrDirection));
		break;

	case(1):
		this->positionOrDirection[1] = positionOrDirection;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, positionOrDirectionLoc[1], sizeof(glm::vec4), glm::value_ptr(positionOrDirection));
		break;

	case(2):
		this->positionOrDirection[2] = positionOrDirection;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, positionOrDirectionLoc[2], sizeof(glm::vec4), glm::value_ptr(positionOrDirection));
		break;
	}
}

void Lights::setSpotDirection(glm::vec3 spotDirection, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->spotDirection[0] = spotDirection;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotDirectionLoc[0], sizeof(glm::vec3), glm::value_ptr(spotDirection));
		break;

	case(1):
		this->spotDirection[1] = spotDirection;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotDirectionLoc[1], sizeof(glm::vec3), glm::value_ptr(spotDirection));
		break;
	}
}

void Lights::setIsSpot(int isSpot, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->isSpot[0] = isSpot;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, isSpotLoc[0], sizeof(int), &isSpot);
		break;

	case(1):
		this->isSpot[1] = isSpot;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, isSpotLoc[1], sizeof(int), &isSpot);
		break;

	case(2):
		this->isSpot[2] = isSpot;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, isSpotLoc[2], sizeof(int), &isSpot);
		break;
	}
}

void Lights::setSpotCutoffCos(float spotCutoffCos, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->spotCutoffCos[0] = spotCutoffCos;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotCutoffCosLoc[0], sizeof(float), &spotCutoffCos);
		break;

	case(1):
		this->spotCutoffCos[1] = spotCutoffCos;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotCutoffCosLoc[1], sizeof(float), &spotCutoffCos);
		break;

	case(2):
		this->spotCutoffCos[2] = spotCutoffCos;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotCutoffCosLoc[2], sizeof(float), &spotCutoffCos);
		break;
	}
}

void Lights::setSpotExponent(float spotExponent, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->spotExponent[0] = spotExponent;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotExponentLoc[0], sizeof(float), &spotExponent);
		break;

	case(1):
		this->spotExponent[1] = spotExponent;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotExponentLoc[1], sizeof(float), &spotExponent);
		break;

	case(2):
		this->spotExponent[2] = spotExponent;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, spotExponentLoc[2], sizeof(float), &spotExponent);
		break;
	}
}

void Lights::setEnabled(int enabled, LightNum lNum) {
	switch (lNum) {
	case(0):
		this->enabled[0] = enabled;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, enabledLoc[0], sizeof(int), &enabled);
		break;

	case(1):
		this->enabled[1] = enabled;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, enabledLoc[1], sizeof(int), &enabled);
		break;

	case(2):
		this->enabled[2] = enabled;
		glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, enabledLoc[2], sizeof(int), &enabled);
		break;
	}
}

void Lights::draw() {
}