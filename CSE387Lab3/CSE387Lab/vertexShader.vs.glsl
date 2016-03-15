// Targeting version 4.2 of GLSL. If the compiler does not support 
// 4.3 it will cause an error.

#version 430 core

layout(shared) uniform sceneTransformBlock 
{
	mat4 viewingMatrix; 
	mat4 projectionMatrix; 
	

} sceneTransform;

uniform mat4 modelMatrix;

out vec4 vertexWorldPosition;
out vec4 vertexWorldNormal;

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;

void main()
{
	// Calculate the position in clip coordinates
     gl_Position = sceneTransform.projectionMatrix * sceneTransform.viewingMatrix * modelMatrix * vertexPosition;

	 // Calculate the vertex position in world coordinates
	vertexWorldPosition = modelMatrix * vertexPosition;

}