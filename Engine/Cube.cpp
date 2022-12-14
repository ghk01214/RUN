#include "pch.h"
#include "Cube.h"

Cube::Cube(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/cube.obj", pos }
#else
	D3{ "Data/Model/cube.obj", pos }
#endif
{
}

Cube::~Cube()
{
}
