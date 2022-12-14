#include "pch.h"
#include "Teapot.h"

Teapot::Teapot(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/teapot.obj", pos }
#else
	D3{ "Data/Model/teapot.obj", pos }
#endif
{
}

Teapot::~Teapot()
{
}
