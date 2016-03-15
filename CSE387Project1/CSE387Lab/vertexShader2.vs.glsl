// Targeting version 4.2 of GLSL. 
#version 420 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 
layout(shared) uniform TransformBlock
{
	mat4 modelMatrix;
	mat4 viewingMatrix;
	mat4 projectionMatrix;
} transformBlock;

/*uniform*/ mat4 normalModelMatrix; // *new added commented out uniform as a reminder

out vec4 vertexWorldPosition;
out vec4 vertexWorldNormal;
out vec3 eyePosition;
out vec2 TexCoord0;

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

void main()
{
	normalModelMatrix = transpose(inverse(transformBlock.modelMatrix));
		
    // Calculate the position in clip coordinates
    gl_Position = transformBlock.projectionMatrix * transformBlock.viewingMatrix * transformBlock.modelMatrix * vertexPosition;

	vertexWorldPosition = transformBlock.modelMatrix * vertexPosition;

	vertexWorldNormal = normalModelMatrix * vec4(vertexNormal, 1.0);

	TexCoord0 = vertexTexCoord; 

	eyePosition = inverse(transformBlock.viewingMatrix)[3].xyz;

} // end main