// Targeting version 4.2 of GLSL. 
#version 420 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 
precision mediump float;

out vec4 fragmentColor;

in vec4 color0;
in vec2 TexCoord0;

// Create a to sampler
uniform sampler2D gSampler;

uniform int textureMode;

void main()
{
	// Use the texture coordinate and the sampler to get a color value
	//fragmentColor = texture2D(gSampler, TexCoord0.st);

	fragmentColor = color0;
}