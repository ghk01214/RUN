#include "pch.h"
#include <Shader.h>
#include <Figure.h>
#include <Camera.h>
#include "GameScene.h"
#include <Engine.h>

extern Engine engine;

GameScene::GameScene() :
#pragma region [BASE VARIABLE]
	_camera{ std::make_shared<Camera>(glm::vec3{ 0.f, 0.f, 2.f }) }, // 초기값 0 1 2
	_color_shader{ std::make_shared<Shader>() },
	_stop_animation{ true },
	_animation_speed{ 10 },
	_click{ false },
	_old_x{ 0 },
	_old_y{ 0 },
	_time{ glutGet(GLUT_ELAPSED_TIME) },
	_old_time{ _time },
	_delta_time{ 0.f },
	_wait_time{ 0.f },
#pragma endregion
#pragma region [USER-DEFINE METHOD]
	_sphere{ nullptr }
#pragma endregion
{
#if _DEBUG
	_color_shader->OnLoad("../Dependencies/shader/Vertex.glsl", "../Dependencies/shader/Color.glsl");
#else
	_color_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Color.glsl");
#endif
	CreateObjects();
	CreateGrid();
}

GameScene::~GameScene()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void GameScene::OnLoad()
{
	LoadSingleObject(_sphere, _color_shader);
	LoadMultipleObject(&_grid, _color_shader);
}

// 동적할당한 모든 객체 할당 해제
void GameScene::OnRelease()
{
	ReleaseSingleObject(_sphere, _color_shader);
	ReleaseMultipleObject(&_grid);
}

void GameScene::OnIdleMessage()
{
	CalculateDeltaTime();
}

void GameScene::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{

}

void GameScene::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	if (_wait_time < 0.001f)
	{
		_wait_time += _delta_time;
		return;
	}

	// 동시입력 받고 싶으면 switch문 대신 if문 쓰기
		if(key == GLUT_KEY_LEFT)
		{
			Move(define::DIRECTION::LEFT);
		}
		if (key == GLUT_KEY_RIGHT)
		{
			Move(define::DIRECTION::RIGHT);
		}
		if (key == GLUT_KEY_UP)
		{
			Move(define::DIRECTION::UP);
		}
		if (key == GLUT_KEY_DOWN)
		{
			Move(define::DIRECTION::DOWN);
		}

	//_camera->OnSpecialKeyMessage(key, x, y, _delta_time);
	_wait_time = 0;
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
	else
		return;
	auto pos{ _sphere->GetPos().y };
	// 	_jump_pos = _sphere->GetPos().y;
	if (pos + _jump_speed * _delta_time * 1.5f < 0)
	{
		_stop_animation = true;
		//_sphere->Move(vec3::up(_jump_pos - pos));
		_jumping = false;

		_sphere->SetPos(_sphere->GetPos().x, 0.f, _sphere->GetPos().z);
		//_sphere->Move(vec3::up(_jump_pos - pos));


		return;
	}
	
	_sphere->Move(vec3::up(_jump_speed * _delta_time * 1.5f));
	_jump_speed -= _gravity * _delta_time * 1.5f;

}

void GameScene::OnRender()
{
	glClearColor(BLACK, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TODO : Object render
	RenderMultipleObject(&_grid, _color_shader);
	RenderSingleObject(_sphere, _color_shader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

// Delta time 계산 함수
void GameScene::CalculateDeltaTime()
{
	_time = glutGet(GLUT_ELAPSED_TIME);

	_delta_time = Convert::ToFloat((_time - _old_time)) / 1000.f;
	_old_time = _time;
}

// 단일 객체를 load하는 함수
void GameScene::LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	object->OnLoad(shader);
}

// 동적할당된 단일 객체를 할당 해제하는 함수
void GameScene::ReleaseSingleObject(Object* object, std::shared_ptr<Shader>& shader)
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

	// 객체의 색상을 셰이더에 적용
	object->ApplyColor();

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

		// 객체의 색상을 셰이더에 적용
		obj->ApplyColor();

		glDrawElements(obj->GetDrawType(), obj->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}
}

void GameScene::CreateObjects()
{
	_sphere = new Sphere{};
	_sphere->Scale(glm::vec3{ 0.1f });
	_sphere->SetShader(_color_shader);
	_sphere->SetColor(RAND_COLOR);
	_sphere->Scale(0.1f, 0.1f, 0.1f);
}

void GameScene::CreateGrid()
{
	_grid.resize(2, nullptr);

	// x축
	_grid[0] = new Line{};
	_grid[0]->SetShader(_color_shader);
	_grid[0]->SetColor(RAND_COLOR);

	// y축
	_grid[1] = new Line{};
	_grid[1]->RotateZ(90.f);
	_grid[1]->SetShader(_color_shader);
	_grid[1]->SetColor(RAND_COLOR);
}

void GameScene::ChangeRenderObject(OBJECT obj_type)
{
	//_render_object = _object[obj_type];
}

void GameScene::Move(define::DIRECTION direction)
{
	if (direction == define::DIRECTION::LEFT) {
		_sphere->Move(vec3::left(0.01f));
	}
	if (direction == define::DIRECTION::RIGHT)
	{
		_sphere->Move(vec3::right(0.01f));
	}
	if (direction == define::DIRECTION::UP)
	{
		if (_jumping == false) {
			_stop_animation = false;
			_jump_speed = 4.f;
			_jump_pos = _sphere->GetPos().y;
			_jumping = true;

			glutTimerFunc(_animation_speed, Engine::OnAnimate, 1);

		}
	}
	if (direction == define::DIRECTION::DOWN)
	{
		// 사용 x
	}
}