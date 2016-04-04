#pragma once

#include "BasicIncludesAndDefines.h"

struct OldMaterial
{
protected: 

	static GLuint buffer;
	static bool bufferMapped;

	static GLint blockIndex;
	static GLint blockSize;
	GLint bindingPoint = 3;

	static GLint ambientMatLoc;
	glm::vec4 ambientMat;

	static GLint diffuseMatLoc;
	glm::vec4 diffuseMat;
	
	static GLint specularMatLoc;
	glm::vec4 specularMat;
	
	static GLint specularExpMatLoc;
	float specularExpMat;
	
	static GLint emissiveMatLoc;
	glm::vec4 emissiveMat;

	static GLint textureMappedLoc;
	TextureMode textureMode;

	// For texture mapping
	GLuint textureObject;

public:
	OldMaterial();

	OldMaterial(GLuint shaderProgram);

	void setUniformIndex(GLuint shaderProgram);

	void setOffsets(GLuint shaderProgram);

	void setBuffer();

	void setDefaultProperties();

	void setShaderMaterialProperties();

	void setAmbientMat(glm::vec4 ambientMat);

	void setDiffuseMat(glm::vec4 diffuseMat);

	void setSpecularMat(glm::vec4 specularMat);

	void setSpecularExponentMat(float specularExpMat);

	void setEmissiveMat(glm::vec4 emissiveMat);

	void setAmbientAndDiffuseMat(glm::vec4 objectColor);
	
	TextureMode getTextureMapped();

	void setTextureMapped(int textureM);

	bool setupTexture(string textureFileName, TextureMode textureMode);
};