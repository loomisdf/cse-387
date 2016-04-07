#include "SharedMaterialProperties.h"

GLuint SharedMaterialProperties::ambientMatLocation; // Byte offset of the projection matrix

GLuint SharedMaterialProperties::diffuseMatLocation; // Byte offset of the viewing matrix

GLuint SharedMaterialProperties::specularMatLocation; // Byte offset of the modeling matrix

GLuint SharedMaterialProperties::emmissiveMatLocation; // Byte offset of the modeling matrix to transform normal vectors

GLuint SharedMaterialProperties::specularExpLocation;  // Byte offset of the eye position

GLuint SharedMaterialProperties::textureModeLocation;  // Byte offset of the eye position

GLuint SharedMaterialProperties::materialBlockBuffer; // Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
GLint SharedMaterialProperties::materialBlockSize = 0; // Size in bytes of both the buffer and the uniform blocks in all the the shaders. 
GLuint SharedMaterialProperties::materialBlockIndex; // Identifier for the uniform block.

bool SharedMaterialProperties::blockSizeAndOffetsSet = false; // Indicates whether or not the buffer for the blocks has been setup

const string SharedMaterialProperties::materialBlockName = "MaterialBlock";

bool checkLocFound(const GLchar* locationName, GLuint indice)
{
	if (indice == GL_INVALID_INDEX) {
		cout << locationName << " not found in shader." << endl;

		return false;
	}
	else {

		//cout << locationName << " index is " <<  indice << endl; 
		return true;
	}

} // end checkLocationFound


void SharedMaterialProperties::setUniformBlockForShader(GLuint shaderProgram)
{
	// Determine the size of the block and set the binding point for the block(s)
	determineBlockSizeSetBindingPoint(shaderProgram);

	// Has the buffer been created and have the byte offset been found?
	if (blockSizeAndOffetsSet == false) {

		// Set up the buffers and bind to binding points
		allocateBuffers(shaderProgram);

		// Find the byte offsets of the uniform block variables
		findOffsets(shaderProgram);
	}

	checkOpenGLErrors("setUniformBlockForShader");

} // end setUniformBlockForShader


void SharedMaterialProperties::allocateBuffers(GLuint shaderProgram)
{
	if (materialBlockSize > 0) {

		// Get an identifier for a buffer
		glGenBuffers(1, &materialBlockBuffer);

		// Bind the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, materialBlockBuffer);

		// Allocate the buffer. Does not load data. Note the use of NULL where the data would normally be.
		// Data is not loaded because the above struct will not be byte alined with the uniform block.
		glBufferData(GL_UNIFORM_BUFFER, materialBlockSize, NULL, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point to be the same as the uniform in the shader(s). In this case 1.
		glBindBufferBase(GL_UNIFORM_BUFFER, materialBlockBindingPoint, materialBlockBuffer);

		blockSizeAndOffetsSet = true;
	}

	checkOpenGLErrors("findOffets");

} // end allocateBuffers


void SharedMaterialProperties::findOffsets(GLuint shaderProgram)
{
	const int numberOfNames = 6;

	GLuint uniformIndices[numberOfNames] = { 0 };
	GLint uniformOffsets[numberOfNames] = { 0 };

	const GLchar * charStringNames[] = { "object.ambientMat", "object.diffuseMat", "object.specularMat", 
										 "object.emmissiveMat", "object.specularExp", "object.textureMode" };

	glGetUniformIndices(shaderProgram, numberOfNames, (const GLchar **)charStringNames, uniformIndices);

	if (uniformIndices[0] != GL_INVALID_INDEX && uniformIndices[1] != GL_INVALID_INDEX) {
		for (int i = 0; i < numberOfNames; i++) {

			checkLocFound(charStringNames[i], uniformIndices[i]);
		}

		//Get the offsets of the uniforms. The offsets in the buffer will be the same.
		glGetActiveUniformsiv(shaderProgram, numberOfNames, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

		for (int i = 0; i < numberOfNames; i++) {

			cout << '\t' << charStringNames[i] << " offset is " << uniformOffsets[i] << endl;
		}

		// Save locations
		ambientMatLocation = uniformOffsets[0];
		diffuseMatLocation = uniformOffsets[1];
		specularMatLocation = uniformOffsets[2];
		emmissiveMatLocation = uniformOffsets[3];
		specularExpLocation = uniformOffsets[4];
		textureModeLocation = uniformOffsets[5];
	}
	else {

		cout << " Did not find names in " << materialBlockName.c_str() << endl;
	}

	checkOpenGLErrors("findOffets");

} // findOffsets


void SharedMaterialProperties::determineBlockSizeSetBindingPoint(GLuint shaderProgram)
{
	// Get the index of the "materialBlock"
	materialBlockIndex = glGetUniformBlockIndex(shaderProgram, materialBlockName.c_str());

	cout << materialBlockName.c_str() << " index is " << materialBlockIndex << endl;

	if (checkLocFound(materialBlockName.c_str(), materialBlockIndex)) {

		// Determine the size in bytes of the uniform block.
		glGetActiveUniformBlockiv(shaderProgram, materialBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &materialBlockSize);
		cout << materialBlockName.c_str() << " size is " << materialBlockSize << endl;

		// Assign the block to a binding point.
		glUniformBlockBinding(shaderProgram, materialBlockIndex, materialBlockBindingPoint);
	}

	checkOpenGLErrors("findBlockSizeSetBindingPoint");

} // end determineBlockSizeSetBindingPoint


void SharedMaterialProperties::setShaderMaterialProperties(Material material)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, materialBlockBuffer);

	glBufferSubData(GL_UNIFORM_BUFFER, ambientMatLocation, sizeof(glm::vec4), glm::value_ptr(material.ambientMat));
	glBufferSubData(GL_UNIFORM_BUFFER, diffuseMatLocation, sizeof(glm::vec4), glm::value_ptr(material.diffuseMat));
	glBufferSubData(GL_UNIFORM_BUFFER, specularMatLocation, sizeof(glm::vec4), glm::value_ptr(material.specularMat));
	glBufferSubData(GL_UNIFORM_BUFFER, emmissiveMatLocation, sizeof(glm::vec4), glm::value_ptr(material.emissiveMat));
	glBufferSubData(GL_UNIFORM_BUFFER, specularExpLocation, sizeof(float), &material.specularExpMat);
	glBufferSubData(GL_UNIFORM_BUFFER, textureModeLocation, sizeof(int), &material.textureMode);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Set for 2D texture mapping
	if (material.textureMode != NO_TEXTURE) {

		glBindTexture(GL_TEXTURE_2D, material.textureObject);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, 0);
	}

} // end setShaderMaterialProperties
