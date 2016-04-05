#include "VisbleObjectIndexedRender.h"


VisbleObjectIndexedRender::VisbleObjectIndexedRender()
{
}


VisbleObjectIndexedRender::~VisbleObjectIndexedRender()
{
}

void VisbleObjectIndexedRender::draw()
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

	VisibleObject::draw();

} // end draw
