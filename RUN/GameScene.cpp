#include "pch.h"
#include <Shader.h>
#include <Figure.h>
#include <Camera.h>
#include "GameScene.h"
#include <Engine.h>

extern Engine engine;

GameScene::GameScene() :
#pragma region [BASE VARIABLE]
	_camera{ std::make_shared<Camera>(glm::vec3{ 0.f, 1.f, 2.f }, vec3::up(), 0.f, -90.f) },
	_color_shader{ std::make_shared<Shader>() },
	_light_shader{ std::make_shared<Shader>() },
	_stop_animation{ true },
	_animation_speed{ 100 },
	_click{ false },
	_old_x{ 0 },
	_old_y{ 0 },
	_time{ glutGet(GLUT_ELAPSED_TIME) },
	_old_time{ _time },
	_delta_time{ 0.f }
#pragma endregion
#pragma region [USER-DEFINE METHOD]
#pragma endregion
{
#if _DEBUG
	_color_shader->OnLoad("../Dependencies/shader/Vertex.glsl", "../Dependencies/shader/Color.glsl");
	_light_shader->OnLoad("../Dependencies/shader/Vertex.glsl", "../Dependencies/shader/Light.glsl");
#else
	_color_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Color.glsl");
	_light_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Light.glsl");
#endif

}

GameScene::~GameScene()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void GameScene::OnLoad()
{
	
}

// 동적할당한 모든 객체 할당 해제
void GameScene::OnRelease()
{
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
	_camera->OnSpecialKeyMessage(key, x, y, _delta_time);
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
}

// Delta time 계산 함수
void GameScene::CalculateDeltaTime()
{
	_time = glutGet(GLUT_ELAPSED_TIME);

	_delta_time = Convert::ToFloat((_time - _old_time)) / 1000.f;
	_old_time = _time;
}
#pragma endregion