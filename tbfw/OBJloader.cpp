#include "OBJloader.h"

std::vector<Mesh*> OBJloader::LoadObject(const char * fileName, bool normalMapping, bool enableTextCoord, bool hasTextCoord, bool enableNormCoord, bool hasNormCoord)
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
			OBJloader::LoadVertices(obj, reader, tmp_vertices);
			// load textures coord if enabled
			if (hasTextCoord) {
				OBJloader::LoadTextures(obj, reader, tmp_textures);
			}
			// load normals coord if enabled
			if (hasNormCoord) {
				OBJloader::LoadNormals(obj, reader, tmp_normals);
			}
			// load face with vertices, textures and normals
			if (hasTextCoord && hasNormCoord) {
				std::cout << "load face with vertices, textures and normals" << std::endl;
				vertices = OBJloader::LoadFaces(obj, reader, moreMeshes, verticesArraySize, normalMapping, vertexIndices, textureIndices, normalIndices, tmp_vertices, tmp_textures, tmp_normals);
			}
			// load face with vertices
			else if (!hasTextCoord && !hasNormCoord) {
				std::cout << "load face with vertices" << std::endl;
				vertices = OBJloader::LoadFacesWithVertices(obj, reader, moreMeshes, verticesArraySize, vertexIndices, tmp_vertices);
			}
			// load face with vertices and textures
			else if (hasTextCoord && !hasNormCoord) {
				std::cout << "load face with vertices and textures" << std::endl;
				vertices = OBJloader::LoadFacesWithTextures(obj, reader, moreMeshes, verticesArraySize, vertexIndices, textureIndices, tmp_vertices, tmp_textures);
			}
			// load face with vertices nad normals
			else if (!hasTextCoord && hasNormCoord) {
				std::cout << "load face with vertices and normals" << std::endl;
				vertices = OBJloader::LoadFacesWithNormals(obj, reader, moreMeshes, verticesArraySize, vertexIndices, normalIndices, tmp_vertices, tmp_normals);
			}
			// create the actual mesh and push it in a vector
			MeshData meshData;
			meshData.vertices = vertices;
			meshData.amountVertices = verticesArraySize;
			std::cout << "verticesArraysize: " << verticesArraySize << std::endl;
			Mesh* mesh;
			mesh = new Mesh(meshData, normalMapping, enableTextCoord, hasTextCoord, enableNormCoord, hasNormCoord);
			meshes.push_back(mesh);
		}
	}
	// return the vector of meshes
	return meshes;
}

void OBJloader::LoadVertices(std::ifstream& obj, char* reader, std::vector<glm::vec3>& tmp_vertices)
{
	// eof stands for end_of_file
	while (!obj.eof()) {
		// v means we found a geometric vertices so we push it in the vector
		if (reader[0] == 'v' && reader[1] == ' ') {
			glm::vec3 tmpVertex;
			sscanf(reader, "v %f %f %f", &tmpVertex.x, &tmpVertex.y, &tmpVertex.z);
			tmp_vertices.push_back(tmpVertex);
			obj.getline(reader, 128);
			continue;
		}
		// vt means we found a textCoord so we break out of the loop
		if (reader[0] == 'v' && reader[1] == 't') {
			break;
		}
		// vn means we found a normCoord so we break out of the loop
		else if (reader[0] == 'v' && reader[1] == 'n') {
			break;
		}
		// f means we found a face so we break out of the loop
		else if (reader[0] == 'f') {
			break;
		}
		obj.getline(reader, 128);
	}
}

void OBJloader::LoadTextures(std::ifstream & obj, char * reader, std::vector<glm::vec2>& tmp_textures)
{
	// eof stands for end_of_file
	while (!obj.eof()) {
		// vt means we found a texture coordinate so we push it in the vector
		if (reader[0] == 'v' && reader[1] == 't') {
			glm::vec2 tmpTexture;
			sscanf(reader, "vt %f %f", &tmpTexture.x, &tmpTexture.y);
			tmp_textures.push_back(tmpTexture);
			obj.getline(reader, 128);
			continue;
		}
		// vn means we found a normCoord so we break out of the loop
		if (reader[0] == 'v' && reader[1] == 'n') {
			break;
		}
		// f means we found a face so we break out of the loop
		else if (reader[0] == 'f') {
			break;
		}
		obj.getline(reader, 128);
	}
}

