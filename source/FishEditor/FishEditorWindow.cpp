#include "FishEditorWindow.hpp"

#include <thread>

#include "Debug.hpp"
#include "EditorRenderSystem.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Time.hpp"
#include "Scene.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "App.hpp"

#include "EditorRenderSystem.hpp"
#include "EditorGUI.hpp"

const uint32_t WIDTH = 1280, HEIGHT = 960;

using namespace FishEngine;

NAMESPACE_FISHEDITOR_BEGIN

int FishEditorWindow::m_windowWidth = WIDTH;
int FishEditorWindow::m_windowHeight = HEIGHT;

GLFWwindow* FishEditorWindow::m_window = nullptr;
std::vector<std::shared_ptr<App>> FishEditorWindow::m_apps;
float FishEditorWindow::m_fixedFrameRate = 30;

static void GlfwErrorCallback(int error, const char* description)
{
    Debug::LogError("Error %d: %s\n", error, description);
}

void FishEditorWindow::Init()
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
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    m_window = glfwCreateWindow(WIDTH, HEIGHT, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    
    // Set the required callback functions
    glfwSetKeyCallback(m_window, FishEditorWindow::KeyCallBack);
    glfwSetCursorPosCallback(m_window, FishEditorWindow::MouseCallback);
    glfwSetScrollCallback(m_window, FishEditorWindow::MouseScrollCallback);
    glfwSetCharCallback(m_window, FishEditorWindow::CharacterCallback);
    glfwSetWindowSizeCallback(m_window, FishEditorWindow::WindowSizeCallback);
    glfwSetMouseButtonCallback(m_window, FishEditorWindow::MouseButtonCallback);
    
    glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
    
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    //m_pixelScaleX = float(w) / m_windowWidth;
    //m_pixelScaleY = float(h) / m_windowHeight;
    //glViewport(0, 0, w, h);
    Screen::m_width = w;
    Screen::m_height = h;
    
    EditorRenderSystem::Init();
    
    for (auto& r : m_apps) {
        r->Init();
    }
}

void FishEditorWindow::Run()
{
    Scene::Start();
    float fixed_delta_time = 1.0f / m_fixedFrameRate;
    float old_time = 0;
    Time::m_time = (float)glfwGetTime();
    
    // Game loop
    while (!glfwWindowShouldClose(m_window))
    {
        //auto start_time = std::chrono::high_resolution_clock::now();
        Input::Update();
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        Input::UpdateMousePosition(float(xpos)/m_windowWidth, 1.0f-float(ypos)/m_windowHeight);
        
        Scene::Update();
        
        EditorRenderSystem::Render();
        
        float new_t = (float)glfwGetTime();
        float interval = new_t - old_time;
        if (interval < fixed_delta_time) {
            std::chrono::milliseconds sleep_time((long long)((fixed_delta_time - interval) * 1000));
            std::this_thread::sleep_for(sleep_time);
        }

        old_time = (float)glfwGetTime();
        Time::m_deltaTime = old_time - Time::m_time;
        Time::m_time = old_time;
    }
}

void FishEditorWindow::Clean()
{
//    for (auto& r : m_apps) {
//        r->Clean();
//    }
    
    EditorRenderSystem::Clean();
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}


void FishEditorWindow::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    //TwEventKeyGLFW(key, action);
    //GUI::OnKey(key, action);
    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mode);
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if ((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)) {
        Input::UpdateKeyState((KeyCode)key, (KeyState)action);
    }
}

void FishEditorWindow::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //    bool handled = GUI::OnMouseScroll(yoffset);
    //    if (handled)
    //        return;
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
    if (!ImGui::GetIO().WantCaptureMouse)
        Input::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
}

//GLfloat lastX = 400, lastY = 300;
//bool firstMouse = true;

void FishEditorWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //    if (GUI::OnMouseButton(button, action))
    //        return;
    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
    if (!ImGui::GetIO().WantCaptureMouse) {
        MouseButtonState s = action == GLFW_PRESS ? MouseButtonState::Down : MouseButtonState::Up;
        bool done = EditorGUI::OnMouseButton((MouseButtonCode)button, s);
        //if (done) return;
        Input::UpdateMouseButtonState(button, s);
    }
}

void FishEditorWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    Screen::m_width = w;
    Screen::m_height = h;
    if (w != 0 && h != 0)
        EditorRenderSystem::OnWindowSizeChanged(w, h);
    //glViewport(0, 0, width, height);
    //GUI::OnWindowSizeChanged(width, height);
}

void FishEditorWindow::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    //GUI::OnMouse(xpos, ypos);
}

void FishEditorWindow::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
    ImGui_ImplGlfwGL3_CharCallback(window, codepoint);
}


NAMESPACE_FISHEDITOR_END
