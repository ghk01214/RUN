#include "pch.h"
#include "Cube.h"

Cube::Cube(glm::vec3 pos) :
	D3{ "../Dependencies/model/cube.obj", pos }
{
	_radius = vec3::unit();
}

Cube::~Cube()
{
}
