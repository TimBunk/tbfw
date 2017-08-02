#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <gl\glew.h>
#include <glm-0.9.9.0\glm\glm.hpp>

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#include "mesh.h"

class OBJloader {

public:

	static std::vector<Mesh*> LoadObject(const char* fileName, bool hasTextureCoord, bool hasNormalCoord);

private:

	static void LoadVertices(std::ifstream& obj, char* reader, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures, std::vector<glm::vec3>& tmp_normals);
	static GLfloat* LoadIndices(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureIndices, std::vector<unsigned int>& normalIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures, std::vector<glm::vec3>& tmp_normals);

};

#endif // !OBJLOADER_H