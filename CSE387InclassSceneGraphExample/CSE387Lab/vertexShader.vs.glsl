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

layout (shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};


struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat; // *new added emmissive property
	float specularExp;
	int textureMode;
};

uniform Material object;

struct Light {

	vec4 ambientColor; // ambient color of the light 
	vec4 diffuseColor; // diffuse color of the light 
	vec4 specularColor; // specular color of the light
	// Either the position or direction 
	// if w = 0 then the light is directional and direction specified 
	// is the negative of the direction the light is shinning 
	// if w = 1 then the light is positional 
	vec4 positionOrDirection;
	
	 // spotlight attributes 
	 // the direction the cone of light is shinning 
	 vec3 spotDirection; 

	// 1 if the light is a spotlight 
	int isSpot; 
	 
	// Cosine of the spot cutoff angle float spotExponent;
	float spotCutoffCos; 
	
	float spotExponent; // For gradual falloff near cone edge 

	int enabled; // 1 if light is "on"  
};

uniform Light light1; 
uniform Light light2;
uniform Light light3;

/*out*/ vec4 vertexWorldPosition;
/*out*/ vec4 vertexWorldNormal;

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

out vec2 TexCoord0;
out vec4 color0;

vec3 eyePosition;

vec3 shadingCaculation(Light light )
{
	vec3 totalFromThisLight = vec3(0.0, 0.0, 0.0);

	if (light.enabled == 1) {

		// Calculate a bunch of vectors
		vec3 lightVector;
		if (light.positionOrDirection.w < 1) {
			// Directional
			lightVector = normalize(light.positionOrDirection.xyz);
		}
		else {
			// Positional
			lightVector = normalize(light.positionOrDirection.xyz - 
				vertexWorldPosition.xyz );
		}

		vec3 reflection = normalize( reflect(-lightVector, vertexWorldNormal.xyz) );
		vec3 eyeVector = normalize(eyePosition - vertexWorldPosition.xyz);

		float spotCosFactor = 0; 
		if (light.isSpot == 1) {
		
			spotCosFactor = dot (-lightVector, normalize(light.spotDirection));
		}	

		// Is it a spot light and are we in the cone?
		if( light.isSpot < 1 ||  (light.isSpot == 1 && spotCosFactor >= light.spotCutoffCos ) ) {

			// Ambient Reflection
			totalFromThisLight += object.ambientMat.xyz * light.ambientColor.xyz;

			// Difusse Reflection
			totalFromThisLight += max(dot(vertexWorldNormal.xyz, lightVector ), 0) *
				object.diffuseMat.xyz * light.diffuseColor.xyz;

			totalFromThisLight += pow( max( dot( reflection, eyeVector ), 0), object.specularExp) *
			object.specularMat.xyz * light.specularColor.xyz;
		}
	}

	return totalFromThisLight;

} // end shadingCaculation



void main()
{
		
    // Calculate the position in clip coordinates
    gl_Position = projectionMatrix * viewingMatrix * modelMatrix * vertexPosition;

	vertexWorldPosition = modelMatrix * vertexPosition;

	vertexWorldNormal = normalModelMatrix * vec4(vertexNormal, 1.0);

	TexCoord0 = vertexTexCoord; 

	eyePosition = inverse(viewingMatrix)[3].xyz;

	color0 = object.emmissiveMat + // * new emmissive property contributes to total illumination
			  vec4(shadingCaculation(light1),1.0) + 
			  vec4(shadingCaculation(light2),1.0) + 
			  vec4(shadingCaculation(light3),1.0);

} // end main

