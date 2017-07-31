#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>

#include <GL\glew.h>

#include "shader.h"
#include "texture.h"

struct MeshData {
	GLfloat* vertices;
	GLfloat amountVertices;
};

class Mesh {

public:

	Mesh(const char* fileName);
	~Mesh();

	void DrawTextures(Shader* shader);
	void Draw();
	void WireframeMode(bool enabled);

	void SetAmountOfTextures(unsigned int diffuseMaps, unsigned int specularMaps, unsigned int emissionMaps);
	void AddTexture(Texture texture);

private:
	MeshData meshData;
	GLuint VBO, VAO;

	unsigned int diffuseMaps, specularMaps, emissionMaps;
	std::vector<Texture> textures;
};

#endif // !MESH_H
