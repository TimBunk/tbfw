#include "OBJloader.h"

MeshData OBJloader::LoadObject(const char * fileName)
{
	// initialize the needed variables
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec2> tmp_textures;
	std::vector<glm::vec3> tmp_normals;

	std::vector<unsigned int> vertexIndices, textureIndices, normalIndices;
	GLfloat* vertices;
	int sizeVerticesArray;

	std::ifstream obj(fileName);
	// check if the file exists
	if (!obj.is_open()) {
		std::cout << "Could not open/find: " << fileName << std::endl;
	}
	else {
		// read the file
		char reader[256];
		// eof stands for end_of_file
		while (!obj.eof()) {
			obj.getline(reader, 256);
			// # means we found a command
			if (reader[0] == '#') {
				continue;
			}
			// v means we found a geometric vertices
			else if (reader[0] == 'v' && reader[1] == ' ') {
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
		}
		// push all of the indices in the correct vector
		while (!obj.eof()) {
			// if we found a face we continue with the loop
			if (reader[0] != 'f') {
				obj.getline(reader, 256);
				continue;
			}
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
		// create and calculate the size of the vertices array
		sizeVerticesArray = vertexIndices.size() * 8;
		vertices = new GLfloat[sizeVerticesArray];
		// place all of the v/vt/vn in the right place
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
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
	}
	// return the meshData
	MeshData meshData;
	meshData.vertices = vertices;
	meshData.amountVertices = sizeVerticesArray;
	return meshData;
}