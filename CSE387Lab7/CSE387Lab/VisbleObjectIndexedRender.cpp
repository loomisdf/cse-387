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
	SharedProjectionAndViewing::setModelingMatrix(getWorldTransformation());

	// Set the material properties and bind the texture object
	// material.setShaderMaterialProperties();
	SharedMaterialProperties::setShaderMaterialProperties(material);
	
	// Draw the object
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw the kids
	VisibleObject::draw();

} // end draw
