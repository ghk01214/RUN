#pragma once

#include <Scene.h>

class Shader;
class Camera;
class Object;
class Map;

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	// 기본 정의된 클래스 메소드
#pragma region [PRE-DEFINED METHOD]
	void OnLoad() override;
	void OnRelease() override;

	void OnIdleMessage() override;
	void OnKeyboardMessage(uchar key, int32_t x, int32_t y) override;
	void OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y) override;
	void OnKeyboardPressedMessage();
	void OnSpecialKeyPressedMessage();
	void OnKeyboardUpMessage(uchar key, int32_t x, int32_t y) override;
	void OnSpecialKeyUpMessage(int32_t key, int32_t x, int32_t y) override;
	void OnMouseMessage(int32_t button, int32_t x, int32_t y) override;
	void OnMouseMotionMessage(int32_t x, int32_t y) override;
	void OnMouseUpMessage(int32_t button, int32_t x, int32_t y) override;
	void OnAnimate(int32_t index) override;
	void OnRender() override;
private:
	void CalculateDeltaTimeNFPS();
	void LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader);
	void ReleaseSingleObject(Object* object);
	void LoadMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader);
	void ReleaseMultipleObject(std::vector<Object*>* object);
	void ViewProjection(std::shared_ptr<Shader>& shader);
	void RenderSingleObject(Object* object, std::shared_ptr<Shader>& shader);
	void RenderMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader);

#pragma endregion
	// 유저가 정의한 클래스 메소드
#pragma region [USER-DEFINED METHOD]
private:
	void CreateMap();
	void CreatePlayer();

	void LoadMap(std::vector<Map*>* map, std::shared_ptr<Shader>& shader);
	void ReleaseMap(std::vector<Map*>* map);
	void RenderMap(std::vector<Map*>* map, std::shared_ptr<Shader>& shader);

	void Move(define::DIRECTION direction);
	void Jump();

	void MoveWorld();
	void CheckWorldCollision(Map* map);

	void DetectGameOver();
	void CheckPlayerStatus();
#pragma endregion

	// 기본 정의된 클래스 멤버 변수(변경 X)
#pragma region [PRE-DEFINED VARIABLE]
private:
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Shader> _shader;

	bool _stop_animation;
	int32_t _animation_speed;
	bool _click;

	int32_t _old_x;
	int32_t _old_y;

	int32_t _time;
	int32_t _old_time;
	float _delta_time;
	int32_t _frame;
	int32_t _frame_time;
	float _fps;

	std::unordered_set<uchar> _key;
	std::unordered_set<int32_t> _special;
#pragma endregion
	// 유저가 정의한 클래스 멤버 변수
#pragma region [USER-DEFINED VARIABLE]
private:
	std::vector<Map*> _map;
	std::vector<Object*> _map_line;
	Object* _player;		// == Object* _object[2](이 변수는 단일 객체가 아닌 배열로서 사용)

	bool _jumping;
	float _jump_speed;
	float _jump_pos;
	float _gravity;

	bool _game_over;
	float _end_time;

	std::unordered_map<std::string, bool> _status;
	float _fever_time;
	bool _fever;
	float _game_speed;

	float _replace;
	glm::vec3 _light_pos;
#pragma endregion
};