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

	Mesh(MeshData meshData, bool normalMapping, bool enableTextCoord, bool hasTextCoord, bool enableNormCoord, bool hasNormCoord);
	~Mesh();

	void DrawTextures(Shader* shader);
	void Draw();
	void WireframeMode(bool enabled);

	void SetAmountOfTextures(unsigned int diffuseMaps, unsigned int specularMaps, unsigned int emissionMaps);
	void AddTexture(Texture texture);

private:
	unsigned int amountVertices;
	GLuint VBO, VAO;

	unsigned int diffuseMaps, specularMaps, emissionMaps;
	std::vector<Texture> textures;
};

#endif // !MESH_H
