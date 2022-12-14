#pragma once

// NOTE :
// 월드 상에 사각형을 그려주는 object class를 상속받은 rect class
// 

#include "D3.h"

class Rect : public D3
{
public:
	Rect(glm::vec3 pos = vec3::zero());
	~Rect();
};
