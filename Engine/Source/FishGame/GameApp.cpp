#include <FishGame/GameApp.hpp>

#include <string>
#include <chrono>
#include <FishEngine/GLEnvironment.hpp>
#include <GLFW/glfw3.h>

#include <FishEngine/Debug.hpp>
#include <FishEngine/Resources.hpp>
#include <FishEngine/Input.hpp>
#include <FishEngine/Screen.hpp>
#include <FishEngine/RenderSystem.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Camera.hpp>
#include <FishEngine/PhysicsSystem.hpp>
#include <FishEngine/RenderTarget.hpp>
#include <FishEngine/Pipeline.hpp>
#include <FishEngine/Material.hpp>
#include <FishEngine/Graphics.hpp>
#include <FishEngine/Shader.hpp>
#include <FishEngine/ShaderCompiler.hpp>
#include <FishEngine/Mesh.hpp>

using namespace std;
using namespace FishEngine;
//using namespace FishGame;

GLFWwindow* GameApp::m_window = nullptr;
int GameApp::m_windowWidth = 640;
int GameApp::m_windowHeight = 480;

int GameApp::Run()
{
	Debug::Init();
	Debug::setColorMode(true);

	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "FishEngine", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	glCheckError();

	glfwSetKeyCallback(m_window, GameApp::KeyCallBack);
	//glfwSetCursorPosCallback(m_window, GameApp::MouseCallback);
	glfwSetScrollCallback(m_window, GameApp::MouseScrollCallback);
	//glfwSetCharCallback(m_window, GameApp::CharacterCallback);
	glfwSetWindowSizeCallback(m_window, GameApp::WindowSizeCallback);
	glfwSetMouseButtonCallback(m_window, GameApp::MouseButtonCallback);

	glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

	glfwSwapInterval(0);

#if FISHENGINE_PLATFORM_WINDOWS
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	auto err = glewInit();
	if (err != GLEW_OK)
	{
		//LogError(glewGetErrorString(err));
		LogError("GLEW not initialized");
	}
	else
	{
		LogInfo("GlEW initialized");
	}
#endif
	
	int w, h;
	glfwGetFramebufferSize(m_window, &w, &h);
	Screen::m_width = w;
	Screen::m_height = h;
	Screen::m_pixelsPerPoint = static_cast<float>(w) / m_windowWidth;

#if FISHENGINE_PLATFORM_WINDOWS
	auto shaderRoot = FishEngine::Path(R"(D:\program\github\FishEngine\Engine\Shaders)");
#else
	auto shaderRoot = FishEngine::Path("/Users/yushroom/program/FishEngine/Engine/Shaders");
#endif
	auto shaderIncludeDir = shaderRoot / "include";
	ShaderCompiler::setShaderIncludeDir(shaderIncludeDir.string());
	Shader::Init(shaderRoot.string());
	
#if FISHENGINE_PLATFORM_WINDOWS
	Mesh::Init(R"(D:\program\github\FishEngine\assets\Models)");
#else
	Mesh::Init("/Users/yushroom/program/FishEngine/assets/Models");
#endif
	
	//Resources::Init();
	Input::Init();
	RenderSystem::Init();
	//WindowSizeCallback(m_window, m_windowWidth, m_windowHeight);

	Init();
	Scene::Init();
	PhysicsSystem::Init();

	Scene::Start();
	//PhysicsSystem::Start();
	
	Init();

	constexpr int report_frames = 1000;
	int frames = 0;
	int fps = 30;
	//float time_stamp = static_cast<float>(glfwGetTime());
	auto time_stamp = std::chrono::high_resolution_clock::now();
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Poll for and process events */
		Input::Update();
		glfwPollEvents();
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);
		float x = static_cast<float>(xpos);
		float y = static_cast<float>(ypos);
		Input::UpdateMousePosition(x / m_windowWidth, 1.0f - y / m_windowHeight);

		Scene::Update();
		PhysicsSystem::FixedUpdate();

		glViewport(0, 0, Screen::width(), Screen::height());
		RenderSystem::Render();

		frames++;
		if (frames >= report_frames)
		{
			auto new_time_stamp = std::chrono::high_resolution_clock::now();
			//float new_time_stamp = static_cast<float>(glfwGetTime());
			auto elapse = new_time_stamp - time_stamp;
			auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapse).count() / 1000.0f;
			fps = static_cast<int>(report_frames / seconds);
			string title = "FishEngine FPS: " + to_string(fps);
			glfwSetWindowTitle(m_window, title.c_str());
			time_stamp = new_time_stamp;
			frames = 0;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);
	}

	glfwTerminate();
	return 0;
}

int KeyCodeFromGLFWKey(int key)
{
	if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
	{
		constexpr int offset = static_cast<int>(KeyCode::A) - GLFW_KEY_A;
		return key + offset;
	}
	else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
	{
		constexpr int offset = static_cast<int>(KeyCode::Alpha0) - GLFW_KEY_0;
		return key + offset;
	}
	else if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F15)
	{
		constexpr int offset = static_cast<int>(KeyCode::F1) - GLFW_KEY_F1;
		return key + offset;
	}
}

void GameApp::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
	KeyState state = action == GLFW_RELEASE ? KeyState::Up : KeyState::Down;

	Input::UpdateKeyState(KeyCodeFromGLFWKey(key), state);

	if (mods & GLFW_MOD_ALT)
	{
		Input::UpdateKeyState(KeyCode::LeftAlt, state);
		Input::UpdateKeyState(KeyCode::RightAlt, state);
	}
	if (mods & GLFW_MOD_CONTROL)
	{
		Input::UpdateKeyState(KeyCode::LeftControl, state);
		Input::UpdateKeyState(KeyCode::RightControl, state);
	}
	if (mods & GLFW_MOD_SUPER)
	{
		Input::UpdateKeyState(KeyCode::LeftCommand, state);
		Input::UpdateKeyState(KeyCode::RightCommand, state);
	}
	if (mods & GLFW_MOD_SHIFT)
	{
		Input::UpdateKeyState(KeyCode::LeftShift, state);
		Input::UpdateKeyState(KeyCode::RightShift, state);
	}

}

void FishEngine::GameApp::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
	Input::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
	//if (Input::m_mainSceneViewEditor->isMouseHovered())
	//{
	//Input::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
	//}
}

void GameApp::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
	MouseButtonState s = action == GLFW_PRESS ? MouseButtonState::Down : MouseButtonState::Up;
	Input::UpdateMouseButtonState(button, s);
	//if (Input::m_mainSceneViewEditor->isMouseHovered())
	//{
	//Input::UpdateMouseButtonState(button, s);
	//}
}

void GameApp::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	//Debug::Log("window size changed");
	m_windowWidth = width;
	m_windowHeight = height;
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	//Screen::m_width = w;
	//Screen::m_height = h;
	Screen::m_pixelsPerPoint = static_cast<float>(w) / width;
	//m_pixelsPerPoint = static_cast<float>(w) / width;
	if (w != 0 && h != 0)
	{
		//auto size = EditorGUI::sceneViewSize();
		Screen::m_width = w;
		Screen::m_height = h;
		RenderSystem::ResizeBufferSize(w, h);
		Camera::OnWindowSizeChanged(w, h);
		//EditorRenderSystem::OnWindowSizeChanged(w, h);
		//EditorGUI::OnWindowSizeChanged(w, h);
	}
}

//void GameApp::MouseCallback(GLFWwindow* window, double xpos, double ypos)
//{
//}

//void GameApp::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
//{
//    ImGui_ImplGlfwGL3_CharCallback(window, codepoint);
//}
