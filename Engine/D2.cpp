#include "pch.h"
#include "D2.h"

void D2::OnLoad(std::shared_ptr<Shader>& shader)
{
	Object::OnLoad(shader);

	_mesh->CreateVertex2(shader);
}
