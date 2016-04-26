#include "MyScene.h"


MyScene::MyScene()
	//:sphere(1, 64, 64), earth(0.5), moon(0.25), model("model/nanosuit.obj"), model2("space/retro_rocket_fbx/retro_rocket.fbx")
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
	ShaderInfo shadersPointFive[] = {
		{ GL_VERTEX_SHADER, "vertexShaderPerPixel.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShaderPerPixelMultiTexture.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	GLuint modelShaderProgram = BuildShaderProgram(shadersPointFive);

	checkOpenGLErrors("MyScene::initialize1");

	// Set up the uniform blocks for this shader
	SharedProjectionAndViewing::setUniformBlockForShader(modelShaderProgram);
	SharedMaterialProperties::setUniformBlockForShader(modelShaderProgram);
	SharedGeneralLighting::setUniformBlockForShader(modelShaderProgram);

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

	//cube->initialize();
	cube->material.setAmbientAndDiffuseMat(glm::vec4(0.1f, 0.1f, 1.0f, 1.0f));
	cube->material.setupTexture("Brick.bmp", DECAL);
	//cube->addBehavior(new Behavior());
	//vector<glm::vec3> points;
	//points.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	//points.push_back(glm::vec3(0.0f, 0.0f, 5.0f));
	//cube->addBehavior(new WaypointBehavior(points));
	addChild(cube);

	Sphere* sun = new Sphere(1, 64, 64);

	//sphere->initialize();
	sun->material.setAmbientAndDiffuseMat(glm::vec4(1.0f, 1.0f, 0.1f, 1.0f));
	sun->material.setupTexture("preview_sun.jpg", REPLACE_AMBIENT_DIFFUSE);
	sun->addBehavior(new Behavior());
	sun->addBehavior(new OrbitBehavior(glm::vec3(0.0f, 1.0f, 0.0f), 2.0f, 1.0f));
	addChild(sun);

	Sphere* earth = new Sphere(0.5);

	//earth->initialize();
	earth->material.setAmbientAndDiffuseMat(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
	earth->material.setupTexture("earth.bmp", REPLACE_AMBIENT_DIFFUSE);
	earth->addBehavior(new Behavior());
	earth->addBehavior(new OrbitBehavior(glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 1.5f));
	sun->addChild(earth);

	Sphere* moon = new Sphere(0.25);

	//moon->initialize();
	moon->material.setupTexture("moon.bmp", REPLACE_AMBIENT_DIFFUSE);
	moon->addBehavior(new Behavior());
	moon->addBehavior(new OrbitBehavior(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 2.0f));
	earth->addChild(moon);

	//Model ===================================================
	AssimpModel* model = new AssimpModel("model/nanosuit.obj");

	//model->initialize();
	model->setShader(modelShaderProgram);
	model->material.setTextureMapped(REPLACE_AMBIENT_DIFFUSE);
	//model->material.setSpecularExponentMat(16.0f);


	model->addBehavior(new Behavior());
	model->scale = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model->addBehavior(new SpinBehavior(glm::vec3(0.0f, -1.0f, 0.0f),
										glm::vec3(0.0f, 1.0f, 0.0f),
										glm::radians(30.0f)));

	addChild(model);
	//=========================================================

	// Initialize the shader for all the obects
	selectShader(0);

	checkOpenGLErrors("MyScene::initialize3");

	// Viewing transformation
	glm::mat4  viewingTransformation = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),
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


bool MyScene::update(float deltaTime)
{
//	const float rotationRate = 1.0f;
//	static float angle = 0.0f;
//
//	angle += (rotationRate * deltaTime);

	//cube.setLocalTransformation(glm::translate(glm::vec3(0.0f, -1.5f, 0.0f)));

	//this->sunTrans = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
	//				 glm::translate(glm::vec3(5.0f, 0.0f, 0.0f));

	//// Set the modeling transformation
	//sphere.setLocalTransformation(sunTrans);

	//this->earthTrans = glm::rotate(1.5f *angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
	//	glm::translate(glm::vec3(3.0f, 0.0f, 0.0f));

	//earth.setLocalTransformation(earthTrans);

	//this->moonTrans = glm::rotate(2.0f * angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
	//	glm::translate(glm::vec3(1.0f, 0.0f, 0.0f));

	//moon.setLocalTransformation(moonTrans);

	//glm::mat4 blueManTrans = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)) *
	//						 glm::rotate(-0.75f * angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
	//						 glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));

	//model.setLocalTransformation(blueManTrans);

	return VisibleObject::update(deltaTime);
}

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
	switch (shaderProgramNumber)
	{
	case 0:
		for (VisibleObject* child : children) {
			child->setShader(perPixelShaderProgram);
		}
		cout << "per pixel" << endl;
		break;
	case 1:
		for (VisibleObject* child : children) {
			child->setShader(shaderProgram);
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
		for (VisibleObject* child : children) {
			child->material.setTextureMapped(NO_TEXTURE);
		}
		cout << "No Texture" << endl;
		break;
	case 1:
		for (VisibleObject* child : children) {
			child->material.setTextureMapped(DECAL);
		}
		cout << "Decal" << endl;
		break;
	case 2:
		for (VisibleObject* child : children) {
			child->material.setTextureMapped(REPLACE_AMBIENT_DIFFUSE);
		}
		cout << "Replace ambient and diffuse" << endl;
		break;

	default:
		cout << "Illegal shader" << endl;
		break;
	}

}