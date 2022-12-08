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

	virtual void CheckCollision(Object* other);

	void BindVAO() { _mesh->BindVAO(); }
	void BindIndex() { _mesh->BindIndex(); }

	void ApplyColor() { _material->ApplyColor(); }

	size_t GetIndexNum() { return _mesh->GetIndexNum(); }
	glm::vec3 GetPos() { return _pos = _model[3]; }	// 편집(삭제)
	glm::vec3 GetAngle() { return _angle; }
	glm::vec3 GetColor() { return _material->GetColor(); }
	uint32_t GetDrawType() { return _draw_type; }
	
	void SetPos(glm::vec3 pos) { _pos = pos; /*_model[3] = glm::vec4(pos, 1.f); */} // 편집(추가)
	void SetPos(float x, float y, float z) { SetPos(glm::vec3{ x, y, z }); }
	void SetColor(glm::vec3 color) { _material->SetColor(color); }
	void SetColor(float r, float g, float b) { SetColor(glm::vec3(r, g, b)); }
	void SetDrawType(uint32_t type) { _draw_type = type; }
	void SetShader(std::shared_ptr<Shader>& shader) { _material->SetShader(shader); }

protected:
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;

	glm::vec3 _pos;
	glm::vec3 _angle;

	glm::mat4 _model;
	std::list<glm::mat4> _transform;

	uint32_t _draw_type;
};
