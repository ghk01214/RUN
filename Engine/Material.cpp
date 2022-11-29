#include "pch.h"
#include "Material.h"
#include "Shader.h"

Material::Material() :
	_color{ glm::vec3{ -1 } }
{
}

Material::~Material()
{
}

void Material::ApplyColor()
{
	// 현재 객체에 저장된 색을 셰이더에 적용
	_shader->SetVec3("obj_color", &_color);
}

void Material::SetObjectColor(glm::vec3 color)
{
	// 객체의 색을 저장하고 적용
	_color = color;

	ApplyColor();
}
