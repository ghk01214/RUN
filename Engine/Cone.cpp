#include "pch.h"
#include "Cone.h"

Cone::Cone(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/cone.obj", pos }
#else
	D3{ "Data/Model/cone.obj", pos }
#endif
{
	_radius = glm::vec3{ 10.f };
}

Cone::~Cone()
{
}
