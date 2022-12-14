#include "pch.h"
#include "Map.h"

Map::Map(const std::string& path, glm::vec3 pos) :
	D3{ path, pos }
{
	_radius = glm::vec3{ 5.4, 9.372, 50 };
}

Map::~Map()
{
}
