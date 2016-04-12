#ifndef _MESH_
#define _MESH_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "BasicIncludesAndDefines.h"
#include "VisibleObject.h"

struct Texture {

	GLuint id;
	string type;
    aiString path; 
};

class Mesh  {

public:
	
	Mesh(const vector<pntVertexData>& vertices, const vector<GLuint>& indices, const vector<Texture>& textures);
	~Mesh();

	// Set the shader program for this object
	void setShader(GLuint shaderProgram) { this->shaderProgram = shaderProgram; }

	virtual void initialize();

	virtual void draw(); 
	
protected:

	// Mesh Data 
	std::vector<pntVertexData> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	GLuint shaderProgram;

	GLuint vertexArrayObject;

	int numberOfIndices;
	
};

#endif