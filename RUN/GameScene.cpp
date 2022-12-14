#include "pch.h"
#include <Shader.h>
#include <Figure.h>
#include <Camera.h>
#include "Map.h"
#include "GameScene.h"
#include <Engine.h>

extern Engine engine;

GameScene::GameScene() :
#pragma region [BASE VARIABLE]
	_camera{ std::make_shared<Camera>(glm::vec3{ 0.f, 1.f, 2.f }) },
	_shader{ std::make_shared<Shader>() },
	_stop_animation{ true },
	_animation_speed{ 10 },
	_click{ false },
	_old_x{ 0 },
	_old_y{ 0 },
	_time{ glutGet(GLUT_ELAPSED_TIME) },
	_old_time{ _time },
	_delta_time{ 0.f },
	_frame{ 0 },
	_frame_time{ _time },
	_fps{ 0.f },
#pragma endregion
#pragma region [USER-DEFINE METHOD]
	_sphere{ nullptr },
	_jumping{ false },
	_jump_speed{ 4.f },
	_jump_pos{ 0.f },
	_gravity{ 9.8f }
#pragma endregion
{
#if _DEBUG
	_shader->OnLoad("../Dependencies/shader/Vertex.glsl", "../Dependencies/shader/Light.glsl");
#else
	_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Light.glsl");
#endif

	CreateMap();
	CreateObjects();
}

GameScene::~GameScene()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void GameScene::OnLoad()
{
	LoadSingleObject(_sphere, _shader);
	LoadMultipleObject(&_map, _shader);
}

// 동적할당한 모든 객체 할당 해제
void GameScene::OnRelease()
{
	ReleaseSingleObject(_sphere);
	ReleaseMultipleObject(&_map);
}

void GameScene::OnIdleMessage()
{
	CalculateDeltaTimeNFPS();

	OnKeyboardPressedMessage();
	OnSpecialKeyPressedMessage();

	if (_jumping == true)
		Jump();
}

void GameScene::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	_key.insert(key);
}

void GameScene::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	_special.insert(key);


	//_camera->OnSpecialKeyMessage(key, x, y, _delta_time);
}

void GameScene::OnKeyboardPressedMessage()
{
}

void GameScene::OnSpecialKeyPressedMessage()
{
	if (_special.contains(GLUT_KEY_LEFT))
		Move(define::DIRECTION::LEFT);
	if (_special.contains(GLUT_KEY_RIGHT))
		Move(define::DIRECTION::RIGHT);
	if (_special.contains(GLUT_KEY_UP))
		Move(define::DIRECTION::FRONT);
	if (_special.contains(GLUT_KEY_DOWN))
		Move(define::DIRECTION::BACK);
	if (_special.contains(GLUT_KEY_PAGE_UP))
		Move(define::DIRECTION::UP);
	if (_special.contains(GLUT_KEY_PAGE_DOWN))
		Move(define::DIRECTION::DOWN);
}

void GameScene::OnKeyboardUpMessage(uchar key, int32_t x, int32_t y)
{
	_key.erase(key);
}

void GameScene::OnSpecialKeyUpMessage(int32_t key, int32_t x, int32_t y)
{
	_special.erase(key);
}

void GameScene::OnMouseMessage(int32_t button, int32_t x, int32_t y)
{
	float x2{ Convert::ToFloat(x) / (engine.GetWindowWidth() / 2) - 1.f };
	float y2{ 1.f - Convert::ToFloat(y) / (engine.GetWindowHeight() / 2) };

	if (button == GLUT_LEFT_BUTTON)
	{
		_click = true;
	}
}

void GameScene::OnMouseMotionMessage(int32_t x, int32_t y)
{
	float x2{ Convert::ToFloat(x) / (engine.GetWindowWidth() / 2) - 1.f };
	float y2{ 1.f - Convert::ToFloat(y) / (engine.GetWindowHeight() / 2) };

	if (_click)
	{
		_old_x = x;
		_old_y = y;
		_click = false;
	}

	float delta_x{ Convert::ToFloat(x - _old_x) };
	float delta_y{ Convert::ToFloat(_old_y - y) };
	_old_x = x;
	_old_y = y;

	_camera->OnMouseMotionMessage(delta_x, delta_y);
}

void GameScene::OnMouseUpMessage(int32_t button, int32_t x, int32_t y)
{
}

void GameScene::OnAnimate(int32_t index)
{
	// 조건
	if (_stop_animation == false)
	{

		glutTimerFunc(_animation_speed, Engine::OnAnimate, index);
	}
}

