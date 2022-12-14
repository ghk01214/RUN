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
	_camera{ std::make_shared<Camera>(glm::vec3{ 0.f, 1.f, 10.f }) },
	_shader{ std::make_shared<Shader>() },
	_stop_animation{ true },
	_animation_speed{ 20 },
	_click{ false },
	_old_x{ 0 },
	_old_y{ 0 },
	_time{ glutGet(GLUT_ELAPSED_TIME) },
	_old_time{ 0 },
	_delta_time{ 0.f },
	_frame{ 0 },
	_frame_time{ _time },
	_fps{ 0.f },
#pragma endregion
#pragma region [USER-DEFINE METHOD]
	_player{ nullptr },
	_jumping{ false },
	_jump_speed{ 4.f },
	_jump_pos{ 0.f },
	_gravity{ 9.8f },
	_game_over{ false },
	_end_time{ 0.f },
	_status{},
	_fever{ false },
	_fever_time{ 0.f },
	_game_speed{ 0.5f },
	_replace{ 0.f },
	_light_pos{ -5.f, 5.f, -5.f }
#pragma endregion
{
#if _DEBUG
	_shader->OnLoad("../Dependencies/shader/Vertex.glsl", "../Dependencies/shader/Light.glsl");
#else
	_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Light.glsl");
#endif

	CreateMap();
	CreatePlayer();

	_stop_animation = false;
	glutTimerFunc(_animation_speed, Engine::OnAnimate, 1);
}

GameScene::~GameScene()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void GameScene::OnLoad()
{
	LoadSingleObject(_player, _shader);
	LoadMap(&_map, _shader);
}

// 동적할당한 모든 객체 할당 해제
void GameScene::OnRelease()
{
	ReleaseSingleObject(_player);
	ReleaseMap(&_map);
}

void GameScene::OnIdleMessage()
{
	CalculateDeltaTimeNFPS();

	if (_game_over == false)
	{
		OnKeyboardPressedMessage();
		OnSpecialKeyPressedMessage();

		if (_fever == false and _jumping == false)
		{
			_replace = _gravity * _delta_time * 0.5f;

			_player->Move(vec3::down(_replace));
			_camera->Move(define::DIRECTION::DOWN, _replace);
		}

		for (auto& map : _map)
		{
			CheckWorldCollision(map);
		}

		if (_jumping == true)
			Jump();

		CheckPlayerStatus();
		DetectGameOver();
	}
	else
	{
		if (_end_time > 5.f)
		{
			glutLeaveMainLoop();
			return;
		}

		_end_time += _delta_time;
	}
}

void GameScene::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	_key.insert(key);
}

void GameScene::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	_special.insert(key);

	//_camera->OnSpecialKeyMessage(key, x, y, _delta_time * 10.f);
}

void GameScene::OnKeyboardPressedMessage()
{
	if (_key.contains(0x20))
		Move(define::DIRECTION::UP);
}

void GameScene::OnSpecialKeyPressedMessage()
{
	if (_special.contains(GLUT_KEY_LEFT))
		Move(define::DIRECTION::LEFT);
	if (_special.contains(GLUT_KEY_RIGHT))
		Move(define::DIRECTION::RIGHT);
	if (_special.contains(GLUT_KEY_UP))
#if _DEBUG
		Move(define::DIRECTION::FRONT);
	if (_special.contains(GLUT_KEY_DOWN))
		Move(define::DIRECTION::BACK);
	if (_special.contains(GLUT_KEY_PAGE_UP))
		Move(define::DIRECTION::UP);
	if (_special.contains(GLUT_KEY_PAGE_DOWN))
		Move(define::DIRECTION::DOWN);
#else
		Move(define::DIRECTION::UP);
#endif
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
		MoveWorld();

		glutTimerFunc(_animation_speed, Engine::OnAnimate, index);
	}
}

