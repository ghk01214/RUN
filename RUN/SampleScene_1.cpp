#include "pch.h"
#include <Shader.h>
#include <Figure.h>
#include <Camera.h>
#include "SampleScene_1.h"
#include <Engine.h>

extern Engine engine;

SampleScene_1::SampleScene_1() :
#pragma region [BASE VARIABLE]
	_camera{ std::make_shared<Camera>(glm::vec3{ 3.f, 3.f, 3.f }, -90.f - 45.f, -30.f) },
	_color_shader{ std::make_shared<Shader>() },
	_stop_animation{ false },
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
#else
	_color_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Color.glsl");
#endif
	CreateObjects();
	CreateGrid();
	OnAnimate(0);
}

SampleScene_1::~SampleScene_1()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void SampleScene_1::OnLoad()
{
	LoadMultipleObject(&_object, _color_shader);
	LoadMultipleObject(&_grid, _color_shader);
}

// 동적할당한 모든 객체 할당 해제
void SampleScene_1::OnRelease()
{
	ReleaseMultipleObject(&_object);
	ReleaseMultipleObject(&_grid);
}

void SampleScene_1::OnIdleMessage()
{
	CalculateDeltaTime();
}

void SampleScene_1::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	switch (key)
	{
		case 'X':
		{
			RotateX(0, define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'x':
		{
			RotateX(0, define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'Y':
		{
			RotateY(0, define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'y':
		{
			RotateY(0, define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'N':
		{
			RotateX(1, define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'n':
		{
			RotateX(1, define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'B':
		{
			RotateY(1, define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'b':
		{
			RotateY(1, define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'R':
		{
			OrbitY(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'r':
		{
			OrbitY(define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'C':
		{
			ChangeRenderObject(0, OBJECT::CUBE_0);
			ChangeRenderObject(1, OBJECT::SPHERE_1);
		}
		break;
		case 'c':
		{
			ChangeRenderObject(0, OBJECT::CONE_0);
			ChangeRenderObject(1, OBJECT::CUBE_1);
		}
		break;
		case 'W':
		{
			Move(define::DIRECTION::DOWN);
		}
		break;
		case 'w':
		{
			Move(define::DIRECTION::UP);
		}
		break;
		case 'A':
		{
			Move(define::DIRECTION::LEFT);
		}
		break;
		case 'a':
		{
			Move(define::DIRECTION::RIGHT);
		}
		break;
		case 'S':
		{
			Move(define::DIRECTION::BACK);
		}
		break;
		case 's':
		{
			Move(define::DIRECTION::FRONT);
		}
		break;
	}

}

void SampleScene_1::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	_camera->OnSpecialKeyMessage(key, x, y, _delta_time);
}

void SampleScene_1::OnMouseMessage(int32_t button, int32_t x, int32_t y)
{
	float x2{ Convert::ToFloat(x) / (engine.GetWindowWidth() / 2) - 1.f };
	float y2{ 1.f - Convert::ToFloat(y) / (engine.GetWindowHeight() / 2) };

	if (button == GLUT_LEFT_BUTTON)
	{
		_click = true;
	}
}

void SampleScene_1::OnMouseMotionMessage(int32_t x, int32_t y)
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

void SampleScene_1::OnMouseUpMessage(int32_t button, int32_t x, int32_t y)
{
}

void SampleScene_1::OnAnimate(int32_t index)
{

	if (_render_object[1]->CheckCollision(_render_object[0])) {
		std::cout << "collide" << std::endl;
		_render_object[0]->SetColor(RAND_COLOR);
	}

	glutTimerFunc(_animation_speed, Engine::OnAnimate, index);
}

void SampleScene_1::OnRender()
{
	glClearColor(BLACK, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TODO : Object render
	RenderMultipleObject(&_grid, _color_shader);
	RenderMultipleObject(&_render_object, _color_shader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

// Delta time 계산 함수
void SampleScene_1::CalculateDeltaTime()
{
	_time = glutGet(GLUT_ELAPSED_TIME);

	_delta_time = Convert::ToFloat((_time - _old_time)) / 1000.f;
	_old_time = _time;
}

void SampleScene_1::LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	object->OnLoad(shader);
}

void SampleScene_1::ReleaseSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	delete object;
	object = nullptr;
}

void SampleScene_1::LoadMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
{
	for (auto& obj : *object)
	{
		obj->OnLoad(shader);
	}
}

void SampleScene_1::ReleaseMultipleObject(std::vector<Object*>* object)
{
	// 객체들은 Object class를 동적할당 해서 생성했으므로 할당 해제
	for (auto& obj : *object)
	{
		delete obj;
		obj = nullptr;
	}
}

void SampleScene_1::ViewProjection(std::shared_ptr<Shader>& shader)
{
	auto view{ _camera->GetViewMatrix() };
	auto projection{ _camera->GetProjectionMatrix() };

	shader->SetMat4("view", &view);
	shader->SetMat4("projection", &projection);
}

void SampleScene_1::RenderSingleObject(Object* object, std::shared_ptr<Shader>& shader)
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

void SampleScene_1::RenderMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
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

#pragma endregion
#pragma region [USER-DEFINED METHOD]

void SampleScene_1::CreateObjects()
{
	_object.resize(4, nullptr);

	_object[CUBE_0] = new Cube{};
	_object[CUBE_0]->Move(glm::vec3(-1.5f, 0.5f, -0.5f));
	_object[CUBE_0]->SetShader(_color_shader);
	_object[CUBE_0]->SetColor(RAND_COLOR);

	_object[CONE_0] = new Sphere{};
	_object[CONE_0]->Scale(glm::vec3(-0.5f, -0.5f, -0.5f));
	_object[CONE_0]->Move(glm::vec3(-1.5f, 0.5f, -0.5f));
	_object[CONE_0]->SetShader(_color_shader);
	_object[CONE_0]->SetColor(RAND_COLOR);

	_object[SPHERE_1] = new Sphere{};
	_object[SPHERE_1]->Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	_object[SPHERE_1]->Move(glm::vec3(1.5f, 0.5f, -0.5f));
	_object[SPHERE_1]->SetShader(_color_shader);
	_object[SPHERE_1]->SetColor(RAND_COLOR);

	_object[CUBE_1] = new Cube{};
	_object[CUBE_1]->Move(glm::vec3(1.5f, 0.5f, -0.5f));
	_object[CUBE_1]->SetShader(_color_shader);
	_object[CUBE_1]->SetColor(RAND_COLOR);

	_render_object.resize(2, nullptr);

	_render_object[0] = _object[CUBE_0];
	_render_object[1] = _object[SPHERE_1];

}

void SampleScene_1::CreateGrid()
{
	_grid.resize(3, nullptr);

	// x축
	_grid[0] = new Line{};
	_grid[0]->SetShader(_color_shader);
	_grid[0]->SetColor(RAND_COLOR);

	// y축
	_grid[1] = new Line{};
	_grid[1]->RotateZ(90.f);
	_grid[1]->SetShader(_color_shader);
	_grid[1]->SetColor(RAND_COLOR);

	// z축
	_grid[2] = new Line{};
	_grid[2]->RotateY(90.f);
	_grid[2]->SetShader(_color_shader);
	_grid[2]->SetColor(RAND_COLOR);
}

void SampleScene_1::RotateX(int32_t index, define::ROTATE_DIRECTION direction)
{
	auto pos{ _render_object[index]->GetPos() };

	_render_object[index]->Move(-pos);

	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[index]->RotateX(direction);
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[index]->RotateX(direction);

	_render_object[index]->Move(pos);
}

void SampleScene_1::OrbitY(define::ROTATE_DIRECTION direction)
{
	for (auto& obj : _render_object)
	{
		// y축에 대하여 양(시계)의 방향으로 회전
		if (direction == define::ROTATE_DIRECTION::CLOCK)
			obj->RotateY(direction);
		// y축에 대하여 음(반시계)의 방향으로 회전
		else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
			obj->RotateY(direction);
	}
}

void SampleScene_1::RotateY(int32_t index, define::ROTATE_DIRECTION direction)
{
	auto pos{ _render_object[index]->GetPos() };

	_render_object[index]->Move(-pos);

	// y축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[index]->RotateY(direction);
	// y축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[index]->RotateY(direction);

	_render_object[index]->Move(pos);
}

void SampleScene_1::ChangeRenderObject(int index, OBJECT obj_type)
{
	// 화면에 그릴 객체를 obj_type으로 변경
	_render_object[index] = _object[obj_type];
	_render_object[index] = _object[obj_type];
}

void SampleScene_1::Move(define::DIRECTION direction)
{
	switch (direction)
	{
		case define::DIRECTION::LEFT:
		{
			// 화면에 그려지는 객체를 x축의 방향으로 -0.1만큼 이동
			_render_object[1]->Move(vec3::left(0.1f));
		}
		break;
		case define::DIRECTION::RIGHT:
		{
			// 화면에 그려지는 객체를 x축의 방향으로 +0.1만큼 이동
			_render_object[1]->Move(vec3::right(0.1f));
		}
		break;
		case define::DIRECTION::UP:
		{
			// 화면에 그려지는 객체를 y축의 방향으로 +0.1만큼 이동
			_render_object[1]->Move(vec3::up(0.01f));
		}
		break;
		case define::DIRECTION::DOWN:
		{
			// 화면에 그려지는 객체를 y축의 방향으로 -0.1만큼 이동
			_render_object[1]->Move(vec3::down(0.01f));
		}
		break;
		case define::DIRECTION::FRONT:
		{
			_render_object[1]->Move(vec3::front(0.01f));
		}
		break;
		case define::DIRECTION::BACK:
		{
			_render_object[1]->Move(vec3::back(0.01f));
		}
		break;
	}
	std::cout << _render_object[1]->GetPos().x << std::endl;
}
#pragma endregion