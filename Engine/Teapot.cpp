#include "pch.h"
#include "Teapot.h"

Teapot::Teapot(glm::vec3 pos) :
	D3{ "../Dependencies/model/teapot.obj", pos }
{
}

Teapot::~Teapot()
{
}
