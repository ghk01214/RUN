#include "pch.h"
#include "Torus.h"

Torus::Torus(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/torus.obj", pos }
#else
	D3{ "Data/Model/torus.obj", pos }
#endif

{
}

Torus::~Torus()
{
}
