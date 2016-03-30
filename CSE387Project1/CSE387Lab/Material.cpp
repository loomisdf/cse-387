#include "Material.h"

bool Material::bufferMapped = false;
GLuint Material::buffer;

GLint Material::blockIndex;
GLint Material::blockSize;

GLint Material::ambientMatLoc;
GLint Material::diffuseMatLoc;
GLint Material::specularMatLoc;
GLint Material::specularExpMatLoc;
GLint Material::emissiveMatLoc;
GLint Material::textureMappedLoc;

Material::Material() {
	setDefaultProperties();
}

Material::Material(GLuint shaderProgram) {
	setDefaultProperties();
	setUniformIndex(shaderProgram);
	setOffsets(shaderProgram);
	if (!bufferMapped) {
		bufferMapped = true;
		setBuffer();
	}
	setShaderMaterialProperties();
}

void Material::setUniformIndex(GLuint shaderProgram) {
	blockIndex = glGetUniformBlockIndex(shaderProgram, "MaterialBlock");

	glGetActiveUniformBlockiv(shaderProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);
}

void Material::setOffsets(GLuint shaderProgram) {
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

void Material::setBuffer() {
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
}

void Material::setDefaultProperties()
{
	ambientMat = glm::vec4( 0.75f, 0.75f, 0.75f, 1.0f );
	diffuseMat = glm::vec4( 0.75f, 0.75f, 0.75f, 1.0f );
	specularMat = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
	specularExpMat = 64.0f;
	emissiveMat = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	textureMode = NO_TEXTURE;
}

void Material::setShaderMaterialProperties()
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

void Material::setAmbientMat( glm::vec4 ambientMat )
{
	this->ambientMat = ambientMat;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, ambientMatLoc, sizeof(glm::vec4), glm::value_ptr(ambientMat));
}

void Material::setDiffuseMat( glm::vec4 diffuseMat )
{
	this->diffuseMat = diffuseMat;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, diffuseMatLoc, sizeof(glm::vec4), glm::value_ptr(diffuseMat));
}

void Material::setSpecularMat( glm::vec4 specularMat )
{
	this->specularMat = specularMat;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, specularMatLoc, sizeof(glm::vec4), glm::value_ptr(specularMat));
}

void Material::setSpecularExponentMat( float specularExpMat )
{
	this->specularExpMat = specularExpMat;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, specularExpMatLoc, sizeof(float), &specularExpMat);
}

void Material::setEmissiveMat( glm::vec4 emissiveMat )
{
	this->emissiveMat = emissiveMat;
	
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, emissiveMatLoc, sizeof(glm::vec4), glm::value_ptr(emissiveMat));
}

void Material::setAmbientAndDiffuseMat( glm::vec4 objectColor )
{
	setAmbientMat( objectColor );
	setDiffuseMat( objectColor );
}

TextureMode Material::getTextureMapped() 
{ 
	return textureMode; 
}

void Material::setTextureMapped(int textureM)
{
	this->textureMode = textureMode;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, textureMappedLoc, sizeof(int), &textureM);
}

bool Material::setupTexture(string textureFileName, TextureMode textureMode = REPLACE_AMBIENT_DIFFUSE)
{
	CBitmap image;

	if (image.loadBMP((const char *)textureFileName.c_str()) == false) {

		cerr << "Unable to load texture!" << endl;
		return false;
	}

	this->textureMode = textureMode;

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