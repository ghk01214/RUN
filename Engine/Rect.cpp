#include "pch.h"
#include "Rect.h"

Rect::Rect(glm::vec3 pos) :
	D3{ "../Dependencies/model/rect.obj", pos }
{
}

Rect::~Rect()
{
}
