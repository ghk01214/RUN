#include "pch.h"
#include "Tetraherdon.h"

Tetraherdon::Tetraherdon(glm::vec3 pos)
{
	_pos = pos;

	std::vector<float> vertex
	{
		pos.x + 0.f, pos.y + 0.5f, pos.z + 0.f,
		pos.x + 0.5f, pos.y + (-0.5f), pos.z + 0.5f,
		pos.x + 0.f, pos.y + (-0.5f), pos.z + (-0.5f),
		pos.x + (-0.5f), pos.y + (-0.5f), pos.z + 0.5f,
	};

	std::vector<float> color
	{
		WHITE, 1.f,
		RED, 1.f,
		GREEN, 1.f,
		BLUE, 1.f
	};

	std::vector<uint32_t> index
	{
		1, 0, 3,
		2, 0, 1,
		3, 0, 2,
		1, 2, 3
	};

	_mesh->SetVertex(&vertex);
	_mesh->SetColor(&color);
	_mesh->SetIndex(&index);

	Move(pos);
}

Tetraherdon::~Tetraherdon()
{
}