void GameScene::OnRender()
{
	glClearColor(BLACK, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TODO : Object render
	RenderSingleObject(_sphere, _shader);
	RenderMultipleObject(&_map, _shader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

// Delta time 계산 함수
void GameScene::CalculateDeltaTimeNFPS()
{
	++_frame;
	_time = glutGet(GLUT_ELAPSED_TIME);

	_delta_time = Convert::ToFloat((_time - _old_time)) / 1000.f;

	if (_time - _frame_time > 1000)
	{
		_fps = _frame * 1000.f / Convert::ToFloat((_time - _frame_time));
		_frame_time = _time;
		_frame = 0;
	}

	_old_time = _time;
}

// 단일 객체를 load하는 함수
void GameScene::LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	object->OnLoad(shader);
}

// 동적할당된 단일 객체를 할당 해제하는 함수
void GameScene::ReleaseSingleObject(Object* object)
{
	delete object;
	object = nullptr;
}

// 복수의 object들이 사용돼서 하나의 객체를 이루는 객체를 load하는 함수
void GameScene::LoadMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
{
	for (auto& obj : *object)
	{
		obj->OnLoad(shader);
	}
}

// 복수의 object들이 동적할당 되어 하나의 객체를 이루는 객체를 할당 해제하는 함수
void GameScene::ReleaseMultipleObject(std::vector<Object*>* object)
{
	// 객체들은 Object class를 동적할당 해서 생성했으므로 할당 해제
	for (auto& obj : *object)
	{
		delete obj;
		obj = nullptr;
	}
}

// 뷰 및 투영 변환을 하는 method
void GameScene::ViewProjection(std::shared_ptr<Shader>& shader)
{
	auto view{ _camera->GetViewMatrix() };
	auto projection{ _camera->GetProjectionMatrix() };

	shader->SetMat4("view", &view);
	shader->SetMat4("projection", &projection);
}

// 단일 객체를 rendering하는 method
void GameScene::RenderSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	// shader program 사용
	shader->OnUse();
	// 뷰 변환 및 투영 변환 적용
	ViewProjection(shader);

	// VAO 바인드
	object->BindVAO();

	// 월드 변환
	object->Transform(shader);
	object->ApplyLight();

	glDrawElements(object->GetDrawType(), object->GetIndexNum(), GL_UNSIGNED_INT, 0);
}

// 복수의 object들이 사용된 객체를 rendering하는 method
void GameScene::RenderMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
{
	// shader program 사용
	shader->OnUse();
	// 뷰 변환 및 투영 변환 적용
	ViewProjection(shader);

	for (auto& obj : *object)
	{
		// VAO 바인드
		obj->BindVAO();

		// 월드 변환
		obj->Transform(shader);
		obj->ApplyLight();

		glDrawElements(obj->GetDrawType(), obj->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}
}

void GameScene::CreateMap()
{
	for (int32_t i = 0; i < 30; ++i)
	{
		std::string path{ "../Dependencies/model/map/" + std::to_string(i + 1) + ".obj" };

		_map.push_back(new Map{ path });
		_map.back()->SetShader(_shader);
		_map.back()->SetObjectColor(RAND_COLOR, 1.f);
		_map.back()->Move(vec3::front(50.f * (2 * i - 1)));
		
		_map_line.push_back(new Map{ path });
		_map_line.back()->SetShader(_shader);
		_map_line.back()->SetObjectColor(BLACK, 1.f);
		_map_line.back()->Scale(glm::vec3{ 0.9 });
		_map_line.back()->Move(vec3::front(50.f * (2 * i - 1)));
	}
}

void GameScene::CreateObjects()
{
	_sphere = new Sphere{};
	_sphere->SetShader(_shader);
	_sphere->SetObjectColor(RAND_COLOR, 1.f);
}

void GameScene::Move(define::DIRECTION direction)
{
	switch (direction)
	{
		case define::DIRECTION::LEFT:
		{
			_sphere->Move(vec3::left(0.1f));
		}
		break;
		case define::DIRECTION::RIGHT:
		{
			_sphere->Move(vec3::right(0.1f));
		}
		break;
		case define::DIRECTION::UP:
		{
			/*if (_jumping == false)
			{
				_jump_speed = 4.f;
				_jump_pos = _sphere->GetPos().y;
				_jumping = true;
			}*/
			_sphere->Move(vec3::up(0.1f));
		}
		break;
		case define::DIRECTION::DOWN:
		{
			_sphere->Move(vec3::down(0.1f));
		}
		case define::DIRECTION::FRONT:
		{
			_sphere->Move(vec3::front(0.1f));
		}
		break;
		case define::DIRECTION::BACK:
		{
			_sphere->Move(vec3::back(0.1f));
		}
		break;
		default:
		break;
	}
}

void GameScene::Jump()
{
	auto pos{ _sphere->GetPos().y };

	if (pos + _jump_speed * _delta_time < 0)
	{
		_jumping = false;

		_sphere->Move(vec3::up(_jump_pos - pos));

		return;
	}

	_sphere->Move(vec3::up(_jump_speed * _delta_time));
	_jump_speed -= _gravity * _delta_time;
}
