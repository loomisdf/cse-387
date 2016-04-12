#include "Mesh.h"
#include <iostream>
#include <sstream>


Mesh::Mesh(const vector<pntVertexData>& vertices, const vector<GLuint>& indices, const vector<Texture>& textures) 
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->initialize();
}

Mesh::~Mesh() {}

void Mesh::draw()
{
	GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    for(GLuint i = 0; i < textures.size(); i++) {

	     glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding

        // Retrieve texture number (the N in diffuse_textureN)
        std::stringstream ss;
        std::string number;
        std::string name = this->textures[i].type;

        if (name == "texture_diffuse") {

            ss << diffuseNr++; // Transfer GLuint to stream

		} else if (name == "texture_specular") {

           ss << specularNr++; // Transfer GLuint to stream
		}	

        number = ss.str(); 

		//std::cout << glGetUniformLocation(shaderProgram, (name + number).c_str()) << "\n";

		glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }

    // Draw mesh
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < textures.size(); i++) {

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::initialize() {

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint VBO, IBO;
	
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(pntVertexData), &(vertices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(sizeof(glm::vec4) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	numberOfIndices = indices.size();

	this->vertices.clear();
	this->indices.clear();

	glBindVertexArray(0);
}