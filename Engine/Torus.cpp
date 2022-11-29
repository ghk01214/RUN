#include "pch.h"
#include "Torus.h"

Torus::Torus(glm::vec3 pos) :
	D3{ "../Dependencies/model/torus.obj", pos }
{
}

Torus::~Torus()
{
}
