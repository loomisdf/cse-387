#include "MyScene.h"




MyScene::MyScene()
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
	glClearColor(0.5f, 0.5f, 0.9f, 0.0f); // Set the window clear color

	// Build shader proram
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	shaderProgram = BuildShaderProgram(shaders);

	cube.setShader(shaderProgram);
	cube.initialize();

	// Viewing transformation
	viewingTransformation = glm::translate(glm::vec3(0.0, 0.0f, -6.0f));
	viewingMatrixLocation = glGetUniformLocation(shaderProgram, "viewingMatrix");
	assert(viewingMatrixLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(viewingMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewingTransformation));

	// Projection transformation
	projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 50.0f);
	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	assert(projectionMatrixLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

	setRotation(glm::vec3(0.0f, 1.0f, 0.0f));

	checkOpenGLErrors("MyScene::initialize");


}

void MyScene::resize()
{
	projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 1.0f, 50.0f);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

	checkOpenGLErrors("MyScene::resize");
}

void MyScene::setRotation(glm::vec3 rotation) {
	cube.rotation = rotation;
}

void MyScene::draw()
{
	cube.draw();

	checkOpenGLErrors("MyScene::draw");

}
