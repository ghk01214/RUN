#pragma once

#include <Scene.h>

class Shader;
class Camera;
class Object;

class GameScene : public Scene
{
private:
	enum OBJECT
	{
		SPHERE = 0,
		MAX
	};
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
	void CreateObjects();
	void CreateGrid();
	void ChangeRenderObject(OBJECT obj_type);
	void Move(define::DIRECTION direction);
	void Jump();
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
	float _wait_time;
	int32_t _frame;
	int32_t _frame_time;
	float _fps;

	bool _jumping = false;
	float _jump_speed;
	float _jump_pos;
	float _gravity = 9.8;

	std::unordered_set<uchar> _key;
	std::unordered_set<int32_t> _special;
#pragma endregion
	// 유저가 정의한 클래스 멤버 변수
#pragma region [USER-DEFINED VARIABLE]
private:
	Object* _sphere;		// == Object* _object[2](이 변수는 단일 객체가 아닌 배열로서 사용)
	std::vector<Object*> _grid;
#pragma endregion
};