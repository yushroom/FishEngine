//
//  GameLoop.cpp
//  FishEngine
//
//  Created by 俞云康 on 9/17/16.
//  Copyright © 2016 yushroom. All rights reserved.
//

#include "GameLoop.hpp"

#include <thread>

#include "Debug.hpp"
#include "RenderSystem.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Time.hpp"
#include "Scene.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "App.hpp"

const uint32_t WIDTH = 800, HEIGHT = 600;

NAMESPACE_FISHENGINE_BEGIN

GLFWwindow* GameLoop::m_window = nullptr;
std::vector<std::shared_ptr<App>> GameLoop::m_apps;
float GameLoop::m_fixedFrameRate = 30;

static void GlfwErrorCallback(int error, const char* description)
{
    Debug::LogError("Error %d: %s\n", error, description);
}

void GameLoop::Init()
{
    Debug::Init();
    Input::Init();
    
    Debug::Log("Starting GLFW context, OpenGL 4.1");
    glfwSetErrorCallback(GlfwErrorCallback);
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    m_window = glfwCreateWindow(WIDTH, HEIGHT, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    
    // Set the required callback functions
    glfwSetKeyCallback(m_window, GameLoop::KeyCallBack);
    glfwSetCursorPosCallback(m_window, GameLoop::MouseCallback);
    glfwSetScrollCallback(m_window, GameLoop::MouseScrollCallback);
    glfwSetCharCallback(m_window, GameLoop::CharacterCallback);
    glfwSetWindowSizeCallback(m_window, GameLoop::WindowSizeCallback);
    glfwSetMouseButtonCallback(m_window, GameLoop::MouseButtonCallback);
    
    RenderSystem::Init();
    
    Time::m_time = (float)glfwGetTime();
    
    for (auto& r : m_apps) {
        r->Init();
    }
}

void GameLoop::Run()
{
    Scene::Start();
    float fixed_delta_time = 1.0f / m_fixedFrameRate;
    
    // Game loop
    while (!glfwWindowShouldClose(m_window))
    {
        //auto start_time = std::chrono::high_resolution_clock::now();
        Input::Update();
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        int w = RenderSystem::width();
        int h = RenderSystem::height();
        Input::UpdateMousePosition(float(xpos)/w, float(ypos)/h);
        
        Scene::Update();
        
        RenderSystem::Render();
        
        double new_t = glfwGetTime();
        float interval = float(new_t) - Time::m_time;
        if (interval < fixed_delta_time) {
            std::chrono::milliseconds sleep_time((long long)((fixed_delta_time-interval)*1000));
            std::this_thread::sleep_for(sleep_time);
        }
        
        Time::m_deltaTime = float(glfwGetTime() - Time::m_time);
        Time::m_time = new_t;
    }
}

void GameLoop::Clean()
{
//    for (auto& r : m_apps) {
//        r->Clean();
//    }
    
    RenderSystem::Clean();
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}


void GameLoop::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if ((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)) {
        Input::UpdateKeyState((KeyCode)key, (Input::KeyState)action);
    }
    
    //TwEventKeyGLFW(key, action);
    //GUI::OnKey(key, action);
    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mode);
}

void GameLoop::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //    bool handled = GUI::OnMouseScroll(yoffset);
    //    if (handled)
    //        return;
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
    //auto& t = m_mainCamera->gameObject->transform;
    auto t = Scene::mainCamera()->transform();
    t->setLocalPosition(t->position() + 0.2f*float(yoffset)*t->forward());
    
}

//GLfloat lastX = 400, lastY = 300;
//bool firstMouse = true;

void GameLoop::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //    if (GUI::OnMouseButton(button, action))
    //        return;
    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
    Input::UpdateMouseButtonState(button, action == GLFW_PRESS ? Input::MouseButtonState_Down : Input::MouseButtonState_Up);
}

void GameLoop::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    //glViewport(0, 0, width, height);
    //GUI::OnWindowSizeChanged(width, height);
}

void GameLoop::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    //GUI::OnMouse(xpos, ypos);
}

void GameLoop::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
    ImGui_ImplGlfwGL3_CharCallback(window, codepoint);
}


NAMESPACE_FISHENGINE_END
