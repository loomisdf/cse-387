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
	SharedProjectionAndViewing::setModelingMatrix(getWorldTransformation() * scale);

	// Set the material properties and bind the texture object
	SharedMaterialProperties::setShaderMaterialProperties(shaderProgram, material);

	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, numberOfIndices);

	// Unbind the vertex array object
	glBindVertexArray(0);

	// Draw the kids
	VisibleObject::draw();
}
