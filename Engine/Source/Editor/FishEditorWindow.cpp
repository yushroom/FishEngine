#include "FishEditorWindow.hpp"

#include <thread>
#include <fstream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui/imgui_dock.h>

#include "Debug.hpp"
#include "EditorRenderSystem.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Time.hpp"
#include "Scene.hpp"
#include "App.hpp"
#include <PhysicsSystem.hpp>
#include <Camera.hpp>

#include "EditorRenderSystem.hpp"
#include "EditorGUI.hpp"
#include "EditorTime.hpp"
#include "Selection.hpp"
#include "EditorInput.hpp"

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

    class WindowManager
    {
    public:
        WindowManager() = delete;
        
        static void NewWindow(const char* title, const int width = 512, const int height = 256)
        {
            auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);
            s_windows.push_back(window);
            //return window;
        }

        static void Update()
        {
            s_windows.remove_if([](GLFWwindow* window) { return window == nullptr; });
            for (auto& window : s_windows)
            {
                if (glfwWindowShouldClose(window))
                {
                    glfwDestroyWindow(window);
                    window = nullptr;
                }
                else
                {
                    glClear(GL_COLOR_BUFFER_BIT);
                }
            }
        }

        static GLFWwindow* firstWindow()
        {
            return s_windows.front();
        }

        static GLFWwindow* secondWindow()
        {
            return *(++s_windows.begin());
        }

    private:
        static std::list<GLFWwindow*> s_windows;
    };

    std::list<GLFWwindow*> WindowManager::s_windows;

    void FishEditorWindow::Init()
    {
        Debug::Init();
// #if FISHENGINE_PLATFORM_APPLE
//         Debug::setColorMode(false);
// #endif
        EditorInput::Init();

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
        //WindowManager::NewWindow("FishEngine", WIDTH, HEIGHT);
        //WindowManager::NewWindow("Another window", WIDTH/2, HEIGHT/2);
        //m_window = WindowManager::firstWindow();
        glfwMakeContextCurrent(m_window);

        //WindowManager::NewWindow("Material Editor", 1024, 768);

        // Set the required callback functions
        glfwSetKeyCallback(m_window, FishEditorWindow::KeyCallBack);
        glfwSetCursorPosCallback(m_window, FishEditorWindow::MouseCallback);
        glfwSetScrollCallback(m_window, FishEditorWindow::MouseScrollCallback);
        glfwSetCharCallback(m_window, FishEditorWindow::CharacterCallback);
        glfwSetWindowSizeCallback(m_window, FishEditorWindow::WindowSizeCallback);
        glfwSetMouseButtonCallback(m_window, FishEditorWindow::MouseButtonCallback);

        glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

        std::ifstream fin("./FishEditorLayout.xml");
        if (fin.is_open())
            ImGui::LoadDock(fin);
        else
            Debug::LogWarning("Layout file not found.");

        WindowSizeCallback(m_window, m_windowWidth, m_windowHeight);

        //Camera::m_mainCamera = SceneView::m_camera;
        EditorRenderSystem::Init();
        Camera::m_mainCamera = EditorGUI::m_mainSceneViewEditor->camera();

        //PhysicsSystem::Init();

        for (auto& r : m_apps) {
            r->Init();
        }

        CommandManager::Init();
    }

    void FishEditorWindow::Run()
    {
        const float fixed_delta_time = 1.0f / m_fixedFrameRate;
        EditorTime::m_time = (float)glfwGetTime();

        // Game loop
        while (!glfwWindowShouldClose(m_window))
        {
            WindowManager::Update();

            //auto start_time = std::chrono::high_resolution_clock::now();
            EditorInput::Update();
            Input::Update();
            // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
            glfwPollEvents();
            double xpos, ypos;
            glfwGetCursorPos(m_window, &xpos, &ypos);
            float x = static_cast<float>(xpos);
            float y = static_cast<float>(ypos);
            EditorInput::UpdateMousePosition(x / m_windowWidth, 1.0f - y / m_windowHeight);
            if (EditorGUI::m_mainSceneViewEditor->isMouseHovered())
            {
                auto& p = EditorGUI::m_mainSceneViewEditor->m_position;
                auto& s = EditorGUI::m_mainSceneViewEditor->m_size;
                float px = (x-p.x)/(float)s.x;
                float py = 1.0f - (y-p.y)/(float)s.y;
                //Debug::Log("%lf %lf", px, py);
                Input::UpdateMousePosition(px, py);
            }


            if (m_inPlayMode)
            {
                Scene::Update();
                PhysicsSystem::FixedUpdate();
            }
            else {
                //SceneView::Update();
                EditorGUI::m_mainSceneViewEditor->Update();
            }

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

            //auto window = WindowManager::secondWindow();
            //glfwMakeContextCurrent(window);
            //glClearColor(1, 0, 0, 1);
            //glClear(GL_COLOR_BUFFER_BIT);
            //glfwSwapBuffers(window);
            //
            //glfwMakeContextCurrent(m_window);
        }
    }

    void FishEditorWindow::Play()
    {
        PhysicsSystem::Init();
        m_inPlayMode = true;
        Camera::m_mainCamera = nullptr;
        Scene::Start();
    }

    void FishEditorWindow::Stop()
    {
        m_inPlayMode = false;
        Camera::m_mainCamera = EditorGUI::m_mainSceneViewEditor->camera();
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


    void FishEditorWindow::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        //TwEventKeyGLFW(key, action);
        //GUI::OnKey(key, action);
        ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
        //if (ImGui::GetIO().WantCaptureKeyboard)
        //    return;

        EditorInput::UpdateKeyState(key, (KeyState)action);
        if (mods & GLFW_MOD_ALT)
        {
            EditorInput::UpdateKeyState(GLFW_KEY_LEFT_ALT, (KeyState)action);
            EditorInput::UpdateKeyState(GLFW_KEY_RIGHT_ALT, (KeyState)action);
        }
        if (mods & GLFW_MOD_CONTROL)
        {
            EditorInput::UpdateKeyState(GLFW_KEY_LEFT_CONTROL, (KeyState)action);
            EditorInput::UpdateKeyState(GLFW_KEY_RIGHT_CONTROL, (KeyState)action);
        }
        if (mods & GLFW_MOD_SUPER)
        {
            EditorInput::UpdateKeyState(GLFW_KEY_LEFT_SUPER, (KeyState)action);
            EditorInput::UpdateKeyState(GLFW_KEY_RIGHT_SUPER, (KeyState)action);
        }
        if (mods & GLFW_MOD_SHIFT)
        {
            EditorInput::UpdateKeyState(GLFW_KEY_LEFT_SHIFT, (KeyState)action);
            EditorInput::UpdateKeyState(GLFW_KEY_RIGHT_SHIFT, (KeyState)action);
        }

    }

    void FishEditorWindow::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
        EditorInput::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
        if (EditorGUI::m_mainSceneViewEditor->isMouseHovered())
        {
            Input::UpdateAxis(Axis::MouseScrollWheel, (float)yoffset);
        }
    }

    void FishEditorWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
        MouseButtonState s = action == GLFW_PRESS ? MouseButtonState::Down : MouseButtonState::Up;
        EditorInput::UpdateMouseButtonState(button, s);
        if (EditorGUI::m_mainSceneViewEditor->isMouseHovered())
        {
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
        //Screen::m_width = w;
        //Screen::m_height = h;
        Screen::m_pixelsPerPoint = static_cast<float>(w) / width;
        m_pixelsPerPoint = static_cast<float>(w) / width;
        if (w != 0 && h != 0)
        {
            auto size = EditorGUI::sceneViewSize();
            Screen::m_width = size.x;
            Screen::m_height = size.y;
            EditorRenderSystem::OnWindowSizeChanged(w, h);
            EditorGUI::OnWindowSizeChanged(w, h);
        }
    }

    void FishEditorWindow::MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
    }

    void FishEditorWindow::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
    {
        ImGui_ImplGlfwGL3_CharCallback(window, codepoint);
    }
}
