#include "pch.h"
#include "Pyramid.h"

Pyramid::Pyramid(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/pyramid.obj", pos }
#else
	D3{ "Data/Model/pyramid.obj", pos }
#endif
{
}

Pyramid::~Pyramid()
{
}
