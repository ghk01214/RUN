#include "pch.h"
#include "Cone.h"

Cone::Cone(glm::vec3 pos) :
	D3{ "../Dependencies/model/cone.obj", pos }
{
	_radius = glm::vec3{ 10.f };
}

Cone::~Cone()
{
}
