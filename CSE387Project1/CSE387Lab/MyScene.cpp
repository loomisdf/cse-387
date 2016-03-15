#include "MyScene.h"

#include "Lights.h"



MyScene::MyScene()
	:sphere(1, 64, 64), earth(0.5), moon(0.25)
{
	cout << "Scene Constructor Called." << endl;
}


MyScene::~MyScene()
{
	cout << "Scene Destructor Called." << endl;

	// Delete shader programs and vertex array object
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void MyScene::initialize()
{
	// Initialize OpenGL
	glEnable(GL_DEPTH_TEST); // Turn depth testing
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the window clear color
	ShaderInfo per_vertex[] = {
		{ GL_VERTEX_SHADER, "vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	ShaderInfo per_pixel[] = {
		{ GL_VERTEX_SHADER, "vertexShader2.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShader2.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	shaderProgram_PV = BuildShaderProgram(per_pixel);

	shaderProgram_PP = BuildShaderProgram(per_pixel);

	transformBlock.setShader(shaderProgram_PV);
	transformBlock.initialize();

	cube.setShader(shaderProgram_PV);
	cube.initialize();
	cube.material.setupTexture("BRICK.bmp", REPLACE_AMBIENT_DIFFUSE);
	cube.material.setShaderMaterialProperties();
	cube.material.setAmbientAndDiffuseMat(glm::vec4(0.1f, 0.1f, 1.0f, 1.0f));

	sphere.setShader(shaderProgram_PV);
	sphere.initialize();
	sphere.material.setupTexture("sun.bmp", REPLACE_AMBIENT_DIFFUSE);
	sphere.material.setAmbientAndDiffuseMat(glm::vec4(1.0f, 1.1f, 0.1f, 1.0f));

	earth.setShader(shaderProgram_PV);
	earth.initialize();
	earth.material.setupTexture("earth.bmp", REPLACE_AMBIENT_DIFFUSE);
	earth.material.setAmbientAndDiffuseMat(glm::vec4(0.0f, 1.0f, 0.1f, 1.0f));

	moon.setShader(shaderProgram_PV);
	moon.initialize();
	moon.material.setupTexture("moon.bmp", REPLACE_AMBIENT_DIFFUSE);
	moon.material.setAmbientAndDiffuseMat(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

	//// Viewing transformation
	//viewingTransformation = glm::translate(glm::vec3(0.0f, 0.0f, -6.0f));
	viewingTransformation = glm::lookAt(glm::vec3(0.0f, 35.0f, 0.0f), 
							glm::vec3(0.0f, 0.0f, 0.0f), 
							glm::vec3(0.0f, 0.0f, -1.0f));

	transformBlock.setViewingMatrix(viewingTransformation);
	lights.setShader(shaderProgram_PV);
	lights.initialize();
}


void MyScene::resize(int windowWidth, int windowHeight)
{
	projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)windowWidth / windowHeight, 1.0f, 100.0f);
	//glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionTransformation));
	transformBlock.setProjectionMatrix(projectionTransformation);
}



void MyScene::draw()
{
	// Draw the cube and sphere
	transformBlock.setModelMatrix(cube.getModelingTransformation());
	cube.draw();

	transformBlock.setModelMatrix(sphere.getModelingTransformation());
	sphere.draw();
	
	transformBlock.setModelMatrix(earth.getModelingTransformation());
	earth.draw();

	transformBlock.setModelMatrix(moon.getModelingTransformation());
	moon.draw();
}

void MyScene::update(float deltaTime) {

	const float ROTATION_RATE = 0.5f;
	static float angle = 0.0f;

	angle += (ROTATION_RATE * deltaTime);

	this->cubeTrans = glm::rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));

	this->sunTrans = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)) * 
					 glm::translate(glm::vec3(10.0f, 0.0f, 0.0f));

	this->earthTrans = sunTrans *
					 glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
					 glm::translate(glm::vec3(5.0f, 0.0f, 0.0f));

	this->moonTrans = earthTrans * 
					 glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
					 glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));

	// Set the modeling transformation
	cube.setModelingTransformation(cubeTrans);

	sphere.setModelingTransformation(sunTrans);

	earth.setModelingTransformation(earthTrans);

	moon.setModelingTransformation(moonTrans);

}

void MyScene::enableLight(string lightName) {
	if (lightName == "light1") {
		lights.setEnabled(!lights.enabled[0], LightNum1);
		cout << "toggle light1" << endl;
	}
	else if (lightName == "light2") {
		lights.setEnabled(!lights.enabled[1], LightNum2);
		cout << "toggle light2" << endl;
	}
	else if (lightName == "light3") {
		lights.setEnabled(!lights.enabled[2], LightNum3);
		cout << "toggle light3" << endl;
	}
}

void MyScene::setShader(ShaderProgram shader) {
	

	switch (shader) {
	case(0) :
		cube.setShader(shaderProgram_PV);
		sphere.setShader(shaderProgram_PV);
		earth.setShader(shaderProgram_PV);
		moon.setShader(shaderProgram_PV);
		lights.setShader(shaderProgram_PV);
		transformBlock.setShader(shaderProgram_PV);
		break;

	case(1) :
		cube.setShader(shaderProgram_PP);
		sphere.setShader(shaderProgram_PP);
		earth.setShader(shaderProgram_PP);
		moon.setShader(shaderProgram_PP);
		lights.setShader(shaderProgram_PP);
		transformBlock.setShader(shaderProgram_PP);
		break;
	}
}

void MyScene::setTextureMode(TextureMode textureMode) {
	cube.material.setTextureMapped(textureMode);
	sphere.material.setTextureMapped(textureMode);
	earth.material.setTextureMapped(textureMode);
	moon.material.setTextureMapped(textureMode);
}