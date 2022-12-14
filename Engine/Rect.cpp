#include "pch.h"
#include "Rect.h"

Rect::Rect(glm::vec3 pos) :
#if _DEBUG
	D3{ "../Dependencies/model/rect.obj", pos }
#else
	D3{ "Data/Model/rect.obj", pos }
#endif

{
	_radius.y = 0.f;
}

Rect::~Rect()
{
}
