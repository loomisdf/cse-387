// Targeting version 4.2 of GLSL. 
#version 420 core

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings 
precision mediump float;

out vec4 fragmentColor;

vec4 color0;

in vec2 TexCoord0;
in vec4 vertexWorldPosition;
in vec4 vertexWorldNormal;
in vec3 eyePosition;

// Create a to sampler
uniform sampler2D gSampler;

// Structure for material properties
struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emissiveMat; // *new added emmissive property
	float specularExp;
	int textureMode;
};

layout (shared) uniform MaterialBlock 
{ 
	uniform Material object; 
};

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

layout (shared) uniform LightBlock
{
	Light light1;
	Light light2;
	Light light3;
};

vec3 shadingCaculation(Light light, Material object)
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
	// Use the texture coordinate and the sampler to get a color value
	fragmentColor = texture2D(gSampler, TexCoord0.st);

	if(object.textureMode == 0) {
		//no texture mapping
		color0 = object.emissiveMat + // * new emmissive property contributes to total illumination
			  vec4(shadingCaculation(light1, object),1.0) + 
			  vec4(shadingCaculation(light2, object),1.0) + 
			  vec4(shadingCaculation(light3, object),1.0);

		fragmentColor = color0;
	}
	else if(object.textureMode == 1) {
		fragmentColor = texture2D(gSampler, TexCoord0.st);
	}
	else { // object texturemode == 2
		Material myObject = object;

		myObject.ambientMat =  texture2D(gSampler, TexCoord0); 
		myObject.diffuseMat =  texture2D(gSampler, TexCoord0);

		color0 = object.emissiveMat + // * new emmissive property contributes to total illumination
			  vec4(shadingCaculation(light1, myObject),1.0) + 
			  vec4(shadingCaculation(light2, myObject),1.0) + 
			  vec4(shadingCaculation(light3, myObject),1.0);

		fragmentColor = color0;
	}
}