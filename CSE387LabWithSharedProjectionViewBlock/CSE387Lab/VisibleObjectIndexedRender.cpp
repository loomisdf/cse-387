#include "VisibleObjectIndexedRender.h"


VisibleObjectIndexedRender::VisibleObjectIndexedRender()
{
}


VisibleObjectIndexedRender::~VisibleObjectIndexedRender()
{
}

void VisibleObjectIndexedRender::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Set the modeling transformation
	SharedProjectionAndViewing::setModelingMatrix(modelingTransformation);

	// Set the material properties and bind the texture object
	material.setShaderMaterialProperties();

	// Draw the object
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

} // end draw