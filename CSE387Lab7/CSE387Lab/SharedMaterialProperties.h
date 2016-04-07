#pragma once

#include "BasicIncludesAndDefines.h"

#define materialBlockBindingPoint 12

struct Material
{
	Material()
	{
		setDefaultProperties();
	}

	void setDefaultProperties()
	{
		ambientMat = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		diffuseMat = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		specularMat = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		specularExpMat = 64.0f;
		emissiveMat = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		textureMode = 0;
	}

	void setAmbientMat(glm::vec4 ambientMat)
	{
		this->ambientMat = ambientMat;
	}

	void setDiffuseMat(glm::vec4 diffuseMat)
	{
		this->diffuseMat = diffuseMat;
	}

	void setSpecularMat(glm::vec4 specularMat)
	{
		this->specularMat = specularMat;
	}

	void setSpecularExponentMat(float specularExpMat)
	{
		this->specularExpMat = specularExpMat;
	}

	void setEmissiveMat(glm::vec4 emissiveMat)
	{
		this->emissiveMat = emissiveMat;
	}

	void setAmbientAndDiffuseMat(glm::vec4 objectColor)
	{
		setAmbientMat(objectColor);
		setDiffuseMat(objectColor);

	}

	void setTextureMapped(TextureMode textureMode)
	{
		switch (textureMode) {

		case NO_TEXTURE:
			this->textureMode = 0;
			break;
		case DECAL:
			this->textureMode = 1;
			break;
		case REPLACE_AMBIENT_DIFFUSE:
			this->textureMode = 2;
			break;
		default:
			cerr << "Illegal texture mode" << endl;
			break;
		}

		//this->textureMode = ;
	}

	bool setupTexture(string textureFileName, TextureMode textureMode = REPLACE_AMBIENT_DIFFUSE)
	{
		CBitmap image;

		if (image.loadBMP((const char *)textureFileName.c_str()) == false) {

			cerr << "Unable to load texture!" << endl;
			return false;
		}
		else {

			cout << "Loaded " << textureFileName.c_str() << endl;
		}

		setTextureMapped(textureMode);
		//this->textureMode = textureMode;

		glGenTextures(1, &textureObject);
		glBindTexture(GL_TEXTURE_2D, textureObject);

		glTexImage2D(GL_TEXTURE_2D, 0,
			image.getChannels(),
			image.getWidth(), image.getHeight(), 0,
			GL_BGR, GL_UNSIGNED_BYTE, image.getLinePtr(0));

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return true;

	} // end setupTexture

	glm::vec4 ambientMat;

	glm::vec4 diffuseMat;

	glm::vec4 specularMat;

	float specularExpMat;

	glm::vec4 emissiveMat;

	int textureMode;

	// For texture mapping
	GLuint textureObject;
};



class SharedMaterialProperties
{
public:

	// Should be called for each shader program that includes the
	// projectionViewBlock and worldEyeBlock uniform blocks.
	static void setUniformBlockForShader(GLuint shaderProgram);

	// Call the set the material properties in the shader before 
	// rendering the object.
	static void setShaderMaterialProperties(Material material);

protected:

	// Finds the byte offsets of the variables in the two uniform 
	// blocks.
	static void findOffsets(GLuint shaderProgram);

	// Creates the buffers and binds them to the binding points.
	static void allocateBuffers(GLuint shaderProgram);

	// Determines the sizes in bytes of the blocks and binds them to the binding points. 
	static void determineBlockSizeSetBindingPoint(GLuint shaderProgram);

	static GLuint SharedMaterialProperties::ambientMatLocation; // Byte offset of the projection matrix

	static GLuint SharedMaterialProperties::diffuseMatLocation; // Byte offset of the viewing matrix

	static GLuint SharedMaterialProperties::specularMatLocation; // Byte offset of the modeling matrix

	static GLuint SharedMaterialProperties::emmissiveMatLocation; // Byte offset of the modeling matrix to transform normal vectors

	static GLuint SharedMaterialProperties::specularExpLocation;  // Byte offset of the eye position

	static GLuint SharedMaterialProperties::textureModeLocation;  // Byte offset of the eye position

	static GLuint SharedMaterialProperties::materialBlockBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
	static GLint SharedMaterialProperties::materialBlockSize; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
	static GLuint SharedMaterialProperties::materialBlockIndex; // Identifier for the uniform block.

	static bool SharedMaterialProperties::blockSizeAndOffetsSet; // Indicates whether or not the buffer for the blocks has been setup

	const static string SharedMaterialProperties::materialBlockName;


};

