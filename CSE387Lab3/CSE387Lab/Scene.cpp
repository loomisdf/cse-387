#include "Scene.h"

struct pnVertexData
{
	glm::vec4 m_pos;
	glm::vec3 m_normal;

	pnVertexData() {}

	pnVertexData(glm::vec4 pos, glm::vec3 normal)
	{
		m_pos = pos;
		m_normal = normal;

	}
};


Scene::Scene()
	:sceneTransformBlockBindingPoint(1)
{
	//cout << "Scene Constructor Called." << endl;
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
	glClearColor(0.1f, 0.1f, 1.0f, 0.0f); // Set the window clear color

	// Build shader proram
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	shaderProgram = BuildShaderProgram(shaders);

	glUseProgram(shaderProgram);

	initializeVertexData();

	initializeUniformData();

	checkOpenGLErrors("Scene Initialize");

} // end initilize


void Scene::initializeVertexData()
{
	// Buffer Vertex Data for a Color cube
	GLfloat hW = 1 / 2.0f;
	GLfloat hH = 1 / 2.0f;
	GLfloat hD = 1 / 2.0f;

	vector<pnVertexData> v;

	// Vertex Positions
	glm::vec4 v0(-hW, -hH, hD, 1.0f);
	glm::vec4 v1(-hW, -hH, -hD, 1.0f);
	glm::vec4 v2(hW, -hH, -hD, 1.0f);
	glm::vec4 v3(hW, -hH, hD, 1.0f);
	glm::vec4 v4(-hW, hH, hD, 1.0f);
	glm::vec4 v5(-hW, hH, -hD, 1.0f);
	glm::vec4 v6(hW, hH, -hD, 1.0f);
	glm::vec4 v7(hW, hH, hD, 1.0f);

	// Create interleaved position and normal data
	glm::vec3 n = glm::vec3(-1, 0, 0);
	v.push_back(pnVertexData(v0, n));
	v.push_back(pnVertexData(v4, n));
	v.push_back(pnVertexData(v1, n));

	v.push_back(pnVertexData(v1, n));
	v.push_back(pnVertexData(v4, n));
	v.push_back(pnVertexData(v5, n));

	n = glm::vec3(1, 0, 0);
	v.push_back(pnVertexData(v3, n));
	v.push_back(pnVertexData(v2, n));
	v.push_back(pnVertexData(v6, n));

	v.push_back(pnVertexData(v3, n));
	v.push_back(pnVertexData(v6, n));
	v.push_back(pnVertexData(v7, n));

	n = glm::vec3(0, 0, 1);
	v.push_back(pnVertexData(v0, n));
	v.push_back(pnVertexData(v3, n));
	v.push_back(pnVertexData(v7, n));

	v.push_back(pnVertexData(v0, n));
	v.push_back(pnVertexData(v7, n));
	v.push_back(pnVertexData(v4, n));

	n = glm::vec3(-1, 0, 0);
	v.push_back(pnVertexData(v1, n));
	v.push_back(pnVertexData(v5, n));
	v.push_back(pnVertexData(v2, n));

	v.push_back(pnVertexData(v2, n));
	v.push_back(pnVertexData(v5, n));
	v.push_back(pnVertexData(v6, n));

	n = glm::vec3(0, 1, 0);
	v.push_back(pnVertexData(v4, n));
	v.push_back(pnVertexData(v7, n));
	v.push_back(pnVertexData(v6, n));

	v.push_back(pnVertexData(v4, n));
	v.push_back(pnVertexData(v6, n));
	v.push_back(pnVertexData(v5, n));

	n = glm::vec3(0, -1, 0);
	v.push_back(pnVertexData(v0, n));
	v.push_back(pnVertexData(v2, n));
	v.push_back(pnVertexData(v3, n));

	v.push_back(pnVertexData(v0, n));
	v.push_back(pnVertexData(v1, n));
	v.push_back(pnVertexData(v2, n));

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	numberOfVertices = v.size();


	// --- Buffer the vertex data --- //
	GLuint buffer;
	glGenBuffers(1, &buffer);

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Set the buffer data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), (const void*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	checkOpenGLErrors("Scene initializeVertexData");


}

void Scene::initializeUniformData()
{
	// Color for the rendered object
	glm::vec4 objectColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	// Modeling Transformation
	glm::mat4 modelingTransformation = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection transformation
	glm::mat4 projectionTransformation = glm::perspective(M_PI / 4.0f, (GLfloat)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 50.0f);

	// Viewing transformation
	glm::mat4 viewingTransformation = glm::translate(glm::vec3(0.0, 0.0f, -6.0f));




	// --- Set regular uniforms --- //

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	assert(colorLocation != 0xFFFFFFFF);

	glUniform4fv(colorLocation, 1, glm::value_ptr(objectColor));



	// --- set up uniform blocks ---- //

	GLuint sceneTransformBlockIndex = glGetUniformBlockIndex(shaderProgram, "sceneTransformBlock");
	GLint sceneTransformBlockSize;
	glGetActiveUniformBlockiv(shaderProgram, sceneTransformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &sceneTransformBlockSize);

	glUniformBlockBinding(shaderProgram, sceneTransformBlockIndex, sceneTransformBlockBindingPoint);

	GLuint uniformIndeces[2] = { 0 };
	GLint uniformOffsets[2] = { 0 };

	const GLchar * charStringNames[] = { "sceneTransformBlock.viewingMatrix", "sceneTransformBlock.projectionMatrix" };

	// Get the indeces of the uniforms.
	glGetUniformIndices(shaderProgram, 2, (const GLchar **)charStringNames, uniformIndeces);

	// Get the offsets of the uniforms
	glGetActiveUniformsiv(shaderProgram, 2, uniformIndeces, GL_UNIFORM_OFFSET, uniformOffsets);

	viewLocation = uniformOffsets[0];
	projectionLocation = uniformOffsets[1];

	GLuint sceneTransformBuffer;
	glGenBuffers(1, &sceneTransformBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, sceneTransformBuffer);

	glBufferData(GL_UNIFORM_BUFFER, sceneTransformBlockSize, NULL, GL_DYNAMIC_DRAW);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, sceneTransformBlockBindingPoint, sceneTransformBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, sceneTransformBuffer);

	glBufferSubData(GL_UNIFORM_BUFFER, viewLocation, sizeof(glm::mat4), glm::value_ptr(viewingTransformation));

	glBufferSubData(GL_UNIFORM_BUFFER, projectionLocation, sizeof(glm::mat4), glm::value_ptr(projectionTransformation));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	checkOpenGLErrors("Scene initializeUniformData");

}


void Scene::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Render the object
	glm::vec4 objectColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	glUniform4fv(colorLocation, 1, glm::value_ptr(objectColor));

	glm::mat4 modelingTransformation = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
	

	// Render the cube three more times

	objectColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	glUniform4fv(colorLocation, 1, glm::value_ptr(objectColor));

	modelingTransformation = glm::translate(glm::vec3(1.5, 1.5f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);



	objectColor = glm::vec4(0.5f, 0.0f, 1.0f, 1.0f);

	glUniform4fv(colorLocation, 1, glm::value_ptr(objectColor));

	modelingTransformation = glm::translate(glm::vec3(-1.5, -1.5f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

	checkOpenGLErrors("Scene Draw");

} // end draw
