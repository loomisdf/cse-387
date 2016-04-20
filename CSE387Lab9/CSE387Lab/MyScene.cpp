#include "MyScene.h"


MyScene::MyScene()
{
	cout << "Scene Constructor Called." << endl;
}


MyScene::~MyScene()
{
	cout << "Scene Destructor Called." << endl;
}

void MyScene::initialize()
{
	checkOpenGLErrors("MyScene::initialize0");

	// Initialize OpenGL 
	glEnable(GL_DEPTH_TEST); // Turn depth testing
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set the window clear color

	// Build shader proram
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "vertexShaderPerPixel.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShaderPerPixel.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	perPixelShaderProgram = BuildShaderProgram(shaders);
	checkOpenGLErrors("MyScene::initialize1");

	// Set up the uniform blocks for this shader
	SharedProjectionAndViewing::setUniformBlockForShader(perPixelShaderProgram);
	SharedMaterialProperties::setUniformBlockForShader(perPixelShaderProgram);
	SharedGeneralLighting::setUniformBlockForShader(perPixelShaderProgram);

	// Build shader proram
	ShaderInfo shaders2[] = {
		{ GL_VERTEX_SHADER, "vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	shaderProgram = BuildShaderProgram(shaders2);
	checkOpenGLErrors("MyScene::initialize2");

	// Set up the uniform blocks for this shader
	SharedProjectionAndViewing::setUniformBlockForShader(shaderProgram);
	SharedMaterialProperties::setUniformBlockForShader(shaderProgram);
	SharedGeneralLighting::setUniformBlockForShader(shaderProgram);

	Cube* cube = new Cube();

	cube->initialize();
	cube->material.setAmbientAndDiffuseMat(glm::vec4(0.1f, 0.1f, 1.0f, 1.0f));
	cube->material.setupTexture("Brick.bmp", REPLACE_AMBIENT_DIFFUSE);
//	addChild(cube);

	Sphere* sphere = new Sphere(1, 64, 64);

	sphere->initialize();
	sphere->material.setAmbientAndDiffuseMat(glm::vec4(1.0f, 1.0f, 0.1f, 1.0f));
	sphere->material.setupTexture("preview_sun.jpg", REPLACE_AMBIENT_DIFFUSE);
//	addChild(sphere);

	Sphere* earth = new Sphere(0.5);

	earth->initialize();
	earth->material.setAmbientAndDiffuseMat(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
	earth->material.setupTexture("earth.bmp", REPLACE_AMBIENT_DIFFUSE);
//	sphere->addChild(earth);

	Sphere* moon = new Sphere(0.25);
	moon->initialize();
	moon->material.setupTexture("moon.bmp", REPLACE_AMBIENT_DIFFUSE);
//	earth->addChild(moon);

	//Model* model = new Model("Space/dark_fighter_6.obj");
	Model* model = new Model("Space/millenium-falcon/millenium-falcon.obj");

	VisibleObject* empty = new VisibleObject();
	empty->setLocalTransformation(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	addChild(empty);

	model->material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	model->scale = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));

//	model->scaleMatrix = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
	empty->addChild(model);

	model->addBehavior(new SpinBehavior(glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::radians(30.0f)));

	checkOpenGLErrors("MyScene::initialize3");

	// Viewing transformation
	glm::mat4  viewingTransformation = glm::lookAt(glm::vec3(0.0f, 0.0f, -30.0f),
										glm::vec3(0.0f, 0.0f, 0.0f),
										glm::vec3(0.0f, 1.0f, 0.0f));

	// Set viewing transformation
	SharedProjectionAndViewing::setViewMatrix(viewingTransformation);

	checkOpenGLErrors("MyScene::initialize4");
	// Light 1
	glm::vec4 light1AmbColor(0.15f, 0.15f, 0.15f, 1.0f);
	glm::vec4 light1DifColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light1SpecColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light1PositionOfDirection(5.0f, 5.0f, 5.0f, 1.0f);
	bool light1IsSpot = false;
	bool light1Enabled = true;

	SharedGeneralLighting::setAmbientColor(GL_LIGHT_ZERO, light1AmbColor);
	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ZERO, light1DifColor);
	SharedGeneralLighting::setSpecularColor(GL_LIGHT_ZERO, light1SpecColor);
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ZERO, light1PositionOfDirection);
	SharedGeneralLighting::setIsSpot(GL_LIGHT_ZERO, light1IsSpot);
	SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, light1Enabled);

	checkOpenGLErrors("MyScene::initialize5");
	// Light 2
	glm::vec4 light2AmbColor(0.15f, 0.15f, 0.15, 1.0f);
	glm::vec4 light2DifColor(0.75f, 0.75f, 0.75f, 1.0f);
	glm::vec4 light2SpecColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light2PositionOfDirection(1.0f, 1.0f, 1.0f, 0.0f);
	bool light2IsSpot = false;
	bool light2Enabled = true;

	SharedGeneralLighting::setAmbientColor(GL_LIGHT_ONE, light2AmbColor);
	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ONE, light2DifColor);
	SharedGeneralLighting::setSpecularColor(GL_LIGHT_ONE, light2SpecColor);
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ONE, light2PositionOfDirection);
	SharedGeneralLighting::setIsSpot(GL_LIGHT_ONE, light2IsSpot);
	SharedGeneralLighting::setEnabled(GL_LIGHT_ONE, light2Enabled);

	checkOpenGLErrors("MyScene::initialize6");
	// Light 3
	glm::vec4 light3AmbColor(0.15f, 0.15f, 0.15, 1.0f);
	glm::vec4 light3DifColor(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec4 light3SpecColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 light3PositionOfDirection(0.0f, 0.0f, 12.0f, 1.0f);
	glm::vec3 light3SpotDirection(0.0f, 0.0f, -1.0f);
	bool light3IsSpot = true;
	GLfloat light3SpotCutOff = glm::cos(glm::radians(10.0f));
	GLfloat light3SpotExponent = 0.9f;
	bool light3Enabled = true;

	SharedGeneralLighting::setAmbientColor(GL_LIGHT_TWO, light3AmbColor);
	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_TWO, light3DifColor);
	SharedGeneralLighting::setSpecularColor(GL_LIGHT_TWO, light3SpecColor);
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_TWO, light3PositionOfDirection);
	SharedGeneralLighting::setSpotDirection(GL_LIGHT_TWO, light3SpotDirection);
	SharedGeneralLighting::setIsSpot(GL_LIGHT_TWO, light3IsSpot);
	SharedGeneralLighting::setSpotCutoffCos(GL_LIGHT_TWO, light3SpotCutOff);
	SharedGeneralLighting::setSpotExponent(GL_LIGHT_TWO, light3SpotExponent);
	SharedGeneralLighting::setEnabled(GL_LIGHT_TWO, light3Enabled);

	checkOpenGLErrors("MyScene::initialize7");

	VisibleObject::initialize();

	// Initialize the shader for all the obects
	selectShader(0);
}


