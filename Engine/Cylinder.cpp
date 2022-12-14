#include "pch.h"
#include "Cylinder.h"

Cylinder::Cylinder(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/cylinder.obj", pos }
#else
	D3{ "Data/Model/cylinder.obj", pos }
#endif
{
}

Cylinder::~Cylinder()
{
}
