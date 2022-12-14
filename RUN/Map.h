#pragma once

#include <D3.h>

class Map : public D3
{
public:
	Map(const std::string& path, glm::vec3 pos = vec3::zero());
	~Map();
};