void OBJloader::LoadNormals(std::ifstream & obj, char * reader, std::vector<glm::vec3>& tmp_normals)
{
	// eof stands for end_of_file
	while (!obj.eof()) {
		// vn means we found a vertex normals so we push it in the vector
		if (reader[0] == 'v' && reader[1] == 'n') {
			glm::vec3 tmpNormal;
			sscanf(reader, "vn %f %f %f", &tmpNormal.x, &tmpNormal.y, &tmpNormal.z);
			tmp_normals.push_back(tmpNormal);
			obj.getline(reader, 128);
			continue;
		}
		// f means we found a face so we break out of the loop
		if (reader[0] == 'f') {
			break;
		}
		obj.getline(reader, 128);
	}
}

GLfloat* OBJloader::LoadFaces(std::ifstream& obj, char* reader, bool& moreMeshes, int& verticesArraySize, bool& normalMapping, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureIndices, std::vector<unsigned int>& normalIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures, std::vector<glm::vec3>& tmp_normals)
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
	// some variables for normalMapping
	glm::vec3 tangent;
	glm::vec3 bitangent;
	unsigned int multiplier = 8;
	if (normalMapping) {
		multiplier = 14;
	}
	// create and calculate the size of the vertexIndices array
	verticesArraySize = vertexIndices.size() * multiplier;
	GLfloat* vertices;
	vertices = new GLfloat[verticesArraySize];
	int vertexIndicesSize = vertexIndices.size();
	// place all of the vertices in the right place
	for (unsigned int i = 0; i < vertexIndicesSize; i++) {
		unsigned int vertexIndex = vertexIndices[i];
		vertices[i * multiplier] = tmp_vertices[vertexIndex].x;
		vertices[i * multiplier + 1] = tmp_vertices[vertexIndex].y;
		vertices[i * multiplier + 2] = tmp_vertices[vertexIndex].z;

		unsigned int textureIndex = textureIndices[i];
		std::cout << "textureIndex: " << textureIndex << std::endl;
		vertices[i * multiplier + 3] = tmp_textures[textureIndex].x;
		vertices[i * multiplier + 4] = tmp_textures[textureIndex].y;

		unsigned int normalIndex = normalIndices[i];
		vertices[i * multiplier + 5] = tmp_normals[normalIndex].x;
		vertices[i * multiplier + 6] = tmp_normals[normalIndex].y;
		vertices[i * multiplier + 7] = tmp_normals[normalIndex].z;
		// calc triangels edges and delta uv coordinates
		if (normalMapping) {
			if (((i + 1) % 3) == 1) {
				glm::vec3 edge1 = tmp_vertices[vertexIndices[(i + 1)]] - tmp_vertices[vertexIndex];
				glm::vec3 edge2 = tmp_vertices[vertexIndices[(i + 2)]] - tmp_vertices[vertexIndex];
				glm::vec2 deltaUV1 = tmp_textures[textureIndices[(i + 1)]] - tmp_textures[textureIndex];
				glm::vec2 deltaUV2 = tmp_textures[textureIndices[(i + 2)]] - tmp_textures[textureIndex];
				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

				tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				tangent = glm::normalize(tangent);
				bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
				bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
				bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
				bitangent = glm::normalize(bitangent);
			}
			vertices[i * multiplier + 8] = tangent.x;
			vertices[i * multiplier + 9] = tangent.y;
			vertices[i * multiplier + 10] = tangent.z;

			vertices[i * multiplier + 11] = bitangent.x;
			vertices[i * multiplier + 12] = bitangent.y;
			vertices[i * multiplier + 13] = bitangent.z;
		}
	}
	return vertices;
}

