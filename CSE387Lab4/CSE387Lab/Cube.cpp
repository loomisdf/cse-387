#include "Cube.h"

struct pntVertexData
{
	glm::vec4 m_pos;
	glm::vec3 m_normal;
	glm::vec2 m_textCoord;

	pntVertexData() {}

	pntVertexData(glm::vec4 pos, glm::vec3 normal, glm::vec2 textCoord)
	{
		m_pos = pos;
		m_normal = normal;
		m_textCoord = textCoord;

	}
};


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
	// Set texture coordinates
	// -X
	glm::vec3 n = glm::vec3(-1, 0, 0);
	v.push_back(pntVertexData(v0, n, glm::vec2(1.0f, 0.0f)));
	v.push_back(pntVertexData(v4, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v1, n, glm::vec2(0.0f, 0.0f)));

	v.push_back(pntVertexData(v1, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v4, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(0.0f, 1.0f)));

	// +X
	n = glm::vec3(1, 0, 0);
	v.push_back(pntVertexData(v3, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(1.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(1.0f, 1.0f)));

	v.push_back(pntVertexData(v3, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(0.0f, 1.0f)));

	// +Z
	n = glm::vec3(0, 0, 1);
	v.push_back(pntVertexData(v0, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v3, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(0.0f, 0.0f)));

	v.push_back(pntVertexData(v0, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v4, n, glm::vec2(1.0f, 0.0f)));

	// -Z
	n = glm::vec3(0, 0, -1);
	v.push_back(pntVertexData(v1, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(1.0f, 1.0f)));

	v.push_back(pntVertexData(v2, n, glm::vec2(1.0f, 1.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(1.0f, 0.0f)));

	// +Y
	n = glm::vec3(0, 1, 0);
	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v7, n, glm::vec2(2.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(2.0f, 2.0f)));

	v.push_back(pntVertexData(v4, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v6, n, glm::vec2(2.0f, 2.0f)));
	v.push_back(pntVertexData(v5, n, glm::vec2(0.0f, 2.0f)));

	// -Y
	n = glm::vec3(0, -1, 0);
	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(2.0f, 1.0f)));
	v.push_back(pntVertexData(v3, n, glm::vec2(2.0f, 0.0f)));

	v.push_back(pntVertexData(v0, n, glm::vec2(0.0f, 0.0f)));
	v.push_back(pntVertexData(v1, n, glm::vec2(0.0f, 1.0f)));
	v.push_back(pntVertexData(v2, n, glm::vec2(2.0f, 1.0f)));

	glUseProgram(shaderProgram);

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Buffer the vertex data
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW);

	// Set up and enable flow for each of the vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const void*) sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const void*) (sizeof(glm::vec4) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	numberOfIndices = v.size();
	v.clear();

	CBitmap image;

	// Load the texture image
	assert(image.loadBMP((const char *)"BRICK.BMP") != false);

	// Make and bind the texture object
	glGenTextures(1, &textureObject);
	glBindTexture(GL_TEXTURE_2D, textureObject);

	glTexImage2D(GL_TEXTURE_2D, 0, image.getChannels(), image.getWidth(), image.getHeight(), 0,
		GL_BGR, GL_UNSIGNED_BYTE, image.getLinePtr(0));

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set the filtering modes for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set the wrap modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the modeling transformation
	glm::mat4 modelingTransformation = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::rotate(M_PI / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));

	checkOpenGLErrors("Cube::initialize");
	
}
void Cube::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Rotate the cube
	static float x = 1.03;
	glm::mat4 modelingTransformation = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::rotate(/*M_PI / 4.0f*/ glm::radians(x), rotation);
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelingTransformation));
	x+= 1.03;

	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, numberOfIndices);

	checkOpenGLErrors("Cube::draw");

}
