#include "Scene.h"

struct pcVertexData
{
	glm::vec3 m_pos;
	glm::vec4 m_color;

	pcVertexData() {}

	pcVertexData(glm::vec3 pos, glm::vec4 col)
	{
		m_pos = pos;
		m_color = col;

	}
};


Scene::Scene()
{
	cout << "Scene Constructor Called." << endl;
}


Scene::~Scene()
{
	cout << "Scene Destructor Called." << endl;

	// Delete shader programs and vertex array object
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void Scene::initialize()
{
	// Initialize OpenGL 
	glEnable(GL_DEPTH_TEST); // Turn depth testing
	glClearColor(0.1f, 0.1f, 0.5f, 0.0f); // Set the window clear color

	// Build shader proram
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};
//Front face winding order
	shaderProgram = BuildShaderProgram(shaders);

	glUseProgram(shaderProgram);

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);

	glBindVertexArray(vertexArrayObject);

	// Buffer Vertex Data for a Color cube
	GLfloat hW = 1 / 2.0f;
	GLfloat hH = 1 / 2.0f;
	GLfloat hD = 1 / 2.0f;

	vector<pcVertexData> v;

	pcVertexData v0(glm::vec3(-hW, -hH, hD), glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f));
	pcVertexData v1(glm::vec3(-hW, -hH, -hD), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	pcVertexData v2(glm::vec3(hW, -hH, -hD), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pcVertexData v3(glm::vec3(hW, -hH, hD), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	pcVertexData v4(glm::vec3(-hW, hH, hD), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	pcVertexData v5(glm::vec3(-hW, hH, -hD), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pcVertexData v6(glm::vec3(hW, hH, -hD), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	pcVertexData v7(glm::vec3(hW, hH, hD), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


	// 0 4 1
	// 1 4 5

	// 3 2 6
	// 3 6 7

	// 0 3 7
	// 0 7 4

	// 1 5 2Front face winding order
	// 2 5 6

	// 4 7 5
	// 4 6 5

	// 0 2 3
	// 0 1 2

	color red(1.0f, 0.0f, 0.0f, 1.0f);

	v.push_back(v0);
	v.push_back(v4);
	v.push_back(v1);

	v.push_back(v1);
	v.push_back(v4);
	v.push_back(v5);

	v.push_back(v3);
	v.push_back(v2);
	v.push_back(v6);

	v.push_back(v3);
	v.push_back(v6);
	v.push_back(v7);

	v.push_back(v0);
	v.push_back(v3);
	v.push_back(v7);

	v.push_back(v0);
	v.push_back(v7);
	v.push_back(v4);

	v.push_back(v1);
	v.push_back(v5);
	v.push_back(v2);

	v.push_back(v2);
	v.push_back(v5);
	v.push_back(v6);

	v.push_back(v4);
	v.push_back(v7);
	v.push_back(v6);

	v.push_back(v4);
	v.push_back(v6);
	v.push_back(v5);

	v.push_back(v0);
	v.push_back(v2);
	v.push_back(v3);

	v.push_back(v0);
	v.push_back(v1);
	v.push_back(v2);

	GLuint VBO;

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pcVertexData), &v[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pcVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pcVertexData), (const GLvoid*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	numberOfIndices = v.size();
	v.clear();

	// Set the modeling transformation
	glm::mat4 modelingTransformation = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	// Set the viewing transformation
	glm::mat4 viewingTransformation = glm::translate(glm::vec3(0.0, 0.0f, -6.0f));
	GLuint viewLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	assert(viewLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewingTransformation));

	// Set the projection transformation
	glm::mat4 projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 50.0f);
	GLuint projLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	assert(projLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

}


void Scene::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Render the cube
	glDrawArrays(GL_TRIANGLES, 0, numberOfIndices);

}

void Scene::resize(int width, int height) {
	glm::mat4 projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)width / height, 1.0f, 50.0f);
	GLuint projLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	assert(projLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

}
