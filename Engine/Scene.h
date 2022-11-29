#pragma once

// NOTE :
// 모든 scene들의 interface를 결정하는 scene class
// 

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void OnLoad() {}
	virtual void OnRelease() {}

	virtual void OnIdleMessage() {}
	virtual void OnKeyboardMessage(uchar key, int32_t x, int32_t y) {}
	virtual void OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y) {}
	virtual void OnKeyboardUpMessage(uchar key, int32_t x, int32_t y) {}
	virtual void OnSpecialKeyUpMessage(int32_t key, int32_t x, int32_t y) {}
	virtual void OnMouseMessage(int32_t button, int32_t x, int32_t y) {}
	virtual void OnMouseUpMessage(int32_t button, int32_t x, int32_t y) {}
	virtual void OnMouseMotionMessage(int32_t x, int32_t y) {}
	virtual void OnMousePassiveMotionMessage(int32_t x, int32_t y) {}
	virtual void OnAnimate(int32_t index) {}
	virtual void OnRender();
};
