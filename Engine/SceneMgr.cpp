#include "pch.h"
#include "Scene.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr() :
	_current{ define::SCENE_TYPE::NONE }
{
}

SceneMgr::~SceneMgr()
{
	for (auto& [type, scene] : _scene)
	{
		delete scene;
	}
}

void SceneMgr::AddScene(define::SCENE_TYPE type, Scene* scene)
{
	inst->_scene.insert(std::make_pair(type, scene));
}

void SceneMgr::ChangeScene(define::SCENE_TYPE type)
{
	inst->_current = type;
	inst->LoadScene();
}

void SceneMgr::LoadScene()
{
	inst->_scene[inst->_current]->OnLoad();
}

void SceneMgr::OnIdleMessage()
{
	inst->_scene[inst->_current]->OnIdleMessage();
}

void SceneMgr::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnKeyboardMessage(key, x, y);
}

void SceneMgr::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnSpecialKeyMessage(key, x, y);
}

void SceneMgr::OnKeyboardUpMessage(uchar key, int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnKeyboardUpMessage(key, x, y);
}

void SceneMgr::OnSpecialKeyUpMessage(int32_t key, int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnSpecialKeyUpMessage(key, x, y);
}

void SceneMgr::OnMouseMessage(int32_t button, int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnMouseMessage(button, x, y);
}

void SceneMgr::OnMouseUpMessage(int32_t button, int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnMouseUpMessage(button, x, y);
}

void SceneMgr::OnMouseMotionMessage(int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnMouseMotionMessage(x, y);
}

void SceneMgr::OnMousePassiveMotionMessage(int32_t x, int32_t y)
{
	inst->_scene[inst->_current]->OnMousePassiveMotionMessage(x, y);
}

void SceneMgr::OnAnimate(int32_t index)
{
	inst->_scene[inst->_current]->OnAnimate(index);
}

void SceneMgr::OnRender()
{
	inst->_scene[inst->_current]->OnRender();
}
