#pragma once

extern std::uniform_real_distribution<float> uid_color;

// 각 색상의 값을 매크로로 저장
#define RED 1.f, 0.f, 0.f
#define GREEN 0.f, 1.f, 0.f
#define BLUE 0.f, 0.f, 1.f
#define BLACK 0.f, 0.f, 0.f
#define WHITE 1.f, 1.f, 1.f
#define GRAY 0.5f, 0.5f, 0.5f
#define YELLOW 1.f, 1.f, 0.f
#define RAND_COLOR uid_color(dre), uid_color(dre), uid_color(dre)

namespace define
{
	// scene의 종류
	enum class SCENE_TYPE
	{
		NONE = 0,
		START,
		GAME,
		END,
		MAX
	};

	// 카메라 종류
	enum class CAMERA_TYPE
	{
		NONE = 0,
		ORTHOGRAPHIC,
		PERSPECTIVE,
		FIRST,
		THIRD,
		TOP,
		MAX
	};

	// 이동 방향
	enum class DIRECTION
	{
		NONE = 0,
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		UP,
		DOWN,
		MAX
	};

	// 회전 방향
	enum ROTATE_DIRECTION
	{
		COUNTER_CLOCK = -1,
		CLOCK = 1
	};
}

namespace glm
{
	template<typename T>
	inline glm::vec2 ToVec2(T value) { return glm::vec2{ value.x, value.y }; }

	template<typename T>
	inline glm::vec3 ToVec3(T value) { return glm::vec3{ value.x, value.y, value.z }; }
}

namespace vec3
{
	inline const glm::vec3 zero() { return glm::vec3{ 0.f }; }					// 영벡터
	inline const glm::vec3 unit() { return glm::vec3{ 1.f }; }					// 단위 벡터
	inline const glm::vec3 front() { return glm::vec3{ 0.f, 0.f, -1.f }; }		// 정면 방향 벡터
	inline const glm::vec3 front(float delta) { return front() * delta; }		// 정면 방향 벡터를 인자값의 크기 만큼 늘린 벡터
	inline const glm::vec3 back() { return glm::vec3{ 0.f, 0.f, 1.f }; }		// 후면 방향 벡터
	inline const glm::vec3 back(float delta) { return back() * delta; }			// 후면 방향 벡터를 인자값의 크기 만큼 늘린 벡터
	inline const glm::vec3 left() { return glm::vec3{ -1.f, 0.f, 0.f }; }		// 좌측 방향 벡터
	inline const glm::vec3 left(float delta) { return left() * delta; }			// 좌측 방향 벡터를 인자값의 크기 만큼 늘린 벡터
	inline const glm::vec3 right() { return glm::vec3{ 1.f, 0.f, 0.f }; }		// 우측 방향 벡터
	inline const glm::vec3 right(float delta) { return right() * delta; }		// 우측 방향 벡터를 인자값의 크기 만큼 늘린 벡터
	inline const glm::vec3 up() { return glm::vec3{ 0.f, 1.f, 0.f }; }			// 위 방향 벡터
	inline const glm::vec3 up(float delta) { return up() * delta; }				// 위 방향 벡터를 인자값의 크기 만큼 늘린 벡터
	inline const glm::vec3 down() { return glm::vec3{ 0.f, -1.f, 0.f }; }		// 아래 방향 벡터
	inline const glm::vec3 down(float delta) { return down() * delta; }			// 아래 방향 벡터를 인자값의 크기 만큼 늘린 벡터
	inline const glm::vec3 x() { return right(); }								// X축 방향 벡터(사실상 이름 재정의)
	inline const glm::vec3 y() { return up(); }									// Y축 방향 벡터(사실상 이름 재정의)
	inline const glm::vec3 z() { return back(); }								// Z축 방향 벡터(사실상 이름 재정의)
}

namespace mat4
{
	inline const glm::mat4 zero() { return glm::mat4{ 0.f }; }					// 초기화된 행렬
	inline const glm::mat4 unit() { return glm::mat4{ 1.f }; }					// 단위 행렬
}