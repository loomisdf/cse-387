#include "SharedMaterialProperties.h"

GLint SharedMaterialProperties::bindingPoint = 3;

static bool blockSizeAndOffetsSet = false; // Indicates whether or not the buffer for the blocks has been setup

static GLuint materialBlockIndex;
static GLuint materialBuffer;
static GLint materialBlockSize = 0;

static GLint ambientMatLoc;
static GLint diffuseMatLoc;
static GLint specularMatLoc;
static GLint emissiveMatLoc;
static GLint specularExpLoc;
static GLint textureModeLoc;

const static string materialBlockName = "MaterialBlock";

void SharedMaterialProperties::setShaderMaterialProperties(Material material)  {
	if (materialBlockSize > 0) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);

		// set ambient
		glBufferSubData(GL_UNIFORM_BUFFER, ambientMatLoc, sizeof(glm::mat4), glm::value_ptr(material.ambientMat));

		// set diffuse
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseMatLoc, sizeof(glm::mat4), glm::value_ptr(material.diffuseMat));

		// set specularMat
		glBufferSubData(GL_UNIFORM_BUFFER, specularMatLoc, sizeof(glm::mat4), glm::value_ptr(material.specularMat));

		// set emissiveMat
		glBufferSubData(GL_UNIFORM_BUFFER, emissiveMatLoc, sizeof(glm::mat4), glm::value_ptr(material.emissiveMat));

		// set specularExp
		glBufferSubData(GL_UNIFORM_BUFFER, specularExpLoc, sizeof(float), &material.specularExpMat);

		// set textureMode
		glBufferSubData(GL_UNIFORM_BUFFER, textureModeLoc, sizeof(float), &material.textureMode);

	}
}

void SharedMaterialProperties::setUniformBlockForShader(GLuint shaderProgram) {
	// Determine the size of the block and set the binding point for the block(s)
	determineBlockSizeSetBindingPoint(shaderProgram);

	// Has the buffer been creates and have the byte offset been found?
	if (blockSizeAndOffetsSet == false) {

		// Set up the buffers and bind to binding points
		allocateBuffers(shaderProgram);

		// Find the byte offsets of the uniform block variables
		findOffsets(shaderProgram);
	}
}

bool checkLocationFound(const GLchar* locationName, GLuint indice)
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

void SharedMaterialProperties::determineBlockSizeSetBindingPoint(GLuint shaderProgram) {
	// Get the index of the "projectionViewBlock"
	materialBlockIndex = glGetUniformBlockIndex(shaderProgram, materialBlockName.c_str());

	cout << materialBlockName.c_str() << " index is " << materialBlockIndex << endl;

	if (checkLocationFound(materialBlockName.c_str(), materialBlockIndex)) {

		// Determine the size in bytes of the uniform block.
		glGetActiveUniformBlockiv(shaderProgram, materialBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &materialBlockSize);
		cout << materialBlockName.c_str() << " size is " << materialBlockSize << endl;

		// Assign the block to a binding point. In this case 2.
		glUniformBlockBinding(shaderProgram, materialBlockIndex, bindingPoint);
	}
	
}

void SharedMaterialProperties::allocateBuffers(GLuint shaderProgram) {
	if (materialBlockSize > 0) {

		// Get an identifier for a buffer
		glGenBuffers(1, &materialBuffer);

		// Bind the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);

		// Allocate the buffer. Does not load data. Note the use of NULL where the data would normally be.
		// Data is not loaded because the above struct will not be byte alined with the uniform block.
		glBufferData(GL_UNIFORM_BUFFER, materialBlockSize, NULL, GL_DYNAMIC_DRAW);

		// Assign the buffer to a binding point to be the same as the uniform in the shader(s). In this case 1.
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, materialBuffer);

		blockSizeAndOffetsSet = true;
	}
}

void SharedMaterialProperties::findOffsets(GLuint shaderProgram) {

	const int numberOfNames = 6;

	GLuint uniformIndices[numberOfNames] = { 0 };
	GLint uniformOffsets[numberOfNames] = { 0 };

	const GLchar * charStringNames[] = {
		"object.ambientMat",
		"object.diffuseMat",
		"object.specularMat",
		"object.emissiveMat",
		"object.specularExp",
		"object.textureMode"
	};

	glGetUniformIndices(shaderProgram, numberOfNames, (const GLchar **)charStringNames, uniformIndices);

	if (uniformIndices[0] != GL_INVALID_INDEX && uniformIndices[1] != GL_INVALID_INDEX) {
		for (int i = 0; i < numberOfNames; i++) {

			checkLocationFound(charStringNames[i], uniformIndices[i]);
		}

		//Get the offsets of the uniforms. The offsets in the buffer will be the same.
		glGetActiveUniformsiv(shaderProgram, numberOfNames, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

		for (int i = 0; i < numberOfNames; i++) {

			cout << '\t' << charStringNames[i] << " offset is " << uniformOffsets[i] << endl;
		}

		// Save locations
		ambientMatLoc = uniformOffsets[0];
		diffuseMatLoc = uniformOffsets[1];
		specularMatLoc = uniformOffsets[2];
		emissiveMatLoc = uniformOffsets[3];
		specularExpLoc = uniformOffsets[4];
		textureModeLoc = uniformOffsets[5];

	}
	else {

		cout << " Did not find names in " << materialBlockName.c_str() << endl;
	}
}