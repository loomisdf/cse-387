// Targeting version 4.2 of GLSL. 
#version 330 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 


layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 normalModelMatrix;
	mat4 viewingMatrix;
	mat4 projectionMatrix;
};

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

out vec4 vertexWorldPosition;
out vec4 vertexWorldNormal;
out vec2 TexCoord0;

void main()
{
    // Calculate the position in clip coordinates
    gl_Position = projectionMatrix * viewingMatrix * modelMatrix * vertexPosition;

	vertexWorldPosition = modelMatrix * vertexPosition;

	vertexWorldNormal = normalModelMatrix * vec4(vertexNormal, 1.0);

	TexCoord0 = vertexTexCoord; 

} // end main

