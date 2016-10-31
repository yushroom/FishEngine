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
#include <PhysicsSystem.hpp>
#include "SceneView.hpp"

#include "EditorRenderSystem.hpp"
#include "EditorGUI.hpp"
#include "EditorTime.hpp"
#include "Selection.hpp"

const uint32_t WIDTH = 1280, HEIGHT = 960;

using namespace FishEngine;

namespace FishEditor
{
    int FishEditorWindow::m_windowWidth = WIDTH;
    int FishEditorWindow::m_windowHeight = HEIGHT;
    float FishEditorWindow::m_pixelsPerPoint = 1.0f;

    GLFWwindow* FishEditorWindow::m_window = nullptr;
    std::vector<std::shared_ptr<App>> FishEditorWindow::m_apps;
    float FishEditorWindow::m_fixedFrameRate = 30;
    bool FishEditorWindow::m_inPlayMode = false;

    static void GlfwErrorCallback(int error, const char* description)
    {
        Debug::LogError("Error %d: %s\n", error, description);
    }

    void FishEditorWindow::Init()
    {
        Debug::Init();
// #if FISHENGINE_PLATFORM_APPLE
//         Debug::setColorMode(false);
// #endif
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

//        int w, h;
//        glfwGetFramebufferSize(m_window, &w, &h);
//        Screen::m_width = w;
//        Screen::m_height = h;
        WindowSizeCallback(m_window, m_windowWidth, m_windowHeight);

        SceneView::Init();
        Camera::m_mainCamera = SceneView::m_camera;
        EditorRenderSystem::Init();

        //PhysicsSystem::Init();

        for (auto& r : m_apps) {
            r->Init();
        }
    }

    void FishEditorWindow::Run()
    {
        //Scene::Start();
        //PhysicsSystem::Start();

        const float fixed_delta_time = 1.0f / m_fixedFrameRate;
        //float old_time = 0;
        EditorTime::m_time = (float)glfwGetTime();

        // Game loop
        while (!glfwWindowShouldClose(m_window))
        {
            //auto start_time = std::chrono::high_resolution_clock::now();
            Input::Update();
            // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
            glfwPollEvents();
            double xpos, ypos;
            glfwGetCursorPos(m_window, &xpos, &ypos);
            Input::UpdateMousePosition(float(xpos) / m_windowWidth, 1.0f - float(ypos) / m_windowHeight);


            if (m_inPlayMode) {
                Scene::Update();
                PhysicsSystem::FixedUpdate();
            }
            else {
                SceneView::Update();
            }
            
            //Scene::UpdateBounds();
//            if (Input::GetMouseButtonDown(0))
//            {
//                Ray ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
//                auto go = Scene::IntersectRay(ray);
//                Selection::setSelectedGameObjectInHierarchy(go);
//            }

            EditorRenderSystem::Render();

            float new_t = (float)glfwGetTime();
            float interval = new_t - EditorTime::m_time;
            if (interval < fixed_delta_time) {
                std::chrono::milliseconds sleep_time((long long)((fixed_delta_time - interval) * 1000));
                std::this_thread::sleep_for(sleep_time);
            }

            const float old_time = (float)glfwGetTime();
            EditorTime::m_deltaTime = old_time - EditorTime::m_time;
            Time::m_deltaTime = EditorTime::m_deltaTime;
            if (m_inPlayMode) {
                Time::m_time = old_time;
            }
            EditorTime::m_time = old_time;
        }
    }

    void FishEditorWindow::Play()
    {
        PhysicsSystem::Init();
        m_inPlayMode = true;
        Camera::m_mainCamera = nullptr;
        Scene::Start();
        //Camera::m_mainCamera = Scene::mainCamera();
    }

    void FishEditorWindow::Stop()
    {
        m_inPlayMode = false;
        Camera::m_mainCamera = SceneView::m_camera;
        PhysicsSystem::Clean();
    }

     
    void FishEditorWindow::Clean()
    {
        //    for (auto& r : m_apps) {
        //        r->Clean();
        //    }

        EditorRenderSystem::Clean();

        //PhysicsSystem::Clean();

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

        //if ((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)) {
        //    Input::UpdateKeyState(key, (KeyState)action);
        //}
        Input::UpdateKeyState(key, (KeyState)action);
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
            //bool done = EditorGUI::OnMouseButton((MouseButtonCode)button, s);
            //if (done) return;
            Input::UpdateMouseButtonState(button, s);
        }
    }

    void FishEditorWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        //Debug::Log("window size changed");
        m_windowWidth = width;
        m_windowHeight = height;
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        Screen::m_width = w;
        Screen::m_height = h;
        Screen::m_pixelsPerPoint = static_cast<float>(w) / width;
        m_pixelsPerPoint = static_cast<float>(w) / width;
        if (w != 0 && h != 0)
        {
            EditorRenderSystem::OnWindowSizeChanged(w, h);
            EditorGUI::OnWindowSizeChanged(w, h);
            auto scene_view_pos_size = EditorGUI::sceneViewPositionAndSize();
            //Screen::m_width = scene_view_pos_size.z;
            //Screen::m_height = scene_view_pos_size.w;
        }
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
}
