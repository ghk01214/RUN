#include "pch.h"
#include "Shader.h"
#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up, float pitch, float yaw) :
	_pos{ pos },
	_front{ vec3::front() },
	_up{ vec3::zero() },
	_right{ vec3::zero() },
	_look{ _pos + _front },
	_pitch{ pitch },
	_yaw{ yaw },
	_sensitivity{ 0.1f },
	_world_up{ up },
	_view{ mat4::unit() },
	_projection{ mat4::unit() },
	_fov{ 45.f },
	_near{ 1.f },
	_far{ 1000.f },
	_aspect{ 1.f / 1.f },
	_perspective{ true }
{
	Update();
}

Camera::~Camera()
{
}

void Camera::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
}

void Camera::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y, float delta)
{
	float velocity{ 2.5f * delta };

	switch (key)
	{
		case GLUT_KEY_LEFT:
		{
			Move(define::DIRECTION::LEFT, velocity);
		}
		break;
		case GLUT_KEY_RIGHT:
		{
			Move(define::DIRECTION::RIGHT, velocity);
		}
		break;
		case GLUT_KEY_UP:
		{
			Move(define::DIRECTION::FRONT, velocity);
		}
		break;
		case GLUT_KEY_DOWN:
		{
			Move(define::DIRECTION::BACK, velocity);
		}
		break;
		case GLUT_KEY_PAGE_UP:
		{
			Move(define::DIRECTION::UP, velocity);
		}
		break;
		case GLUT_KEY_PAGE_DOWN:
		{
			Move(define::DIRECTION::DOWN, velocity);
		}
		break;
		case GLUT_KEY_HOME:
		{
			Zoom(1.f);
		}
		break;
		case GLUT_KEY_END:
		{
			Zoom(-1.f);
		}
		break;
		default:
		break;
	}
}

void Camera::OnMouseMessage(int32_t button, int32_t x, int32_t y)
{
}

void Camera::OnMouseUpMessage(int32_t button, int32_t x, int32_t y)
{
}

void Camera::OnMouseMotionMessage(float delta_x, float delta_y)
{
	// 카메라 민감도에 따른 마우스 이동량 계산
	delta_x *= _sensitivity;
	delta_y *= _sensitivity;

	// 계산된 이동량을 각 pitch, yaw에 더한다
	_yaw += delta_x;
	_pitch += delta_y;

	if (_pitch > 89.f)
		_pitch = 89.f;
	if (_pitch < -89.f)
		_pitch = -89.f;

	Update();
}

void Camera::Move(define::DIRECTION direction, float velocity)
{
	// 카메라의 위치를 right, up, front vector를 변경해서
	// 카메라가 어느 방향을 바라보고 있어도 키 입력 대로 움직이도록 함
	switch (direction)
	{
		case define::DIRECTION::LEFT:
		{
			_pos -= _right * velocity;
		}
		break;
		case define::DIRECTION::RIGHT:
		{
			_pos += _right * velocity;
		}
		break;
		case define::DIRECTION::FRONT:
		{
			_pos += _front * velocity;
		}
		break;
		case define::DIRECTION::BACK:
		{
			_pos -= _front * velocity;
		}
		break;
		case define::DIRECTION::UP:
		{
			_pos += _up * velocity;
		}
		break;
		case define::DIRECTION::DOWN:
		{
			_pos -= _up * velocity;
		}
		break;
	}

	_look = _pos + _front;
}

void Camera::RotateX(define::ROTATE_DIRECTION direction)
{
	// direction으로 1, -1 중 한 가지 값을 받아서
	// 카메라의 회전 방향 결정
	// 1이면 반시계 방향
	// -1이면 시계 방향
	_pitch += _sensitivity * 10 * direction;

	Update();
}

void Camera::RotateY(define::ROTATE_DIRECTION direction)
{
	// direction으로 1, -1 중 한 가지 값을 받아서
	// 카메라의 회전 방향 결정
	// 1이면 반시계 방향
	// -1이면 시계 방향
	_yaw += _sensitivity * 10 * direction;

	Update();
}

void Camera::Zoom(float delta)
{
	_fov -= delta;

	if (_fov < 1.f)
		_fov = 1.f;
	if (_fov > 45.f)
		_fov = 45.f;
}

void Camera::Update()
{
	glm::vec3 look{ vec3::zero() };
	look.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
	look.y = std::sin(glm::radians(_pitch));
	look.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

	_front = glm::normalize(look);
	_right = glm::normalize(glm::cross(_front, _world_up));
	_up = glm::normalize(glm::cross(_right, _front));
	_look = _pos + _front;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(_pos, _look, _up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if (_perspective == false)
		return glm::ortho(-10.f, 10.f, -10.f, 10.f, _near, _far);

	return glm::perspective(glm::radians(_fov), _aspect, _near, _far);
}
