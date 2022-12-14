#include "pch.h"
#include "Object.h"

std::uniform_real_distribution<float> uid_color{ 0.f, 1.f };

Object::Object() :
	_mesh{ std::make_shared<Mesh>() },
	_material{ std::make_shared<Material>() },
	_pos{ vec3::zero() },
	_angle{ vec3::zero() },
	_model{ mat4::unit() },
	_transform{},
	_draw_type{ GL_TRIANGLES },
	_radius{ vec3::unit() }
{
}

Object::Object(glm::vec3 pos) :
	_mesh{ std::make_shared<Mesh>() },
	_material{ std::make_shared<Material>() },
	_pos{ pos },
	_angle{ vec3::zero() },
	_model{ mat4::unit() },
	_transform{},
	_draw_type{ GL_TRIANGLES }
{
	Move(pos);
}

Object::~Object()
{
	OnRelease();
}

void Object::OnLoad(std::shared_ptr<Shader>& shader)
{
	// 객체의 VAO, VBO 생성
	_mesh->CreateBuffer();
}

void Object::OnRelease()
{
	// 객체의 VAO, VBO 삭제
	_mesh->OnRelease();
}

void Object::Transform(std::shared_ptr<Shader>& shader)
{
	// 모델 변환을 실행하는 함수
	auto model{ mat4::unit() };

	// 변환 값을 가지고 있는 연결 리스트의 뒷부분부터 단위 행렬에 곱하기 시작한다
	// 그렇게하면 가장 먼저 적용하고 싶은 변환이 제일 먼저 행렬에 적용된다
	for (auto iter = _transform.rbegin(); iter != _transform.rend(); ++iter)
	{
		model *= *iter;
	}

	// 변환 값들을 다 곱하면 마지막에 기존의 변환 행렬을 곱한다
	_model = model * _model;
	_pos = _model[3];

	// 연결 리스트 초기화
	_transform.clear();

	// 변환이 적용된 변환 행렬 셰이더에 적용
	shader->SetMat4("model", &_model);
}

void Object::Move(float x, float y, float z)
{
	Move(glm::vec3{ x, y, z });
}

void Object::Move(glm::vec3 delta)
{
	// 이동 행렬을 연결 리스트에 추가
	_transform.push_back(glm::translate(mat4::unit(), delta));
	
	// 추가 
	_pos += delta;
}

void Object::RotateX(float delta)
{
	// x축 회전 행렬을 연결 리스트에 추가
	_angle.x += delta;

	_transform.push_back(glm::rotate(mat4::unit(), glm::radians(delta), vec3::x()));
}

void Object::RotateY(float delta)
{
	// y축 회전 행렬을 연결 리스트에 추가
	_angle.y += delta;

	_transform.push_back(glm::rotate(mat4::unit(), glm::radians(delta), vec3::y()));
}

void Object::RotateZ(float delta)
{
	// z축 회전 행렬을 연결 리스트에 추가
	_angle.z += delta;
	
	_transform.push_back(glm::rotate(mat4::unit(), glm::radians(delta), vec3::z()));
}

void Object::Scale(float x, float y, float z)
{
	Scale(glm::vec3{ x, y, z });
}

void Object::Scale(glm::vec3 delta)
{
	_radius *= delta;

	// 신축 행렬을 연결 리스트에 추가
	_transform.push_back(glm::scale(mat4::unit(), delta));
}

int32_t Object::CheckCollision(Object* other)
{
	// TODO : 충돌처리 함수 작성
	if ((other->GetPos().x + other->GetRadius().x < GetPos().x - _radius.x)
		or (other->GetPos().x - other->GetRadius().x > GetPos().x + _radius.x))
		return 1;

	if ((other->GetPos().y + other->GetRadius().y < GetPos().y - _radius.y)
		or (other->GetPos().y - other->GetRadius().y > GetPos().y + _radius.y))
		return 2;

	if ((other->GetPos().z + other->GetRadius().z < GetPos().z - _radius.z)
		or (other->GetPos().z - other->GetRadius().z > GetPos().z + _radius.z))
		return 3;

	return 0;	// 위에서 검사 한것이 모두 아니면 충돌!
}
