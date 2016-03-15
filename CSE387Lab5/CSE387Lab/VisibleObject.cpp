#include "VisibleObject.h"

GLuint VisibleObject::createTextureObject(string textureFileName)
{
	CBitmap image;

	GLuint localTextureObject; // Texture object to be returned

	assert(image.loadBMP((const char *)textureFileName.c_str()) != false);

	// Make the texture object
	glGenTextures(1, &localTextureObject);
	glBindTexture(GL_TEXTURE_2D, localTextureObject);

	glTexImage2D(GL_TEXTURE_2D, 0, image.getChannels(), image.getWidth(), image.getHeight(), 0,
		GL_BGR, GL_UNSIGNED_BYTE, image.getLinePtr(0));

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set the filtering mode for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set the wrap mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return localTextureObject;

}