// Targeting version 4.2 of GLSL. 
#version 420 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 

uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec4 vertexWorldPosition;
out vec4 vertexWorldNormal;
out vec2 vertexTexCoord0;

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

// Create an "out" variable for the texture coordinates
// TODO

void main()
{
     // Calculate the position in clip coordinates
     gl_Position = projectionMatrix * viewingMatrix * modelMatrix * vertexPosition;

	 // Copy the incoming texture coordinate to the output
	 vertexTexCoord0 = vertexTexCoord;

}

