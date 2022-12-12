#include "pch.h"
#include "Material.h"
#include "Shader.h"

Material::Material() :
	_color{ glm::vec4{ -1 } },
	_light{ 0.3f, 0.5f, 128, vec3::back(3.f), vec3::unit(), false }
{
}

Material::~Material()
{
}

void Material::ApplyColor()
{
	// 현재 객체에 저장된 색을 셰이더에 적용
	_shader->SetVec4("obj_color", &_color);
}

void Material::ApplyLight()
{
	_shader->SetVec4("obj_color", &_color);
	_shader->SetFloat("ambient_strength", _light.ambient);
	_shader->SetFloat("specular_strength", _light.specular);
	_shader->SetInt("shininess", _light.shininess);
	_shader->SetVec3("light_pos", &_light.pos);
	_shader->SetVec3("light_color", &_light.color);
	_shader->SetBool("light_on", _light.on);
}

void Material::SetObjectColor(glm::vec4 color)
{
	// 객체의 색을 저장하고 적용
	_color = color;

	ApplyColor();
}

void Material::SetObjectAlpha(float alpha)
{
	_color.a = alpha;

	ApplyColor();
}

void Material::SetLight(Light light)
{
	_light = light;

	ApplyLight();
}

void Material::SetAmbient(float ambient)
{
	_light.ambient = ambient;

	_shader->SetFloat("ambient_strength", _light.ambient);
}

void Material::SetSpecular(float specular)
{
	_light.specular = specular;

	_shader->SetFloat("specular_strength", _light.specular);
}

void Material::SetShininess(int32_t shininess)
{
	_light.shininess = shininess;

	_shader->SetInt("shininess", _light.shininess);
}

void Material::SetLightPos(glm::vec3 pos)
{
	_light.pos = pos;

	_shader->SetVec3("light_pos", &_light.pos);
}

void Material::SetLightColor(glm::vec3 color)
{
	_light.color = color;

	_shader->SetVec3("light_color", &_light.color);
}

void Material::ChangeLightState()
{
	_light.on = !_light.on;

	_shader->SetBool("light_on", _light.on);
}

void Material::TurnOnLight()
{
	_light.on = true;

	_shader->SetBool("light_on", _light.on);
}

void Material::TurnOffLight()
{
	_light.on = false;

	_shader->SetBool("light_on", _light.on);
}