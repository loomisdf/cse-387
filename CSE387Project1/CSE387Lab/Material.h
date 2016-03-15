#pragma once

#include "BasicIncludesAndDefines.h"

struct Material
{
	Material()
	{
		setDefaultProperties();
	}

	Material(GLuint shaderProgram) {
		setDefaultProperties();
		setUniformIndex(shaderProgram);
		setOffsets(shaderProgram);
		setBuffer();
		setShaderMaterialProperties();
	}

	void setUniformIndex(GLuint shaderProgram) {
		blockIndex = glGetUniformBlockIndex(shaderProgram, "MaterialBlock");

		glGetActiveUniformBlockiv(shaderProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);
	}

	void setOffsets(GLuint shaderProgram) {
		GLuint uniformIndeces[6] = { 0 };
		GLint uniformOffsets[6] = { 0 };

		const GLchar * charStringNames[] = {
			"object.ambientMat",
			"object.diffuseMat",
			"object.specularMat",
			"object.emissiveMat",
			"object.specularExp",
			"object.textureMode"
		};

		glGetUniformIndices(shaderProgram, 6, (const GLchar **)charStringNames, uniformIndeces);

		glGetActiveUniformsiv(shaderProgram, 6, uniformIndeces, GL_UNIFORM_OFFSET, uniformOffsets);

		cout << "uniform offsets for material object" << endl;
		for (int i = 0; i < 6; i++) {
			cout << uniformOffsets[i] << endl;
		}

		this->ambientMatLoc = uniformOffsets[0];
		this->diffuseMatLoc = uniformOffsets[1];
		this->specularMatLoc = uniformOffsets[2];
		this->emissiveMatLoc = uniformOffsets[3];
		this->specularExpMatLoc = uniformOffsets[4];
		this->textureMappedLoc = uniformOffsets[5];
	}

	void setBuffer() {
		glGenBuffers(1, &buffer);

		glBindBuffer(GL_UNIFORM_BUFFER, buffer);

		glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
	}

	void setDefaultProperties()
	{
		ambientMat = glm::vec4( 0.75f, 0.75f, 0.75f, 1.0f );
		diffuseMat = glm::vec4( 0.75f, 0.75f, 0.75f, 1.0f );
		specularMat = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		specularExpMat = 64.0f;
		emissiveMat = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
		textureMode = NO_TEXTURE;
	}

	void setShaderMaterialProperties()
	{
		setAmbientMat( ambientMat );
		setDiffuseMat( diffuseMat );
		setSpecularMat( specularMat );
		setSpecularExponentMat( specularExpMat );
		setEmissiveMat( emissiveMat );
		setTextureMapped(textureMode);

		if ( textureMode != NO_TEXTURE ) {
			glBindTexture(GL_TEXTURE_2D, textureObject);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void setAmbientMat( glm::vec4 ambientMat )
	{
		this->ambientMat = ambientMat;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, ambientMatLoc, sizeof(glm::vec4), glm::value_ptr(ambientMat));
	}

	void setDiffuseMat( glm::vec4 diffuseMat )
	{
		this->diffuseMat = diffuseMat;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseMatLoc, sizeof(glm::vec4), glm::value_ptr(diffuseMat));
	}

	void setSpecularMat( glm::vec4 specularMat )
	{
		this->specularMat = specularMat;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, specularMatLoc, sizeof(glm::vec4), glm::value_ptr(specularMat));
	}

	void setSpecularExponentMat( float specularExpMat )
	{
		this->specularExpMat = specularExpMat;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, specularExpMatLoc, sizeof(float), &specularExpMat);
	}

	void setEmissiveMat( glm::vec4 emissiveMat )
	{
		this->emissiveMat = emissiveMat;
		
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, emissiveMatLoc, sizeof(glm::vec4), glm::value_ptr(emissiveMat));
	}

	void setAmbientAndDiffuseMat( glm::vec4 objectColor )
	{
		setAmbientMat( objectColor );
		setDiffuseMat( objectColor );
	}
	
	TextureMode getTextureMapped() { return textureMode; }

	void setTextureMapped(int textureM)
	{
		this->textureMode = textureMode;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, textureMappedLoc, sizeof(int), &textureM);
	}

	bool setupTexture(string textureFileName, TextureMode textureMode = REPLACE_AMBIENT_DIFFUSE )
	{
		CBitmap image;

		if( image.loadBMP((const char *)textureFileName.c_str()) == false ) {

			cerr << "Unable to load texture!" << endl;
			return false;
		}

		this->textureMode = textureMode;

		glGenTextures(1, &textureObject);
		glBindTexture( GL_TEXTURE_2D, textureObject );

		glTexImage2D(	GL_TEXTURE_2D, 0,
							image.getChannels(), 
							image.getWidth(), image.getHeight(), 0,
							GL_BGR, GL_UNSIGNED_BYTE, image.getLinePtr(0) );

		glGenerateMipmap(GL_TEXTURE_2D);
	
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		return true;

	} // end setupTexture

	protected: 

		GLuint buffer;

		GLint blockIndex;
		GLint blockSize;
		GLint bindingPoint = 3;

		GLint ambientMatLoc;
		glm::vec4 ambientMat;

		GLint diffuseMatLoc;
		glm::vec4 diffuseMat;
		
		GLint specularMatLoc;
		glm::vec4 specularMat;
		
		GLint specularExpMatLoc;
		float specularExpMat;
		
		GLint emissiveMatLoc;
		glm::vec4 emissiveMat;

		GLint textureMappedLoc;
		TextureMode textureMode;

		// For texture mapping
		GLuint textureObject;
};