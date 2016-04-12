#pragma once
#include "VisibleObject.h"

#include "Mesh.h"

bool _loadTexture2D_(std::string imgPath, BYTE** imgData, GLuint& width, GLuint& height, GLenum& iFormat);

GLint TextureFromFile(const char* path, const std::string& directory);

class AssimpModel : public VisibleObject
{

public:

	AssimpModel(GLchar* path);

	~AssimpModel();

	virtual void initialize();

	virtual void draw();

	virtual bool update(float deltaTime);

protected:

	void loadModel(const std::string& path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	// Model Data 
	vector<Mesh> meshes;

	//vector<Texture> texturesLoaded;

	string directory;

	// Path to the model
	GLchar* path;

	// Stores all the textures loaded so far, 
	// optimization to make sure textures aren't loaded more than once.
	vector<Texture> textures_loaded;	

};

