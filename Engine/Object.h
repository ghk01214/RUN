#pragma once

// NOTE :
// 월드 상에 그려지는 모든 object들의 기본이 되는 object class
// 

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

class Object
{
public:
	Object();
	Object(glm::vec3 pos);
	virtual ~Object();

	virtual void OnLoad(std::shared_ptr<Shader>& shader);
	void OnRelease();

	void Transform(std::shared_ptr<Shader>& shader);
	
	virtual void Move(float x, float y, float z);
	virtual void Move(glm::vec3 delta);
	virtual void RotateX(float delta);
	virtual void RotateY(float delta);
	virtual void RotateZ(float delta);
	virtual void Scale(float x, float y, float z);
	virtual void Scale(glm::vec3 delta);

	virtual int32_t CheckCollision(Object* other);

	void BindVAO() { _mesh->BindVAO(); }
	void BindIndex() { _mesh->BindIndex(); }

	MAYBE_UNUSED void ApplyColor() { _material->ApplyColor(); }
	void ApplyLight() { _material->ApplyLight(); }
	void ChangeLightState() { _material->ChangeLightState(); }
	void TurnOnLight() { _material->TurnOnLight(); }
	void TurnOffLight() { _material->TurnOffLight(); }

	const size_t GetIndexNum() const { return _mesh->GetIndexNum(); }
	const glm::vec3 GetPos() { return _pos; }
	const glm::vec3 GetAngle() const { return _angle; }
	const glm::vec3 GetColor() const { return _material->GetColor(); }
	constexpr uint32_t GetDrawType() const { return _draw_type; }
	const glm::vec3 GetRadius() const { return _radius; }
	constexpr bool IsLightOn() const { return _material->IsLightOn(); }
	
	void SetPos(glm::vec3 pos) { _pos = pos; _model[3] = glm::vec4(pos, 1.f); }	// 수정(추가)
	void SetPos(float x, float y, float z) { SetPos(glm::vec3{ x, y, z }); }
	void SetObjectColor(glm::vec4 color) { _material->SetObjectColor(color); }
	void SetObjectColor(float r, float g, float b, float a) { SetObjectColor(glm::vec4(r, g, b, a)); }
	void SetObjectAlpha(float alpha) { _material->SetObjectAlpha(alpha); }
	void SetDrawType(uint32_t type) { _draw_type = type; }
	void SetShader(std::shared_ptr<Shader>& shader) { _material->SetShader(shader); }
	void SetLight(Light light) { _material->SetLight(light); }
	void SetLight(float ambient, float specular, int32_t shininess, glm::vec3 pos, glm::vec3 color) { _material->SetLight(ambient, specular, shininess, pos, color); }
	void SetAmbient(float ambient) { _material->SetAmbient(ambient); }
	void SetShininess(int32_t shininess) { _material->SetShininess(shininess); }
	void SetLightPos(glm::vec3 pos) { _material->SetLightPos(pos); }
	void SetLightPos(float x, float y, float z) { _material->SetLightPos(glm::vec3{ x, y, z }); }
	void SetLightColor(glm::vec3 color) { _material->SetLightColor(color); }
	void SetLightColor(float r, float g, float b) { _material->SetLightColor(glm::vec3{ r, g, b }); }

protected:
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;

	glm::vec3 _pos;
	glm::vec3 _angle;

	glm::mat4 _model;
	std::list<glm::mat4> _transform;

	uint32_t _draw_type;
	glm::vec3 _radius;
};
