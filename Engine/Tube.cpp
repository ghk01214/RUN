#include "pch.h"
#include "Tube.h"

Tube::Tube(glm::vec3 pos) :
	D3{ "../Dependencies/model/tube.obj", pos }
{
}

Tube::~Tube()
{
}
