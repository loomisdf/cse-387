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

class Mesh : public VisibleObject {

public:
	
	Mesh(const vector<pntVertexData>& vertices, const vector<GLuint>& indices, const vector<Texture>& textures);
	~Mesh();

	virtual void initialize();

	virtual void draw(); 
	
protected:

	// Mesh Data 
	std::vector<pntVertexData> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	
};

#endif