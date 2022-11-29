#pragma once

// NOTE :
// 월드 상에 삼각형을 그려주는 object class를 상속받은 triangle class
// 

#include "D2.h"

class Triangle : public D2
{
public:
	Triangle(glm::vec3 pos = vec3::zero());
	~Triangle();
};
