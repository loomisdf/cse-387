// Targeting version 4.2 of GLSL. 
#version 330 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 
precision mediump float;

out vec4 fragmentColor;

in vec4 color0;

void main()
{
	fragmentColor = color0;
}