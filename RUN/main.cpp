#include "pch.h"
#include "GameScene.h"
#include "SampleScene.h"
#include "SampleScene_1.h"
#include <Engine.h>

Engine engine;

int32_t main(int32_t argc, char** argv)
{
	// glut 초기화
	glutInit(&argc, argv);

	// 게임을 표시할 윈도우 크기 결정
	const Window window{ 800, 800, true };

	// 아아 테스트

	// 게임 엔진 초기화
	engine.Init(&window);

	// 새로운 게임 scene 추가 및 추가된 scene으로 변경
	//std::shared_ptr<GameScene> scene{ std::make_shared<GameScene>() };
	//std::shared_ptr<SampleScene> scene{ std::make_shared<SampleScene>() };
	std::shared_ptr<SampleScene_1> scene{ std::make_shared<SampleScene_1>() };
	engine.GetSceneMgr()->AddScene(define::SCENE_TYPE::GAME, &*scene);
	engine.GetSceneMgr()->ChangeScene(define::SCENE_TYPE::GAME);

	engine.Update();
}
