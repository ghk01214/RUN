#include "pch.h"
#include <Shader.h>
#include <Figure.h>
#include <Camera.h>
#include "SampleScene.h"
#include <Engine.h>

extern Engine engine;

SampleScene::SampleScene() :
#pragma region [BASE VARIABLE]
	_camera{ std::make_shared<Camera>(glm::vec3{ 3.f, 3.f, 3.f }, -90.f - 45.f, -30.f) },
	_color_shader{ std::make_shared<Shader>() },
	_stop_animation{ true },
	_animation_speed{ 100 },
	_click{ false },
	_old_x{ 0 },
	_old_y{ 0 },
	_time{ glutGet(GLUT_ELAPSED_TIME) },
	_old_time{ _time },
	_delta_time{ 0.f },
#pragma endregion
#pragma region [USER-DEFINE METHOD]
	_object{},
	_grid{},
	_render_object{ nullptr },
	_apply_cull_face{ true }
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

SampleScene::~SampleScene()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void SampleScene::OnLoad()
{
	// 새로운 객체 생성 시 아래 작업 필요
	// load할 객체의 reference 값과 객체에 적용시킬 shader를 인자로 준다
	LoadMultipleObject(&_object, _color_shader);
	LoadMultipleObject(&_grid, _color_shader);
}

// 동적할당한 모든 객체 할당 해제
void SampleScene::OnRelease()
{
	// 새로운 객체 생성 시 아래 작업 필요
	ReleaseMultipleObject(&_object);
	ReleaseMultipleObject(&_grid);
}

void SampleScene::OnIdleMessage()
{
	// delta time 계산기
	CalculateDeltaTime();
}

