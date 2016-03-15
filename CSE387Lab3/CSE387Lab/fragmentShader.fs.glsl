// Targeting version 4.3 of GLSL. 
#version 430 core


layout (location = 17) uniform vec4 uColor;

out vec4 fragmentColor;

in vec4 vertexWorldPosition;

void main()
{
	fragmentColor = uColor;

}