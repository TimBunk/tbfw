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

	static MeshData LoadObject(const char* fileName);

private:


};

#endif // !OBJLOADER_H