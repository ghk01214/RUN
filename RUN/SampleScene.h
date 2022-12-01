#pragma once

#include <Scene.h>

class Shader;
class Camera;
class Object;

class SampleScene : public Scene
{
private:
	enum OBJECT
	{
		CUBE = 0,
		TETRA,
		MAX
	};
public:
	SampleScene();
	~SampleScene();

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
	// 구현하고자 하는 메소드들을 여기에 선언
#pragma region [USER-DEFINED METHOD]
private:
	void CreateObjects();
	void CreateGrid();
	void ChangeRenderObject(OBJECT obj_type);
	void ChangeCullFaceState();
	void Move(define::DIRECTION direction);
	void RotateX(define::ROTATE_DIRECTION direction);
	void RotateY(define::ROTATE_DIRECTION direction);
	void ChangeDrawType(uint32_t type);

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
	// 단일 객체를 선언할 때는 Object* 객체명
	// 복수 object를 사용하는 객체를 선언할 때는 std::vector<Object*> 객체명
	std::vector<Object*> _object;		// == Object* _object[2](이 변수는 단일 객체가 아닌 배열로서 사용)
	std::vector<Object*> _grid;
	Object* _render_object;

	bool _apply_cull_face;
#pragma endregion
};
