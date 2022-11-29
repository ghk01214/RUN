#pragma once

#include "Object.h"

class D3 : public Object
{
public:
	D3() = default;
	D3(const std::string& path, glm::vec3 pos = vec3::zero());
	~D3() = default;

	virtual void OnLoad(std::shared_ptr<Shader>& shader) override;
};
