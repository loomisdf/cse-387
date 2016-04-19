#include "Model.h"
#include <sstream>

vector<Texture> Model::MeshEntry::textures_loaded;

/**
*	Model constructor, loads the specified filename if supported by Assimp
**/
Model::Model(const string filename)
:fileName(filename)
{

}


/**
*	Clears all loaded MeshEntries
**/
Model::~Model(void)
{
	for (unsigned int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();

} // end destructor


string GetDirectoryPath(string sFilePath)
{
	// Get directory path
	string sDirectory = "";

	for (int i = (int)sFilePath.size() - 1; i >= 0; i--) {

		if (sFilePath[i] == '\\' || sFilePath[i] == '/')
		{
			sDirectory = sFilePath.substr(0, i + 1);
			break;
		}
	}
	return sDirectory;
}


void Model::initialize()
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(fileName,
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices);

	if (!scene) {
		cerr << "ERROR: Unable to load " << fileName << 
			endl<< "\t" << importer.GetErrorString() << endl;
		return;
	}
	else {
		cout << "Loading " << fileName << endl;
	}

	// Clear the list of loaded textures (Assume no models use the same texture)
	Model::MeshEntry::textures_loaded.clear();

	const string directory = GetDirectoryPath(fileName);

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new Model::MeshEntry(scene->mMeshes[i], scene , directory));
	}
	
	// Initialize the children
	VisibleObject::initialize();

} // end initilize


/**
*	Renders all loaded MeshEntries
**/
void Model::draw()
{
	// Use the shader program
	glUseProgram(shaderProgram);

	// Set the modeling transformation
	SharedProjectionAndViewing::setModelingMatrix(getWorldTransformation() * scale);

	for (unsigned int i = 0; i < meshEntries.size(); ++i) {

		meshEntries.at(i)->meshMaterial.setTextureMode(this->material.textureMode);
		meshEntries.at(i)->draw(shaderProgram);
	}

	// Draw the kids
	VisibleObject::draw();

} // end draw


/**
*	Constructor, loading the specified aiMesh
**/
Model::MeshEntry::MeshEntry(aiMesh *mesh, const aiScene* scene , const string directory)
: directory(directory)
{
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	numberOfIndices = mesh->mNumFaces * 3;

	if(mesh->HasPositions()) {

		vector<glm::vec4> vertexPositions;

		for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {

			glm::vec4 tempPosition;
			tempPosition.x = mesh->mVertices[i].x;
			tempPosition.y = mesh->mVertices[i].y;
			tempPosition.z = mesh->mVertices[i].z;
			tempPosition.w = 1.0f;
			vertexPositions.push_back(tempPosition);
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(glm::vec4), &vertexPositions[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (0);

		vertexPositions.clear();
	}
	
	// Normal Vectors
	if(mesh->HasNormals()) {

		vector<glm::vec3> vertexNormals;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

			glm::vec3 tempNormal;
			tempNormal.x = mesh->mNormals[i].x;
			tempNormal.y = mesh->mNormals[i].y;
			tempNormal.z = mesh->mNormals[i].z;
			vertexNormals.push_back(tempNormal);
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(glm::vec3), &vertexNormals[0], GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (1);

		vertexNormals.clear();
	}

	// Texture Coordinates
	if (mesh->HasTextureCoords(0)) {

		vector<glm::vec2> texCoords;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			glm::vec2 tempCoord;
			tempCoord.x = mesh->mTextureCoords[0][i].x;
			tempCoord.y = mesh->mTextureCoords[0][i].y;
			texCoords.push_back(tempCoord);
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		texCoords.clear();
	}
	else {

		cerr << "No texture coordinates." << endl;

	}

	// Indices
	if(mesh->HasFaces()) {

		vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {

			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back( mesh->mFaces[i].mIndices[1]);
			indices.push_back( mesh->mFaces[i].mIndices[2]);
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (3);

		indices.clear();
		numberOfIndices = mesh->mNumFaces * 3;
	}

	// Process materials
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString name;
		material->Get(AI_MATKEY_NAME, name);

		int numProp = material->mNumProperties;

		aiColor3D matColor(0.f, 0.f, 0.f);

		if (material->Get(AI_MATKEY_COLOR_AMBIENT, matColor) == AI_SUCCESS) {

			meshMaterial.setAmbientMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
		}

		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, matColor) == AI_SUCCESS) {

			meshMaterial.setDiffuseMat(glm::vec4(1.0, 1.0, 1.0, 1.0));
		}

		if (material->Get(AI_MATKEY_COLOR_SPECULAR, matColor) == AI_SUCCESS) {

			meshMaterial.setSpecularMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
		}
		
		if (material->Get(AI_MATKEY_COLOR_EMISSIVE, matColor) == AI_SUCCESS) {

			meshMaterial.setEmissiveMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
		}

		//float shininess;

		//if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {

		//	meshMaterial.setSpecularExponentMat(shininess);
		//}

		if (mesh->HasTextureCoords(0)) {

			// Load only the diffuse map
			meshMaterial.textureObject = this->loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

} // end MeshEntry constructor


GLuint Model::MeshEntry::loadMaterialTexture(aiMaterial* mat, aiTextureType type, string typeName)
{
	GLuint textureObject = 0;

	aiString str;
	if (AI_SUCCESS == mat->GetTexture(type, 0, &str)) {

		GLboolean skip = false;

		GLuint j;

		for (j = 0; j < MeshEntry::textures_loaded.size(); j++)
		{
			if (MeshEntry::textures_loaded[j].path == str)
			{
				textureObject = MeshEntry::textures_loaded[j].id;
				skip = true;
				break;
			}
		}

		if (!skip) // If texture hasn't been loaded already, load it
		{   
			string textureFileName = directory + string(str.C_Str());

			textureObject = TextureFromFile(textureFileName);

			Texture texture;

			if (texture.id > 0) {
				texture.type = typeName;
				texture.path = str;
				texture.id = textureObject;

				// Save so that the texture will not be loaded a second time.
				MeshEntry::textures_loaded.push_back(texture);
			}
		}
	}
	return textureObject;

} // end loadMaterialTexture


/**
*	Deletes the allocated OpenGL buffers
**/
Model::MeshEntry::~MeshEntry() {

	if(vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if(vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if(vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if(vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vertexArrayObject);

} // end destructor


/**
*	Renders this MeshEntry
**/
void Model::MeshEntry::draw(GLint shaderProgram)
{
	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Set the material properties and bind the texture object
	SharedMaterialProperties::setShaderMaterialProperties(shaderProgram, meshMaterial);

	// Draw the model
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, NULL);

	// Unbind the vertex array object
	glBindVertexArray(0);

} // end draw

