#pragma once

#include <Scene.h>

class Shader;
class Camera;
class Object;

class SampleScene_1 : public Scene
{
	enum OBJECT
	{
		CUBE_0 = 0,
		CONE_0,
		CUBE_1,
		SPHERE_1,
		MAX
	};
public:
	SampleScene_1();
	~SampleScene_1();

	// 기본 정의된 클래스 메소드
#pragma region [PRE-DEFINED METHOD]
	void OnLoad() override;
	void OnRelease() override;

	void OnIdleMessage() override;
	void OnKeyboardMessage(uchar key, int32_t x, int32_t y) override;
	void OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y) override;
	void OnMouseMessage(int32_t button, int32_t x, int32_t y) override;
	void OnMouseMotionMessage(int32_t x, int32_t y) override;
	void OnMouseUpMessage(int32_t button, int32_t x, int32_t y) override;
	void OnAnimate(int32_t index) override;
	void OnRender() override;
private:
	void CalculateDeltaTime();
	void LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader);
	void ReleaseSingleObject(Object* object, std::shared_ptr<Shader>& shader);
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
	void RotateX(int32_t index, define::ROTATE_DIRECTION direction);
	void OrbitY(define::ROTATE_DIRECTION direction);
	void RotateY(int32_t index, define::ROTATE_DIRECTION direction);
	void ChangeRenderObject(int index, OBJECT obj_type);
	void Move(define::DIRECTION direction);
#pragma endregion

	// 기본 정의된 클래스 멤버 변수(변경 X)
#pragma region [PRE-DEFINED VARIABLE]
private:
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Shader> _color_shader;

	bool _stop_animation;
	int32_t _animation_speed;
	bool _click;

	int32_t _old_x;
	int32_t _old_y;

	int32_t _time;
	int32_t _old_time;
	float _delta_time;
#pragma endregion
	// 유저가 정의한 클래스 멤버 변수
#pragma region [USER-DEFINED VARIABLE]
private:
	std::vector<Object*> _object;
	std::vector<Object*> _grid;
	std::vector<Object*> _render_object;
#pragma endregion
};
