#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <gl\glew.h>
#include <glm-0.9.8.4\glm\glm.hpp>

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#include "mesh.h"

class OBJloader {

public:

	static std::vector<Mesh*> LoadObject(const char* fileName, bool normalMapping, bool enableTextCoord, bool hasTextCoord, bool enableNormCoord, bool hasNormCoord);

private:

	static void LoadVertices(std::ifstream& obj, char* reader, std::vector<glm::vec3>& tmp_vertices);
	static void LoadTextures(std::ifstream& obj, char* reader, std::vector<glm::vec2>& tmp_textures);
	static void LoadNormals(std::ifstream& obj, char* reader, std::vector<glm::vec3>& tmp_normals);

	// this function loads faces with vertices, textures and normals
	static GLfloat* LoadFaces(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, bool& normalMapping, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureIndices, std::vector<unsigned int>& normalIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures, std::vector<glm::vec3>& tmp_normals);
	// this function loads faces that only include vertices!
	static GLfloat* LoadFacesWithVertices(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<glm::vec3>& tmp_vertices);
	// this function loads faces that only include vertices and textures!
	static GLfloat* LoadFacesWithTextures(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures);
	// this function loads faces that only include vertices and normals!
	static GLfloat* LoadFacesWithNormals(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& normalIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec3>& tmp_normals);

};

#endif // !OBJLOADER_H