void GameScene::OnRender()
{
	glClearColor(GRAY, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TODO : Object render
	RenderSingleObject(_player, _shader);
	RenderMap(&_map, _shader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

// Delta time 계산 함수
void GameScene::CalculateDeltaTimeNFPS()
{
	++_frame;
	_time = glutGet(GLUT_ELAPSED_TIME);

	if (_old_time == 0)
		_old_time = _time;

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
	glm::vec4 color{ 0.663865268f, 0.736751020f, 0.674442828f, 1.f };

	for (int32_t i = 0; i < 30; ++i)
	{
		_map.push_back(new Map{ _shader, color, vec3::front(5.f * i) });
	}

	_map[1]->ChangeTileColor(10, BLUE, _shader);

	//_map[2]->RemoveTile(1);
	_map[2]->RemoveTile(7);
	_map[2]->RemoveTile(13);
	_map[2]->ChangeTileColor(5, RED, _shader);
	_map[2]->ChangeTileColor(17, GREEN, _shader);

	_map[3]->RemoveTile(4);
	_map[3]->RemoveTile(10);
	_map[3]->RemoveTile(16);

	_map[4]->ChangeTileColor(2, BLUE, _shader);
	_map[4]->ChangeTileColor(6, GREEN, _shader);
	_map[4]->ChangeTileColor(14, RED, _shader);

	for (int32_t i = 0; i < 18; ++i)
	{
		if (i == 1 or i == 4 or i == 7 or i == 10 or i == 13 or i == 16)
			continue;

		_map[5]->RemoveTile(i);
	}

	_map[6]->ChangeTileColor(2, GREEN, _shader);
	_map[6]->ChangeTileColor(14, GREEN, _shader);

	_map[7]->AddBlock(1, _shader);
	_map[7]->AddBlock(4, _shader);
	_map[7]->AddBlock(7, _shader);
	_map[7]->AddBlock(10, _shader);
	_map[7]->AddBlock(13, _shader);
	_map[7]->AddBlock(16, _shader);

	_map[8]->ChangeTileColor(2, RED, _shader);
	_map[8]->ChangeTileColor(6, BLUE, _shader);
	_map[8]->ChangeTileColor(15, BLUE, _shader);

	for (int32_t i = 0; i < 9; ++i)
	{
		_map[9]->AddBlock(2 * i + 1, _shader);
	}

	_map[10]->ChangeTileColor(9, GREEN, _shader);

	_map[11]->ChangeTileColor(6, RED, _shader);

	_map[12]->ChangeTileColor(4, BLUE, _shader);
	_map[12]->ChangeTileColor(16, RED, _shader);
	for (int32_t i = 0; i < 3; ++i)
	{
		_map[12]->AddBlock(i, _shader);
	}
	for (int32_t i = 6; i < 9; ++i)
	{
		_map[12]->AddBlock(i, _shader);
	}
	for (int32_t i = 12; i < 15; ++i)
	{
		_map[12]->AddBlock(i, _shader);
	}

	for (int32_t i = 0; i < 18; ++i)
	{
		if (i == 1 or i == 10)
			continue;

		_map[14]->RemoveTile(i);
	}
	_map[14]->AddBlock(1, _shader);

	for (int32_t i = 0; i < 18; ++i)
	{
		if (i % 3 == 2)
			continue;

		_map[16]->AddBlock(i, _shader);
	}

	for (int32_t i = 0; i < 18; ++i)
	{
		if (i % 3 == 0)
			continue;

		_map[18]->AddBlock(i, _shader);
	}
	_map[18]->ChangeTileColor(3, RED, _shader);

	_map[20]->RemoveTile(3);
	_map[20]->RemoveTile(4);
	_map[20]->RemoveTile(5);
	_map[20]->RemoveTile(9);
	_map[20]->RemoveTile(10);
	_map[20]->RemoveTile(11);
	_map[20]->RemoveTile(15);
	_map[20]->RemoveTile(16);
	_map[20]->RemoveTile(17);
	_map[20]->ChangeTileColor(7, BLUE, _shader);
	_map[20]->ChangeTileColor(13, RED, _shader);

	_map[21]->ChangeTileColor(3, GREEN, _shader);
	_map[21]->AddBlock(8, _shader);

	for (int32_t i = 0; i < 15; ++i)
	{
		if (i == 1 or i == 4 or i == 7 or i == 10 or i == 13)
			continue;

		_map[22]->RemoveTile(i);
	}
	_map[22]->RemoveTile(16);

	_map[23]->RemoveTile(0);
	_map[23]->RemoveTile(2);
	_map[23]->RemoveTile(6);
	_map[23]->RemoveTile(8);
	_map[23]->RemoveTile(12);
	_map[23]->RemoveTile(14);
	_map[23]->ChangeTileColor(11, GREEN, _shader);

	_map[24]->RemoveTile(4);
	_map[24]->RemoveTile(10);
	_map[24]->RemoveTile(16);
	_map[24]->ChangeTileColor(15, BLUE, _shader);
	_map[24]->AddBlock(3, _shader);
	_map[24]->AddBlock(9, _shader);
	_map[24]->AddBlock(17, _shader);

	_map[26]->RemoveTile(0);
	_map[26]->RemoveTile(1);
	_map[26]->RemoveTile(5);
	_map[26]->RemoveTile(10);
	_map[26]->RemoveTile(12);
	_map[26]->ChangeTileColor(2, RED, _shader);
	_map[26]->AddBlock(6, _shader);

	_map[27]->RemoveTile(1);
	_map[27]->RemoveTile(5);
	_map[27]->RemoveTile(8);
	_map[27]->RemoveTile(11);
	_map[27]->RemoveTile(12);
	_map[27]->RemoveTile(13);
	_map[27]->ChangeTileColor(3, BLUE, _shader);
	_map[27]->AddBlock(16, _shader);

	_map[28]->AddBlock(7, _shader);
	_map[28]->AddBlock(9, _shader);
	_map[28]->AddBlock(13, _shader);

	_map[29]->RemoveTile(3);
	_map[29]->ChangeTileColor(12, GREEN, _shader);
	_map[29]->AddBlock(2, _shader);
	_map[29]->AddBlock(17, _shader);
}

void GameScene::CreatePlayer()
{
	_player = new Sphere{};
	_player->SetShader(_shader);
	_player->SetObjectColor(BLACK, 1.f);
	_player->Scale(glm::vec3{ 0.7f });
	_player->Move(vec3::up(0.35f));
	_player->SetLightPos(_light_pos);
	_player->TurnOnLight();

	_status.insert(std::make_pair("red", false));
	_status.insert(std::make_pair("green", false));
	_status.insert(std::make_pair("blue", false));
}

void GameScene::LoadMap(std::vector<Map*>* map, std::shared_ptr<Shader>& shader)
{
	for (auto& obj : *map)
	{
		obj->OnLoad(shader);
	}
}

void GameScene::ReleaseMap(std::vector<Map*>* map)
{
	for (auto& obj : *map)
	{
		obj->OnRelease();
	}
}

void GameScene::RenderMap(std::vector<Map*>* map, std::shared_ptr<Shader>& shader)
{
	// shader program 사용
	shader->OnUse();
	// 뷰 변환 및 투영 변환 적용
	ViewProjection(shader);

	for (auto& obj : *map)
	{
		obj->Render(shader, _fever);
	}
}

void GameScene::Move(define::DIRECTION direction)
{
	switch (direction)
	{
		case define::DIRECTION::LEFT:
		{
			_player->Move(vec3::left(0.1f));
			_camera->Move(define::DIRECTION::LEFT, 0.1f);
		}
		break;
		case define::DIRECTION::RIGHT:
		{
			_player->Move(vec3::right(0.1f));
			_camera->Move(define::DIRECTION::RIGHT, 0.1f);
		}
		break;
		case define::DIRECTION::UP:
		{
			if (_jumping == false)
			{
				_jump_speed = 4.f;
				_jump_pos = _player->GetPos().y;
				_jumping = true;
			}
		}
		break;
#if _DEBUG
		case define::DIRECTION::DOWN:
		{
			_player->Move(vec3::down(0.1f));
		}
		case define::DIRECTION::FRONT:
		{
			_player->Move(vec3::front(0.1f));
			_camera->Move(define::DIRECTION::FRONT, 0.1f);
		}
		break;
		case define::DIRECTION::BACK:
		{
			_player->Move(vec3::back(0.1f));
			_camera->Move(define::DIRECTION::BACK, 0.1f);
		}
		break;
#endif
		default:
		break;
	}
}

void GameScene::Jump()
{
	auto pos{ _player->GetPos().y };

	if (pos + _jump_speed * _delta_time < 0.f)
	{
		_jumping = false;

		_player->Move(vec3::up(_jump_pos - pos));
		_camera->Move(define::DIRECTION::UP, _jump_pos - pos);

		return;
	}

	_player->Move(vec3::up(_jump_speed * _delta_time));
	_camera->Move(define::DIRECTION::UP, _jump_speed * _delta_time);

	_jump_speed -= _gravity * _delta_time;
}

void GameScene::MoveWorld()
{
	auto temp_mat{ mat4::unit() };
	temp_mat[3] = glm::vec4{ _light_pos, 1.f };
	_light_pos = glm::rotate(temp_mat, glm::radians(-1.f), vec3::z())[3];

	_player->SetLightPos(_light_pos);

	for (int32_t i = 0; i < _map.size(); ++i)
	{
		if (_map[i]->GetPos() > 3.f)
		{
			if (i == _map.size() - 1)
			{
				if (_fever == false)
					_game_speed += 0.5f;
			}

			_map[i]->Reuse(5.f * _map.size(), _light_pos);
		}

		if (_fever == true)
		{
			_map[i]->Move(vec3::back(_game_speed + 2.f), _light_pos);
			continue;
		}

		auto result{ _map[i]->CheckItemCollision(_player) };

		if (result == 1)
		{
			if (_status["red"] == false)
			{
				glm::vec3 color{ RED };

				_player->SetObjectColor(glm::vec4{ color + _player->GetColor(), 1.f });
				_status["red"] = true;
			}
		}
		else if (result == 2)
		{
			if (_status["green"] == false)
			{
				glm::vec3 color{ GREEN };

				_player->SetObjectColor(glm::vec4{ color + _player->GetColor(), 1.f });
				_status["green"] = true;
			}
		}
		else if (result == 3)
		{
			if (_status["blue"] == false)
			{
				glm::vec3 color{ BLUE };

				_player->SetObjectColor(glm::vec4{ color + _player->GetColor(), 1.f });
				_status["blue"] = true;
			}
		}

		if (_map[i]->CheckBlockCollision(_player) == true)
			_player->Move(vec3::back(_game_speed));

		_map[i]->SetLightPos(_light_pos);
		_map[i]->Move(vec3::back(_game_speed), _light_pos);
	}
}

void GameScene::CheckWorldCollision(Map* map)
{
	int32_t collision_type{ map->CheckCollision(_player) };

	if (collision_type == 1)
	{
		for (auto& tube : _map)
		{
			tube->Rotate(-60.f, _light_pos);
		}

		_player->Move(vec3::left(12.f));
		_camera->Move(define::DIRECTION::LEFT, 12.f);
		_player->Transform(_shader);
	}
	else if (collision_type == 2)
	{
		for (auto& tube : _map)
		{
			tube->Rotate(60.f, _light_pos);
		}

		_player->Move(vec3::right(12.f));
		_camera->Move(define::DIRECTION::RIGHT, 12.f);
		_player->Transform(_shader);
	}
	else if (collision_type == 3)
	{
		if (_fever == false)
		{
			_player->Move(vec3::up(_replace));
			_camera->Move(define::DIRECTION::UP, _replace);
		}
	}
}

void GameScene::DetectGameOver()
{
	if (_player->GetPos().y < 0.f)
	{
		_game_over = true;
		_stop_animation = true;
	}
	else if (_player->GetPos().z > _camera->GetPos().z)
	{
		_game_over = true;
		_stop_animation = true;
	}
}

void GameScene::CheckPlayerStatus()
{
	if (_status["red"] == false
		or _status["green"] == false
		or _status["blue"] == false)
		return;

	if (_fever_time > 7.f)
	{
		for (auto& [key, value] : _status)
		{
			value = false;
		}

		_fever_time = 0.f;
		_fever = false;
		_player->SetObjectColor(BLACK, 1.f);
		return;
	}

	_fever_time += _delta_time;
	_fever = true;
	_player->SetObjectColor(RAND_COLOR, 1.f);
}
