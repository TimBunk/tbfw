#include "OBJloader.h"

std::vector<Mesh*> OBJloader::LoadObject(const char * fileName, bool hasTextureCoord, bool hasNormalCoord)
{
	// initialize some usefull variables
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec2> tmp_textures;
	std::vector<glm::vec3> tmp_normals;
	bool moreMeshes = true;
	std::vector<Mesh*> meshes;
	std::ifstream obj(fileName);
	char reader[128];

	// check if the file exists
	if (!obj.is_open()) {
		std::cout << "Could not open/find: " << fileName << std::endl;
	}
	else {
		// read the first line to avoid conflicts
		obj.getline(reader, 128);
		// load the actual meshes
		while (moreMeshes) {
			// initialize the needed variables
			std::vector<unsigned int> vertexIndices, textureIndices, normalIndices;
			GLfloat* vertices;
			int verticesArraySize;

			// load vertices
			OBJloader::LoadVertices(obj, reader, tmp_vertices, tmp_textures, tmp_normals);
			// set the indices
			vertices = OBJloader::LoadIndices(obj, reader, moreMeshes, verticesArraySize, vertexIndices, textureIndices, normalIndices, tmp_vertices, tmp_textures, tmp_normals);

			// create the actual mesh and push it in a vector
			MeshData meshData;
			meshData.vertices = vertices;
			meshData.amountVertices = verticesArraySize;
			Mesh* mesh;
			mesh = new Mesh(meshData);
			meshes.push_back(mesh);
		}
	}
	// return the vector of meshes
	return meshes;
}

void OBJloader::LoadVertices(std::ifstream& obj, char* reader, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures, std::vector<glm::vec3>& tmp_normals)
{
	// eof stands for end_of_file
	while (!obj.eof()) {
		// v means we found a geometric vertices
		if (reader[0] == 'v' && reader[1] == ' ') {
			glm::vec3 tmpVertex;
			sscanf(reader, "v %f %f %f", &tmpVertex.x, &tmpVertex.y, &tmpVertex.z);
			tmp_vertices.push_back(tmpVertex);
		}
		// vt means we found a texture coordinate
		else if (reader[0] == 'v' && reader[1] == 't') {
			glm::vec2 tmpTexture;
			sscanf(reader, "vt %f %f", &tmpTexture.x, &tmpTexture.y);
			tmp_textures.push_back(tmpTexture);
		}
		// vn means we found a vertex normals 
		else if (reader[0] == 'v' && reader[1] == 'n') {
			glm::vec3 tmpNormal;
			sscanf(reader, "vn %f %f %f", &tmpNormal.x, &tmpNormal.y, &tmpNormal.z);
			tmp_normals.push_back(tmpNormal);
		}
		// f means we found a face so we move on to the next loop
		else if (reader[0] == 'f') {
			break;
		}
		obj.getline(reader, 128);
	}
}

GLfloat* OBJloader::LoadIndices(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureIndices, std::vector<unsigned int>& normalIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures, std::vector<glm::vec3>& tmp_normals)
{
	// set moreMeshes to false to avoid conflicts
	moreMeshes = false;
	while (!obj.eof()) {
		// if we found something else then a face we continue with the loop
		if (reader[0] != 'f') {
			// if we find a vertex that means there is another mesh in the obj file
			if (reader[0] == 'v') {
				moreMeshes = true;
				break;
			}
			obj.getline(reader, 256);
			continue;
		}
		// push the indices in to  a vector
		unsigned int a, b, c, d, e, f, g, h, i;
		sscanf(reader, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &b, &c, &d, &e, &f, &g, &h, &i);

		vertexIndices.push_back(a - 1);
		textureIndices.push_back(b - 1);
		normalIndices.push_back(c - 1);
		vertexIndices.push_back(d - 1);
		textureIndices.push_back(e - 1);
		normalIndices.push_back(f - 1);
		vertexIndices.push_back(g - 1);
		textureIndices.push_back(h - 1);
		normalIndices.push_back(i - 1);

		obj.getline(reader, 256);
	}
	// create and calculate the size of the vertexIndices array
	verticesArraySize = vertexIndices.size() * 8;
	GLfloat* vertices;
	vertices = new GLfloat[verticesArraySize];
	int vertexIndicesSize = vertexIndices.size();
	// place all of the vertices in the right place
	for (unsigned int i = 0; i < vertexIndicesSize; i++) {
		unsigned int vertexIndex = vertexIndices[i];
		vertices[i * 8] = tmp_vertices[vertexIndex].x;
		vertices[i * 8 + 1] = tmp_vertices[vertexIndex].y;
		vertices[i * 8 + 2] = tmp_vertices[vertexIndex].z;

		unsigned int textureIndex = textureIndices[i];
		vertices[i * 8 + 3] = tmp_textures[textureIndex].x;
		vertices[i * 8 + 4] = tmp_textures[textureIndex].y;

		unsigned int normalIndex = normalIndices[i];
		vertices[i * 8 + 5] = tmp_normals[normalIndex].x;
		vertices[i * 8 + 6] = tmp_normals[normalIndex].y;
		vertices[i * 8 + 7] = tmp_normals[normalIndex].z;
	}
	return vertices;
}