GLfloat * OBJloader::LoadFacesWithVertices(std::ifstream & obj, char * reader, bool & moreMeshes, int & verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<glm::vec3>& tmp_vertices)
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
		// push the indices in to a vector
		unsigned int a, b, c;
		sscanf(reader, "f %d %d %d", &a, &b, &c);

		vertexIndices.push_back(a - 1);
		vertexIndices.push_back(b - 1);
		vertexIndices.push_back(c - 1);

		obj.getline(reader, 256);
	}
	// create and calculate the size of the vertexIndices array
	verticesArraySize = vertexIndices.size() * 3;
	GLfloat* vertices;
	vertices = new GLfloat[verticesArraySize];
	int vertexIndicesSize = vertexIndices.size();
	// place all of the vertices in the right place
	for (unsigned int i = 0; i < vertexIndicesSize; i++) {
		unsigned int vertexIndex = vertexIndices[i];
		vertices[i * 3] = tmp_vertices[vertexIndex].x;
		vertices[i * 3 + 1] = tmp_vertices[vertexIndex].y;
		vertices[i * 3 + 2] = tmp_vertices[vertexIndex].z;
	}
	return vertices;
}

GLfloat * OBJloader::LoadFacesWithTextures(std::ifstream & obj, char * reader, bool & moreMeshes, int & verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec2>& tmp_textures)
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
		unsigned int a, b, c, d, e, f;
		sscanf(reader, "f %d/%d %d/%d %d/%d", &a, &b, &c, &d, &e, &f);

		vertexIndices.push_back(a - 1);
		textureIndices.push_back(b - 1);
		vertexIndices.push_back(c - 1);
		textureIndices.push_back(d - 1);
		vertexIndices.push_back(e - 1);
		textureIndices.push_back(f - 1);

		obj.getline(reader, 256);
	}
	// create and calculate the size of the vertexIndices array
	verticesArraySize = vertexIndices.size() * 5;
	GLfloat* vertices;
	vertices = new GLfloat[verticesArraySize];
	int vertexIndicesSize = vertexIndices.size();
	// place all of the vertices in the right place
	for (unsigned int i = 0; i < vertexIndicesSize; i++) {
		unsigned int vertexIndex = vertexIndices[i];
		vertices[i * 5] = tmp_vertices[vertexIndex].x;
		vertices[i * 5 + 1] = tmp_vertices[vertexIndex].y;
		vertices[i * 5 + 2] = tmp_vertices[vertexIndex].z;

		unsigned int textureIndex = textureIndices[i];
		vertices[i * 5 + 3] = tmp_textures[textureIndex].x;
		vertices[i * 5 + 4] = tmp_textures[textureIndex].y;
	}
	return vertices;
}

GLfloat * OBJloader::LoadFacesWithNormals(std::ifstream & obj, char * reader, bool & moreMeshes, int & verticesArraySize, std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& normalIndices, std::vector<glm::vec3>& tmp_vertices, std::vector<glm::vec3>& tmp_normals)
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
		unsigned int a, b, c, d, e, f;
		sscanf(reader, "f %d//%d %d//%d %d//%d", &a, &b, &c, &d, &e, &f);

		vertexIndices.push_back(a - 1);
		normalIndices.push_back(b - 1);
		vertexIndices.push_back(c - 1);
		normalIndices.push_back(d - 1);
		vertexIndices.push_back(e - 1);
		normalIndices.push_back(f - 1);

		obj.getline(reader, 256);
	}
	// create and calculate the size of the vertexIndices array
	verticesArraySize = vertexIndices.size() *6;
	GLfloat* vertices;
	vertices = new GLfloat[verticesArraySize];
	int vertexIndicesSize = vertexIndices.size();
	// place all of the vertices in the right place
	for (unsigned int i = 0; i < vertexIndicesSize; i++) {
		unsigned int vertexIndex = vertexIndices[i];
		vertices[i * 6] = tmp_vertices[vertexIndex].x;
		vertices[i * 6 + 1] = tmp_vertices[vertexIndex].y;
		vertices[i * 6 + 2] = tmp_vertices[vertexIndex].z;

		unsigned int normalIndex = normalIndices[i];
		vertices[i * 6 + 3] = tmp_normals[normalIndex].x;
		vertices[i * 6 + 4] = tmp_normals[normalIndex].y;
		vertices[i * 6 + 5] = tmp_normals[normalIndex].z;
	}
	return vertices;
}
