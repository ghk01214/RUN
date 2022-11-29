#include "pch.h"
#include "Pyramid.h"

Pyramid::Pyramid(glm::vec3 pos) :
	D3{ "../Dependencies/model/pyramid.obj", pos }
{
}

Pyramid::~Pyramid()
{
}
