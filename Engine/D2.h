#pragma once

#include "Object.h"

class D2 : public Object
{
public:
	D2() = default;
	~D2() = default;

	virtual void OnLoad(std::shared_ptr<Shader>& shader) override;
};
