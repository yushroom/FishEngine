#include "GameApp.hpp"
#include "GLEnvironment.hpp"
#include <glfw/glfw3.h>

#include "Debug.hpp"
#include "Resources.hpp"
#include "Input.hpp"
#include "Screen.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "PhysicsSystem.hpp"
#include "RenderTarget.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"
#include "ModelImporter.hpp"
#include "Graphics.hpp"

namespace FishEngine
{

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
    auto m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glCheckError();

    glfwSetKeyCallback(m_window, GameApp::KeyCallBack);
    //glfwSetCursorPosCallback(m_window, GameApp::MouseCallback);
    glfwSetScrollCallback(m_window, GameApp::MouseScrollCallback);
    //glfwSetCharCallback(m_window, GameApp::CharacterCallback);
    glfwSetWindowSizeCallback(m_window, GameApp::WindowSizeCallback);
    glfwSetMouseButtonCallback(m_window, GameApp::MouseButtonCallback);

    glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

#if FISHENGINE_PLATFORM_WINDOWS
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    auto err = glewInit();
    if (err != GLEW_OK)
    {
        Debug::LogError("%s", glewGetErrorString(err));
    }
    else
    {
        Debug::Log("GlEW initialized");
    }
#endif

    glFrontFace(GL_CW);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    Screen::m_width = w;
    Screen::m_height = h;
    Screen::m_pixelsPerPoint = static_cast<float>(w) / m_windowWidth;

    Resources::Init();
    Input::Init();
    RenderSystem::Init();
    //WindowSizeCallback(m_window, m_windowWidth, m_windowHeight);

    Init();
    Scene::Init();
    PhysicsSystem::Init();

    Scene::Start();
    //PhysicsSystem::Start();
    
    
    auto m_colorBuffer = ColorBuffer::Create(Screen::width(), Screen::height());
    m_colorBuffer->setName("SceneViewColor");
    //m_depthBuffer = DepthBuffer::Create(m_size.x, m_size.y);
    //m_depthBuffer->setName("SceneViewDepth");
    auto m_sceneViewRenderTarget = std::make_shared<RenderTarget>();
    // TODO
    m_sceneViewRenderTarget->Set(m_colorBuffer, RenderSystem::m_mainDepthBuffer);
    
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

//        Pipeline::PushRenderTarget(m_sceneViewRenderTarget);
        glViewport(0, 0, Screen::width(), Screen::height());
        RenderSystem::Render();
//        Pipeline::PopRenderTarget();
//        
//        auto mtl = Material::builtinMaterial("ScreenTexture");
//        auto quad = Model::builtinMesh(PrimitiveType::Quad);
//        mtl->setMainTexture(m_colorBuffer);
//        Graphics::DrawMesh(quad, mtl);
        
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        
//        auto mtl = Material::defaultMaterial();
//        auto sphere = Model::builtinMesh(PrimitiveType::Sphere);
//        Graphics::DrawMesh(sphere, mtl);
//        
        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }

    glfwTerminate();
    return 0;
}

void GameApp::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    //ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);

    Input::UpdateKeyState(key, (KeyState)action);
    if (mods & GLFW_MOD_ALT)
    {
        Input::UpdateKeyState(GLFW_KEY_LEFT_ALT, (KeyState)action);
        Input::UpdateKeyState(GLFW_KEY_RIGHT_ALT, (KeyState)action);
    }
    if (mods & GLFW_MOD_CONTROL)
    {
        Input::UpdateKeyState(GLFW_KEY_LEFT_CONTROL, (KeyState)action);
        Input::UpdateKeyState(GLFW_KEY_RIGHT_CONTROL, (KeyState)action);
    }
    if (mods & GLFW_MOD_SUPER)
    {
        Input::UpdateKeyState(GLFW_KEY_LEFT_SUPER, (KeyState)action);
        Input::UpdateKeyState(GLFW_KEY_RIGHT_SUPER, (KeyState)action);
    }
    if (mods & GLFW_MOD_SHIFT)
    {
        Input::UpdateKeyState(GLFW_KEY_LEFT_SHIFT, (KeyState)action);
        Input::UpdateKeyState(GLFW_KEY_RIGHT_SHIFT, (KeyState)action);
    }

}

void FishEngine::GameApp::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
    Input::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
    //if (Input::m_mainSceneViewEditor->isMouseHovered())
    //{
    Input::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
    //}
}

void GameApp::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
    MouseButtonState s = action == GLFW_PRESS ? MouseButtonState::Down : MouseButtonState::Up;
    Input::UpdateMouseButtonState(button, s);
    //if (Input::m_mainSceneViewEditor->isMouseHovered())
    //{
    Input::UpdateMouseButtonState(button, s);
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

}
