#include "pch.h"
#include <fstream>
#include <sstream>
#include "Circle.h"

Circle::Circle(glm::vec3 pos)
{
	_draw_type = GL_LINE_LOOP;

	std::vector<float> vertex;

	std::ifstream file{ "../Dependencies/model/circle.obj" };
	std::string line;
	std::stringstream stream;

	vertex.reserve(87);

	while (std::getline(file, line))
	{
		stream.clear();
		stream.str(line);

		std::string type;
		stream >> type;

		if (type != "v")
			continue;

		glm::vec3 pos;
		stream >> pos.x >> pos.y >> pos.z;

		vertex.push_back(pos.x);
		vertex.push_back(pos.y);
		vertex.push_back(pos.z);
	}

	_mesh->SetVertex(&vertex);

	Scale(glm::vec3{ 0.1f });
	Move(pos);
}

Circle::~Circle()
{
}
