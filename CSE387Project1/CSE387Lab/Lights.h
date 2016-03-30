#include "BasicIncludesAndDefines.h"
#include "VisibleObject.h"

#pragma once

enum LightNum { LightNum1 = 0, LightNum2, LightNum3};

class Lights : public VisibleObject
{
public:
	Lights();
	~Lights();

	static GLuint lightBuffer;
	static bool bufferMapped;
	string lightName;

	void draw(); // needed to include because of extending visible object

	void initialize();
	void setUniformIndex();
	void setOffsets();
	void setBuffer();

	void setAmbientColor(glm::vec4 ambientColor, LightNum lNum);
	void setDiffuseColor(glm::vec4 diffuseColor, LightNum lNum);
	void setSpecularColor(glm::vec4 specularColor, LightNum lNum);
	void setPositionOrDirection(glm::vec4 positionOrDirection, LightNum lNum);
	void setSpotDirection(glm::vec3 sportDirection, LightNum lNum);
	void setIsSpot(int isSpot, LightNum lNum);
	void setSpotCutoffCos(float spotCutoffCos, LightNum lNum);
	void setSpotExponent(float spotExponent, LightNum lNum);
	void setEnabled(int enabled, LightNum lNum);

	void setLightDefaults();

	static GLint blockIndex;
	static GLint blockSize;
	static GLint bindingPoint;

	glm::vec4 ambientColor[3];
	glm::vec4 diffuseColor[3];
	glm::vec4 specularColor[3];
	glm::vec4 positionOrDirection[3];
	glm::vec3 spotDirection[3];
	int isSpot[3];
	float spotCutoffCos[3];
	float spotExponent[3];
	int enabled[3];
	
	static GLint ambientColorLoc[3];
	static GLint diffuseColorLoc[3];
	static GLint specularColorLoc[3];
	static GLint positionOrDirectionLoc[3];
	static GLint spotDirectionLoc[3];
	static GLint isSpotLoc[3];
	static GLint spotCutoffCosLoc[3];
	static GLint spotExponentLoc[3];
	static GLint enabledLoc[3];
};