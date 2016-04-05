#include "VisibleObjectOrderedRender.h"


VisibleObjectOrderedRender::VisibleObjectOrderedRender()
{
}


VisibleObjectOrderedRender::~VisibleObjectOrderedRender()
{
}

void VisibleObjectOrderedRender::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Set the modeling transformation
	SharedProjectionAndViewing::setModelingMatrix(modelingTransformation);

	// Set the material properties and bind the texture object
	material.setShaderMaterialProperties();

	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, numberOfIndices);

	VisibleObject::draw();
}
