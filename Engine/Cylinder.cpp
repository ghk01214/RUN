#include "pch.h"
#include "Cylinder.h"

Cylinder::Cylinder(glm::vec3 pos) :
	D3{ "../Dependencies/model/cylinder.obj", pos }
{
}

Cylinder::~Cylinder()
{
}
