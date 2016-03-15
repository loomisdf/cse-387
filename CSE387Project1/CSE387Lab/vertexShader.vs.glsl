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

//uniform mat4 viewingMatrix;
//uniform mat4 projectionMatrix;
//uniform mat4 modelMatrix;

/*uniform*/ mat4 normalModelMatrix; // *new added commented out uniform as a reminder
uniform sampler2D gSampler;

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
	Material object;
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


/*out*/ vec4 vertexWorldPosition;
/*out*/ vec4 vertexWorldNormal;

layout (location = 0) in vec4 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;


out vec4 color0;

vec3 eyePosition;

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
	normalModelMatrix = transpose(inverse(transformBlock.modelMatrix));
		
    // Calculate the position in clip coordinates
    gl_Position = transformBlock.projectionMatrix * transformBlock.viewingMatrix * transformBlock.modelMatrix * vertexPosition;

	vertexWorldPosition = transformBlock.modelMatrix * vertexPosition;

	vertexWorldNormal = normalModelMatrix * vec4(vertexNormal, 1.0);

	eyePosition = inverse(transformBlock.viewingMatrix)[3].xyz;

	if(object.textureMode == 0) {
		//no texture mapping
		color0 = object.emissiveMat + // * new emmissive property contributes to total illumination
			  vec4(shadingCaculation(light1, object),1.0) + 
			  vec4(shadingCaculation(light2, object),1.0) + 
			  vec4(shadingCaculation(light3, object),1.0);
	}
	else if(object.textureMode == 1) {
		color0 = texture2D(gSampler, vertexTexCoord.st);
	}
	else { // object texturemode == 2
		Material myObject = object;

		myObject.ambientMat =  texture2D(gSampler, vertexTexCoord);
		myObject.diffuseMat =  texture2D(gSampler, vertexTexCoord);

		color0 = object.emissiveMat + // * new emmissive property contributes to total illumination
			  vec4(shadingCaculation(light1, myObject),1.0) + 
			  vec4(shadingCaculation(light2, myObject),1.0) + 
			  vec4(shadingCaculation(light3, myObject),1.0);
	}


} // end main

