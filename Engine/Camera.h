#pragma once

// NOTE :
// 화면에 그려진 월드를 비추는 카메라 class

class Shader;

class Camera
{
public:
	Camera() = default;
	Camera(glm::vec3 pos, glm::vec3 up = vec3::up(), float pitch = 0.f, float yaw = -90.f);
	~Camera();

	void OnKeyboardMessage(uchar key, int32_t x, int32_t y);
	void OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y, float delta);
	void OnMouseMessage(int32_t button, int32_t x, int32_t y);
	void OnMouseUpMessage(int32_t button, int32_t x, int32_t y);
	void OnMouseMotionMessage(float delta_x, float delta_y);

	void Move(define::DIRECTION direction, float velocity);
	void RotateX(define::ROTATE_DIRECTION direction);
	void RotateY(define::ROTATE_DIRECTION direction);
	void Zoom(float delta);
	void Update();

	glm::vec3 GetPos() { return _pos; }
	glm::vec3 GetRight() { return _right; }
	glm::vec3 GetUp() { return _up; }
	glm::vec3 GetFront() { return _front; }
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void SetPos(glm::vec3 pos) { _pos = pos; }
	void SetPos(float x, float y, float z) { SetPos(glm::vec3{ x, y, z }); }
	void SetLook(glm::vec3 look) { _look = look; }
	void SetLook(float x, float y, float z) { SetLook(glm::vec3{ x, y, z }); }
	void SetRight(glm::vec3 right) { _right = right; }
	void SetUp(glm::vec3 up) { _up = up; }
	void SetFront(glm::vec3 front) { _front = front; }

	void ChangeCameraMode(bool perspective) { _perspective = perspective; }

private:
	glm::vec3 _pos;				// 카메라 위치
	glm::vec3 _front;			// 카메라 front vector
	glm::vec3 _up;				// 카메라 up vector
	glm::vec3 _right;			// 카메라 right vector
	glm::vec3 _look;			// 카메라 look vector

	float _pitch;				// 카메라 pitch(x축 회전)
	float _yaw;					// 카메라 yaw(y축 회전)
	float _sensitivity;			// 카메라 이동, 회전 시 민감도

	glm::vec3 _world_up;		// 월드 좌표계 up vector

	glm::mat4 _view;			// 뷰 변환 행렬
	glm::mat4 _projection;		// 투영 변환 행렬

	float _fov;					// 카메라 fov
	float _near;				// view frustum near
	float _far;					// view frustum far
	float _aspect;				// 카메라 종횡비

	bool _perspective;			// 원근 투영 여부
};
