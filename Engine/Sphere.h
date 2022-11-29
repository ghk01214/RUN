#pragma once

#include "D3.h"

class Sphere : public D3
{
public:
	Sphere(glm::vec3 pos = vec3::zero());
	~Sphere();
};