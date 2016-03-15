#include "Cube.h"




Cube::Cube()
{
}


Cube::~Cube()
{
}

void Cube::initialize()
{
	// Buffer Vertex Data for a Color cube
	GLfloat hW = 1 / 2.0f;
	GLfloat hH = 1 / 2.0f;
	GLfloat hD = 1 / 2.0f;

	vector<pntVertexData> v;

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
	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v1, n, glm::vec2(1.0f, 0.0f)));

	v.push_back(pntVertexData(v1, n, glm::vec2(1.0f, 0.0f)));
	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(1.0f, 1.0f)));

	n = glm::vec3(1, 0, 0);
	v.push_back(pntVertexData(v3, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(1.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(1.0f, 1.0f)));

	v.push_back(pntVertexData(v3, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(0.0f, 1.0f)));

	n = glm::vec3(0, 0, 1);
	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v3, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(1.0f, 0.0f)));

	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(1.0f, 0.0f)));
	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 0.0f)));

	n = glm::vec3(0, 0, -1);
	v.push_back(pntVertexData(v1, n, glm::vec2(1.0f, 0.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(0.0f, 0.0f)));

	v.push_back(pntVertexData(v2, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(0.0f, 1.0f)));

	n = glm::vec3(0, 1, 0);
	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(2.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(2.0f, 2.0f)));

	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(2.0f, 2.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(0.0f, 2.0f)));

	n = glm::vec3(0, -1, 0);
	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(2.0f, 0.0f)));
	v.push_back(pntVertexData(v3, n, glm::vec2(2.0f, 1.0f)));

	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v1, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(2.0f, 0.0f)));

	glUseProgram(shaderProgram);


	material = Material(glGetUniformLocation(shaderProgram, "object.ambientMat"),
						glGetUniformLocation(shaderProgram, "object.diffuseMat"),
						glGetUniformLocation(shaderProgram, "object.specularMat"),
						glGetUniformLocation(shaderProgram, "object.specularExp"),
						glGetUniformLocation(shaderProgram, "object.emmissiveMat"),
						glGetUniformLocation(shaderProgram, "object.textureMode"));

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec4)));
	glEnableVertexAttribArray(2);

	numberOfIndices = v.size();
	v.clear();

	// Set the modeling transformation
	glm::mat4 modelingTransformation = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));
	
}
void Cube::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Set the modeling transformation
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	// Set the material properties
	material.setShaderMaterialProperties();

	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, numberOfIndices);
}
