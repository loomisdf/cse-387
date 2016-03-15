// Targeting version 4.2 of GLSL. If the compiler does not support 
// 4.2 it will cause an error.
#version 420 core

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix;

out vec4 vsColor;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;

void main()
{
	// Calculate the position in clip coordinates
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);

	 vsColor = vertexColor;

}