void MyScene::resize(int windowWidth, int windowHeight)
{
	glm::mat4 projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)windowWidth / windowHeight, 1.0f, 100.0f);

	SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);
}


//bool MyScene::update(float deltaTime)
//{
//	const float rotationRate = 1.0f;
//	static float angle = 0.0f;
//
//	angle += (rotationRate * deltaTime);
//
//	cube.setLocalTransformation(glm::translate(glm::vec3(0.0f, -1.5f, 0.0f)));
//
//	this->sunTrans = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
//					 glm::translate(glm::vec3(5.0f, 5.0f, 0.0f));
//
//	// Set the modeling transformation
//	sphere.setLocalTransformation(sunTrans);
//
//	this->earthTrans = glm::rotate(1.5f *angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
//		glm::translate(glm::vec3(3.0f, 0.0f, 0.0f));
//
//	earth.setLocalTransformation(earthTrans);
//
//	this->moonTrans = glm::rotate(2.0f * angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
//		glm::translate(glm::vec3(1.0f, 0.0f, 0.0f));
//
//	moon.setLocalTransformation(moonTrans);
//
//	glm::mat4 blueManTrans = glm::translate(glm::vec3(0.0f, -7.5f, 0.0f)) *
//		glm::rotate(-0.75f * angle, glm::vec3(0.0f, 1.0f, 0.0f));
//
//	//model.setLocalTransformation(blueManTrans);
//
//	return true;
//}

