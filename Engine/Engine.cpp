#include "pch.h"
#include "SceneMgr.h"
#include "Engine.h"

Engine::Engine() :
	_window{ nullptr },
	_scene_mgr{ nullptr }
{
}

Engine::~Engine()
{
}

void Engine::Init(const Window* window)
{
	inst->_window.reset(const_cast<Window*>(window));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - window->width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - window->height) / 2);
	glutInitWindowSize(window->width, window->height);
	glutCreateWindow("GL");

	if (glewInit() != GLEW_OK)
	{
		std::cout << std::format("Unable to initialize GLEW") << std::endl;
		exit(EXIT_FAILURE);
	}

	if (glewIsSupported("GL_VERSION_4_5"))
		std::cout << std::format("GL Version is 4.5") << std::endl;
	else
		std::cout << std::format("GLEW 4.5 not supported") << std::endl;

	glClearDepth(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(OnIdleMessage);
	glutKeyboardFunc(OnKeyboardMessage);
	glutSpecialFunc(OnSpecialKeyMessage);
	glutKeyboardUpFunc(OnKeyboardUpMessage);
	glutSpecialUpFunc(OnSpecialKeyUpMessage);
	glutMouseFunc(OnMouseMessage);
	glutMotionFunc(OnMouseMotionMessage);
	glutPassiveMotionFunc(OnMousePassiveMotionMessage);
}

void Engine::Init(const Window* window, const std::string& name)
{
	inst->_window.reset(const_cast<Window*>(window));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - window->width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - window->height) / 2);
	glutInitWindowSize(window->width, window->height);
	glutCreateWindow(name.c_str());

	if (glewInit() != GLEW_OK)
	{
		std::cout << std::format("Unable to initialize GLEW") << std::endl;
		exit(EXIT_FAILURE);
	}

	if (glewIsSupported("GL_VERSION_4_6"))
		std::cout << std::format("GL Version is 4.6") << std::endl;
	else
		std::cout << std::format("GLEW 4.6 not supported") << std::endl;

	glClearDepth(1.f);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(OnIdleMessage);
	glutKeyboardFunc(OnKeyboardMessage);
	glutSpecialFunc(OnSpecialKeyMessage);
	glutKeyboardUpFunc(OnKeyboardUpMessage);
	glutSpecialUpFunc(OnSpecialKeyUpMessage);
	glutMouseFunc(OnMouseMessage);
	glutMotionFunc(OnMouseMotionMessage);
	glutPassiveMotionFunc(OnMousePassiveMotionMessage);
}

void Engine::OnIdleMessage()
{
	inst->_scene_mgr->OnIdleMessage();
	glutPostRedisplay();
}

void Engine::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	inst->_scene_mgr->OnKeyboardMessage(key, x, y);
	glutPostRedisplay();
}

void Engine::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	inst->_scene_mgr->OnSpecialKeyMessage(key, x, y);
	glutPostRedisplay();
}

void Engine::OnKeyboardUpMessage(uchar key, int32_t x, int32_t y)
{
	switch (key)
	{
		case 'Q': FALLTHROUGH
		case 'q':
		{
			std::cout << std::format("Terminate Program\n");

			glutLeaveMainLoop();
		}
		return;
	}

	inst->_scene_mgr->OnKeyboardUpMessage(key, x, y);
	glutPostRedisplay();
}

void Engine::OnSpecialKeyUpMessage(int32_t key, int32_t x, int32_t y)
{
	inst->_scene_mgr->OnSpecialKeyUpMessage(key, x, y);
	glutPostRedisplay();
}

void Engine::OnMouseMessage(int32_t button, int32_t state, int32_t x, int32_t y)
{
	if (state == GLUT_DOWN)
		inst->_scene_mgr->OnMouseMessage(button, x, y);
	else if (state == GLUT_UP)
		inst->_scene_mgr->OnMouseUpMessage(button, x, y);

	glutPostRedisplay();
}

void Engine::OnMouseMotionMessage(int32_t x, int32_t y)
{
	inst->_scene_mgr->OnMouseMotionMessage(x, y);
	glutPostRedisplay();
}

void Engine::OnMousePassiveMotionMessage(int32_t x, int32_t y)
{
	inst->_scene_mgr->OnMousePassiveMotionMessage(x, y);
	glutPostRedisplay();
}

void Engine::OnAnimate(int32_t index)
{
	inst->_scene_mgr->OnAnimate(index);
}

void Engine::Update()
{
	glutMainLoop();
}

void Engine::Render()
{
	// TODO : 그리기
	inst->_scene_mgr->OnRender();

	glutSwapBuffers();
}

void Engine::Reshape(int32_t width, int32_t height)
{
	inst->_window->width = Convert::ToUint32(width);
	inst->_window->height = Convert::ToUint32(height);

	glViewport(0, 0, width, height);
}

void Engine::Resize(const Window* window)
{
	inst->_window.reset(const_cast<Window*>(window));

	glutReshapeWindow(window->width, window->height);
}

const std::unique_ptr<SceneMgr>& Engine::GetSceneMgr() const
{
	return inst->_scene_mgr;
}

const int32_t Engine::GetWindowWidth() const
{
	return inst->_window->width;
}

const int32_t Engine::GetWindowHeight() const
{
	return inst->_window->height;
}
