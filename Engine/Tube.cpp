#include "pch.h"
#include "Tube.h"

Tube::Tube(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/tube.obj", pos }
#else
	D3{ "Data/Model/tube.obj", pos }
#endif
{
}

Tube::~Tube()
{
}
