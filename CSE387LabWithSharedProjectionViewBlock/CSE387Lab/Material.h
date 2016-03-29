#pragma once

#include "BasicIncludesAndDefines.h"

struct Material
{
	Material()
	{
		setDefaultProperties();
	}


	Material( GLint ambientMatLoc,  GLint diffuseMatLoc, 
			  GLint specularMatLoc, GLint specularExpMatLoc, 
			  GLint emissiveMatLoc, GLint textureMappedLoc =  0xFFFFFFFF )
	{
		this->ambientMatLoc = ambientMatLoc;  
		this->diffuseMatLoc =  diffuseMatLoc;
		this->specularMatLoc = specularMatLoc;
		this->specularExpMatLoc = specularExpMatLoc;
		this->emissiveMatLoc = emissiveMatLoc;
		this->textureMappedLoc = textureMappedLoc;

		setDefaultProperties();	
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

	void setUpMaterial( GLint ambientMatLoc = 0xFFFFFFFF,  GLint diffuseMatLoc = 0xFFFFFFFF, 
						GLint specularMatLoc = 0xFFFFFFFF, GLint specularExpMatLoc= 0xFFFFFFFF, 
						GLint emissiveMatLoc = 0xFFFFFFFF, GLint textureMappedLoc =  0xFFFFFFFF )
	{
		this->ambientMatLoc = ambientMatLoc;  
		this->diffuseMatLoc =  diffuseMatLoc;
		this->specularMatLoc = specularMatLoc;
		this->specularExpMatLoc = specularExpMatLoc;
		this->emissiveMatLoc = emissiveMatLoc;
		this->textureMappedLoc = textureMappedLoc;
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
		if (ambientMatLoc != 0xFFFFFFFF) {
			glUniform4fv( ambientMatLoc, 1, value_ptr(ambientMat));
		}
		else {
			cout << "ambient material location not set." << endl;
		}
	}

	void setDiffuseMat( glm::vec4 diffuseMat )
	{
		this->diffuseMat = diffuseMat;
		if (diffuseMatLoc != 0xFFFFFFFF) {
			glUniform4fv( diffuseMatLoc, 1, value_ptr(diffuseMat));
		}
		else {
			cout << "Diffuse material location not set." << endl;
		}
	}

	void setSpecularMat( glm::vec4 specularMat )
	{
		this->specularMat = specularMat;
		if (specularMatLoc != 0xFFFFFFFF) {
			glUniform4fv( specularMatLoc, 1, value_ptr(specularMat) );
		}
		else {
			cout << "Specular material location not set." << endl;
		}
	}

	void setSpecularExponentMat( float specularExpMat )
	{
		this->specularExpMat = specularExpMat;
		if (specularExpMatLoc != 0xFFFFFFFF) {
			glUniform1f( specularExpMatLoc, specularExpMat );

		}
		else {
			cout << "Specular exponent material location not set." << endl;
		}
	}

	void setEmissiveMat( glm::vec4 emissiveMat )
	{
		this->emissiveMat = emissiveMat;
		if (emissiveMatLoc != 0xFFFFFFFF) {
			glUniform4fv( emissiveMatLoc, 1, value_ptr(emissiveMat) );
		}
		else {
			cout << "Emmissive material location not set." << endl;
		}
	}

	void setAmbientAndDiffuseMat( glm::vec4 objectColor )
	{
		setAmbientMat( objectColor );
		setDiffuseMat( objectColor );

	}
	
	TextureMode getTextureMapped() { return textureMode; }


	void setTextureMapped(TextureMode textureMode)
	{
		this->textureMode = textureMode;
		if (textureMappedLoc != 0xFFFFFFFF) {
			glUniform1i(textureMappedLoc, textureMode);
		}
		else {
//			cout << "Texture mapped enabled location not set." << endl;
		}
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