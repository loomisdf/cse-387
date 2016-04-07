#include "AssimpModel.h"


bool _loadTexture2D_(std::string imgPath, BYTE** imgData, GLuint& width, GLuint& height, GLenum& iFormat) 
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	// Check the file signature and deduce its format
	fif = FreeImage_GetFileType(imgPath.c_str(), 0);

	if (fif == FIF_UNKNOWN) {  // if still unkown, try to gurss the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(imgPath.c_str());
	}

	if (fif == FIF_UNKNOWN) {  // if still unkown, return failure
		std::cerr << "ERROR:Cannot get file type\n";
		return false;
	}

	if (FreeImage_FIFSupportsReading(fif)) {  // check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, imgPath.c_str());
	}

	if (!dib) {
		std::cerr << "ERROR:Cannot read image data\n";
		return false;
	}

	// Retrieve the image data
	*imgData = FreeImage_GetBits(dib);

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	int iBPP = FreeImage_GetBPP(dib);

	// if somehow one of these failed (they shouldn't), return failure
	if (*imgData == NULL || width == 0 || height == 0) {
		std::cerr << "ERROR:something bad happened when reading image\n";
		return false;
	}

	iFormat = (iBPP == 24) ? GL_RGB : GL_DEPTH_COMPONENT;

	// unload
	FreeImage_Unload(dib);

	checkOpenGLErrors("_loadTexture2D_");

	return true;

} // end _loadTexture2D_

GLint TextureFromFile(const char* path, const string& directory) 
{
	// Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	checkOpenGLErrors("TextureFromFile0");

	// load texture data
	BYTE* imgData = NULL;
	GLuint width, height;
	GLenum iFormat;

	if (!_loadTexture2D_(filename, &imgData, width, height, iFormat)) {
		std::cerr << "Error loading textures \n";
		return -1;
	}
	else {

		cout << "Loaded " << filename << " texture file." << endl;
	}

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);

	checkOpenGLErrors("TextureFromFile1");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, iFormat, GL_UNSIGNED_BYTE, imgData);

	checkOpenGLErrors("TextureFromFile2");

	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	checkOpenGLErrors("TextureFromFile");

	return textureID;

} // end TextureFromFile


AssimpModel::AssimpModel(GLchar* path)
{
	this->path = path;
}


AssimpModel::~AssimpModel()
{
}

void AssimpModel::initialize()
{
	// Buffer Vertex Data for a Color cube

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	material.setAmbientAndDiffuseMat(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	material.setTextureMapped(NO_TEXTURE);

	loadModel(path);

	checkOpenGLErrors("AssimpModel::initialize");

	VisibleObject::initialize();

}

bool AssimpModel::update(float deltatime) {
	VisibleObject::update(deltatime);
	return true;
}


void AssimpModel::loadModel(const string& path) {

	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	else {

		cout << "Loading " << path << " model." << endl;
	}

	this->directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	checkOpenGLErrors("AssimpModel::loadModel");
}

void AssimpModel::processNode(aiNode* node, const aiScene* scene) 
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}

	// Then do the same for each of its children 
	for (GLuint i = 0; i < node->mNumChildren; ++i) {

		this->processNode(node->mChildren[i], scene);
	}

	checkOpenGLErrors("AssimpModel::processNode");
}

Mesh AssimpModel::processMesh(aiMesh* mesh, const aiScene* scene) 
{
	// Data to fill
	std::vector<pntVertexData> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	checkOpenGLErrors("AssimpModel::processMesh0");

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		pntVertexData vertex;
		
		// We declare a placeholder vector since assimp uses its own 
		// vector class that doesn't directly convert to glm's vec3 
		// class so we transfer the data to this placeholder glm::vec3 first.
		glm::vec4 vector; 
		glm::vec3 normal;

		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vector.w = 1.0f;
		vertex.m_pos = vector;

		// Normals
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.m_normal = normal;

		// Texture Coordinates
		if (mesh->mTextureCoords[0]) { // Does the mesh contain texture coordinates?
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. 
			// We thus make the assumption that we won't use models where a 
			// vertex can have multiple texture coordinates so we always take 
			// the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_textCoord = vec;
		}
		else {
			vertex.m_textCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	checkOpenGLErrors("AssimpModel::processMesh1");

	// Now walk through each of the mesh's faces (a face is a mesh its triangle) and 
	// retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	checkOpenGLErrors("AssimpModel::processMesh2");

	// Process materials
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2. Specular maps
		vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	checkOpenGLErrors("AssimpModel::processMesh3");

	// Return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);

} // end processMesh


vector<Texture> AssimpModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) 
{
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}


void AssimpModel::draw()
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Set the modeling transformation
	SharedProjectionAndViewing::setModelingMatrix(getWorldTransformation());

	// Set the material properties and bind the texture object
	SharedMaterialProperties::setShaderMaterialProperties(material);
	
	// Draw the object
	for (auto& mesh : meshes) {

		mesh.setShader( shaderProgram );
		mesh.draw();
	}

	// Draw the kids
	VisibleObject::draw();

} // end draw
