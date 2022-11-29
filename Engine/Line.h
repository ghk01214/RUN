#pragma once

// NOTE :
// 월드 상에 선을 그려주는 object class를 상속받은 line class
// 

#include "D2.h"

class Line : public D2
{
public:
	Line(glm::vec3 pos = vec3::zero(), glm::vec3 length = vec3::right(5.f));
	~Line();
};

