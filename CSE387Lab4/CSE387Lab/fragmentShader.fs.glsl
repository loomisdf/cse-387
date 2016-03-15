// Targeting version 4.2 of GLSL. 
#version 420 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 
precision mediump float;

out vec4 fragmentColor;

// Create an "in" variable for the texture coordinates
in vec2 vertexTexCoord0;

// Create a 2D sampler
uniform sampler2D gSampler;

void main()
{
	// Use the texture coordinate and the sampler to get a color value
	fragmentColor = texture2D(gSampler, vertexTexCoord0);

}