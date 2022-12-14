#include "pch.h"
#include "Rect.h"

Rect::Rect(glm::vec3 pos) :
	D3{ "../Dependencies/model/rect.obj", pos }
{
	_radius.y = 0.f;
}

Rect::~Rect()
{
}
