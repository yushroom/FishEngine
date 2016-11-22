#ifndef GameLoop_hpp
#define GameLoop_hpp

#include "FishEngine.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    class FE_EXPORT GameLoop
    {
    public:
        GameLoop() = delete;

        static void Init();

        static void Run();

        static void Clean();

        static void AddApp(AppPtr app) {
            m_apps.push_back(app);
        }

        static GLFWwindow* window() {
            return m_window;
        }

    private:
        static GLFWwindow* m_window;
        static std::vector<AppPtr> m_apps;

        static float m_fixedFrameRate;

        // glfw callback
        static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);
    };
}

#endif /* GameLoop_hpp */
