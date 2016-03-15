// Targeting version 4.1 of GLSL. If the compiler does not support 
// 4.1 it will cause an error.
#version 430 core


in vec4 vertexColor;
out vec4 fragmentColor;

void main()
{
	fragmentColor = vertexColor;

}