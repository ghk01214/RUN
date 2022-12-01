#include "pch.h"
#include "Line.h"

extern std::uniform_real_distribution<float> uid_color;

Line::Line(glm::vec3 pos, glm::vec3 length)
{
	_draw_type = GL_LINES;
	_pos = pos;

	std::vector<float> vertex
	{
		pos.x + length.x, pos.y + length.y, pos.z + length.z,
		pos.x - length.x, pos.y - length.y, pos.z - length.z
	};

	std::vector<uint32_t> index
	{
		0, 1
	};

	_mesh->SetVertex(&vertex);
	_mesh->SetIndex(&index);
}

Line::~Line()
{
}
