#pragma once

struct GLFWwindow;

namespace FishEngine
{
    class GameApp
    {

    public:
        int Run();
        virtual void Init() = 0;
        virtual void Update() = 0;
        //virtual void Render() = 0;

    protected:
        // GLFW callback
        static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        //static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
        //static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);

    private:
        GLFWwindow*     m_window = nullptr;
        static int      m_windowWidth;
        static int      m_windowHeight;
    };

}