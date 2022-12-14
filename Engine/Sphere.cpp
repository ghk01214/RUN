#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/sphere.obj", pos }
#else
	D3{ "Data/Model/sphere.obj", pos }
#endif
{
}

Sphere::~Sphere()
{
}
