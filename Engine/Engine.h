#pragma once

// NOTE:
// OpenGl 4.5를 이용한 렌더링 엔진
// Singleton model로 만들어서 메모리 상엔 1개 밖에 존재하지 않는다
//

#include "SceneMgr.h"

class Engine : public Singleton<Engine>
{
public:
	Engine();
	~Engine();

	void Init(const Window* window);
	void Init(const Window* window, const std::string& name);
	void Update();
	static void Render();

	static void Reshape(int32_t width, int32_t height);
	void Resize(const Window* window);

	static void OnIdleMessage();
	static void OnKeyboardMessage(uchar key, int32_t x, int32_t y);
	static void OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y);
	static void OnKeyboardUpMessage(uchar key, int32_t x, int32_t y);
	static void OnSpecialKeyUpMessage(int32_t key, int32_t x, int32_t y);
	static void OnMouseMessage(int32_t button, int32_t state, int32_t x, int32_t y);
	static void OnMouseMotionMessage(int32_t x, int32_t y);
	static void OnMousePassiveMotionMessage(int32_t x, int32_t y);
	static void OnAnimate(int32_t index);

	const std::unique_ptr<SceneMgr>& GetSceneMgr() const;
	const int32_t GetWindowWidth() const;
	const int32_t GetWindowHeight() const;

private:
	std::shared_ptr<Window> _window;
	std::unique_ptr<SceneMgr> _scene_mgr;
};