void MyScene::toggleLights(int light)
{
	switch (light)
	{
		case 0:
			SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, !SharedGeneralLighting::getEnabled(GL_LIGHT_ZERO));
			cout << "toggle lights" << endl;
			break;
		case 1:
			SharedGeneralLighting::setEnabled(GL_LIGHT_ONE, !SharedGeneralLighting::getEnabled(GL_LIGHT_ONE));
			cout << "toggle lights" << endl;
			break;
		case 2:
			SharedGeneralLighting::setEnabled(GL_LIGHT_TWO, !SharedGeneralLighting::getEnabled(GL_LIGHT_TWO));
			cout << "toggle lights" << endl;
			break;
		default:

			cout << "Illegal Light" << endl;
			break;
	}

}

void MyScene::selectShader(int shaderProgramNumber)
{
	//switch (shaderProgramNumber)
	//{
	//case 0:

	//	sphere.setShader(perPixelShaderProgram);
	//	cube.setShader(perPixelShaderProgram);
	//	moon.setShader(perPixelShaderProgram);
	//	earth.setShader(perPixelShaderProgram);
	//	model.setShader(perPixelShaderProgram);

	//	cout << "per pixel" << endl;
	//	break;
	//case 1:
	//	sphere.setShader(shaderProgram);
	//	cube.setShader(shaderProgram);
	//	moon.setShader(shaderProgram);
	//	earth.setShader(shaderProgram);
	//	model.setShader(shaderProgram);

	//	cout << "per vertex" << endl;
	//	break;
	//default:
	//	cout << "Illegal shader" << endl;
	//	break;
	//}




	switch (shaderProgramNumber)
	{
	case 0:
		for (VisibleObject* obj : children) {

			obj->setShader(perPixelShaderProgram);
		}
		cout << "per pixel" << endl;
		break;
	case 1:
		for (VisibleObject* obj : children) {

			obj->setShader(shaderProgram);
		}

		cout << "per vertex" << endl;
		break;
	default:
		cout << "Illegal shader" << endl;
		break;
	}
}

void MyScene::setTextureMode(int mode)
{

	switch (mode)
	{
	case 0:

		for (VisibleObject* obj : children) {

			obj->material.setTextureMode(NO_TEXTURE);
		}

		cout << "No Texture" << endl;
		break;
	case 1:
		for (VisibleObject* obj : children) {

			obj->material.setTextureMode(DECAL);
		}

		cout << "Decal" << endl;
		break;
	case 2:

		for (VisibleObject* obj : children) {

			obj->material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
		}

		cout << "Replace ambient and diffuse" << endl;
		break;

	default:
		cout << "Illegal shader" << endl;
		break;
	}
	//switch (mode)
	//{
	//case 0:

	//	
	//	sphere.material.setTextureMode(NO_TEXTURE);
	//	cube.material.setTextureMode(NO_TEXTURE);
	//	earth.material.setTextureMode(NO_TEXTURE);
	//	moon.material.setTextureMode(NO_TEXTURE);
	//	//model.material.setTextureMode(NO_TEXTURE);
	//	cout << "No Texture" << endl;
	//	break;
	//case 1:
	//	sphere.material.setTextureMode(DECAL);
	//	cube.material.setTextureMode(DECAL);
	//	earth.material.setTextureMode(DECAL);
	//	moon.material.setTextureMode(DECAL);
	//	//model.material.setTextureMode(DECAL);
	//	cout << "Decal" << endl;
	//	break;
	//case 2:
	//	sphere.material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	cube.material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	earth.material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	moon.material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	//model.material.setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	cout << "Replace ambient and diffuse" << endl;
	//	break;

	//default:
	//	cout << "Illegal shader" << endl;
	//	break;
	//}

}



