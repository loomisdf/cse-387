// Targeting version 4.1 of GLSL. 
#version 420 core


in vec4 vsColor; 
out vec4 fragmentColor;

void main()
{
	fragmentColor = vsColor;

}