void SampleScene::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	switch (key)
	{
		case 'C': FALLTHROUGH
		case 'c':
		{
			ChangeRenderObject(OBJECT::CUBE);
		}
		break;
		case 'P': FALLTHROUGH
		case 'p':
		{
			ChangeRenderObject(OBJECT::TETRA);
		}
		break;
		case 'H': FALLTHROUGH
		case 'h':
		{
			ChangeCullFaceState();
		}
		break;
		case 'X':
		{
			RotateX(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'x':
		{
			RotateX(define::ROTATE_DIRECTION::COUNTER_CLOCK);			
		}
		break;
		case 'Y':
		{
			RotateY(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'y':
		{
			RotateY(define::ROTATE_DIRECTION::COUNTER_CLOCK);			
		}
		break;
		case 'W':
		{
			ChangeDrawType(GL_LINE_LOOP);
		}
		break;
		case 'w':
		{
			ChangeDrawType(GL_TRIANGLES);
		}
		break;
	}
}

void SampleScene::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
		{
			Move(define::DIRECTION::LEFT);
		}
		break;
		case GLUT_KEY_RIGHT:
		{
			Move(define::DIRECTION::RIGHT);
		}
		break;
		case GLUT_KEY_UP:
		{
			Move(define::DIRECTION::UP);
		}
		break;
		case GLUT_KEY_DOWN:
		{
			Move(define::DIRECTION::DOWN);
		}
		break;
	}

	//_camera->OnSpecialKeyMessage(key, x, y, _delta_time);
}

void SampleScene::OnMouseMessage(int32_t button, int32_t x, int32_t y)
{
	float x2{ Convert::ToFloat(x) / (engine.GetWindowWidth() / 2) - 1.f };
	float y2{ 1.f - Convert::ToFloat(y) / (engine.GetWindowHeight() / 2) };

	if (button == GLUT_LEFT_BUTTON)
	{
		_click = true;
	}
}

void SampleScene::OnMouseMotionMessage(int32_t x, int32_t y)
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

void SampleScene::OnMouseUpMessage(int32_t button, int32_t x, int32_t y)
{
}

void SampleScene::OnAnimate(int32_t index)
{
	if (_stop_animation == false)
	{
		glutTimerFunc(_animation_speed, Engine::OnAnimate, index);
	}
}

void SampleScene::OnRender()
{
	glClearColor(GRAY, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (_apply_cull_face == true)
		glEnable(GL_CULL_FACE);

	// TODO : Object render
	RenderMultipleObject(&_grid, _color_shader);
	RenderSingleObject(_render_object, _color_shader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

// Delta time 계산 함수
void SampleScene::CalculateDeltaTime()
{
	_time = glutGet(GLUT_ELAPSED_TIME);

	_delta_time = Convert::ToFloat((_time - _old_time)) / 1000.f;
	_old_time = _time;
}

// 단일 객체를 load하는 함수
void SampleScene::LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	object->OnLoad(shader);
}

// 동적할당된 단일 객체를 할당 해제하는 함수
void SampleScene::ReleaseSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	delete object;
	object = nullptr;
}

// 복수의 object들이 사용돼서 하나의 객체를 이루는 객체를 load하는 함수
void SampleScene::LoadMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
{
	for (auto& obj : *object)
	{
		obj->OnLoad(shader);
	}
}

// 복수의 object들이 동적할당 되어 하나의 객체를 이루는 객체를 할당 해제하는 함수
void SampleScene::ReleaseMultipleObject(std::vector<Object*>* object)
{
	// 객체들은 Object class를 동적할당 해서 생성했으므로 할당 해제
	for (auto& obj : *object)
	{
		delete obj;
		obj = nullptr;
	}
}

// 뷰 및 투영 변환을 하는 method
void SampleScene::ViewProjection(std::shared_ptr<Shader>& shader)
{
	auto view{ _camera->GetViewMatrix() };
	auto projection{ _camera->GetProjectionMatrix() };

	shader->SetMat4("view", &view);
	shader->SetMat4("projection", &projection);
}

// 단일 객체를 rendering하는 method
void SampleScene::RenderSingleObject(Object* object, std::shared_ptr<Shader>& shader)
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
void SampleScene::RenderMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
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

void SampleScene::CreateObjects()
{
	// 동적 배열에 OBJECT::MAX 만큼의 공간에 nullptr를 할당
	// 동적 배열이므로 OBJECT::MAX 만큼의 공간 뒤에 더 붙을 수 있다
	_object.resize(OBJECT::MAX, nullptr);

	_object[CUBE] = new Cube{};
	//_object[CUBE]->RotateY(-45.f);
	//_object[CUBE]->RotateX(30.f);
	//_object[CUBE]->SetShader(_color_shader);
	//_object[CUBE]->SetColor(RAND_COLOR);

	_object[TETRA] = new Tetraherdon{};
	//_object[TETRA]->RotateY(-45.f);
	//_object[TETRA]->RotateX(30.f);
	//_object[TETRA]->SetShader(_color_shader);
	//_object[TETRA]->SetColor(RAND_COLOR);

	// 위 주석친 부분과 동일
	// 이 for문의 의미는 _object 안에 있는 모든 원소에 대하여 루프를 돈다는 것이다
	for (auto& obj : _object)	// == for (int32_t i = 0; i < OBJECT::MAX; ++i)
	{
		// 각 객체를 먼저 y축을 기준으로 45도 반시계 방향으로 회전 시킨 뒤
		// x축을 기준으로 반시계 방향으로 30도 회전
		obj->RotateY(-45.f);
		obj->RotateX(30.f);
		// 해당 객체에서 사용할 shader를 결정(material/재질 정보를 위해서 필요)
		obj->SetShader(_color_shader);
		// 해당 객체의 색상 결정
		obj->SetColor(RAND_COLOR);
	}

	// 현재 화면에 그릴 객체를 육면체로 결정
	_render_object = _object[CUBE];
}

void SampleScene::CreateGrid()
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

void SampleScene::ChangeRenderObject(OBJECT obj_type)
{
	// 화면에 그릴 객체를 obj_type으로 변경
	_render_object = _object[obj_type];
}

void SampleScene::ChangeCullFaceState()
{
	// 은면제거 상태 변경
	// int = -int와 같은 원리
	_apply_cull_face = !_apply_cull_face;
}

void SampleScene::Move(define::DIRECTION direction)
{
	switch (direction)
	{
		case define::DIRECTION::LEFT:
		{
			// 화면에 그려지는 객체를 x축의 방향으로 -0.1만큼 이동
			_render_object->Move(vec3::left(0.1f));
		}
		break;
		case define::DIRECTION::RIGHT:
		{
			// 화면에 그려지는 객체를 x축의 방향으로 +0.1만큼 이동
			_render_object->Move(vec3::right(0.1f));
		}
		break;
		case define::DIRECTION::UP:
		{
			// 화면에 그려지는 객체를 y축의 방향으로 +0.1만큼 이동
			_render_object->Move(vec3::up(0.5f));
		}
		break;
		case define::DIRECTION::DOWN:
		{
			// 화면에 그려지는 객체를 y축의 방향으로 -0.1만큼 이동
			_render_object->Move(vec3::down(0.5f));
		}
		break;
	}
}

void SampleScene::RotateX(define::ROTATE_DIRECTION direction)
{
	// x축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object->RotateX(direction);
	// x축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object->RotateX(direction);
}

void SampleScene::RotateY(define::ROTATE_DIRECTION direction)
{
	// y축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object->RotateY(direction);
	// y축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object->RotateY(direction);
}

void SampleScene::ChangeDrawType(uint32_t type)
{
	// 각 객체들의 와이어/솔리드 방식 변경
	for (auto& obj : _object)
	{
		obj->SetDrawType(type);
	}
}

#pragma endregion