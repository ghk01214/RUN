#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(glm::vec3 pos) :
	D3{ "../Dependencies/model/sphere.obj", pos }
{
}

Sphere::~Sphere()
{
}
