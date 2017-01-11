#ifndef FishEditorWindow_hpp
#define FishEditorWindow_hpp

#include "FishEditor.hpp"
#include "FishEngine.hpp"
#include "GLEnvironment.hpp"
#include <glfw/glfw3.h>
#include "App.hpp"

namespace FishEditor
{

	typedef std::shared_ptr<App> AppPtr;

    class FishEditorWindow
    {
    public:
        FishEditorWindow() = delete;

        static void Init();

        static void Run();

        static void Clean();

        static void AddApp(AppPtr app)
        {
            m_apps.push_back(app);
        }

        static GLFWwindow* window()
        {
            return m_window;
        }
        
        static int windowWidth()
        {
            return m_windowWidth;
        }
        
        static int windowHeight()
        {
            return m_windowHeight;
        }
        
        static int windowWidthPixels()
        {
            return static_cast<int>(m_windowWidth * m_pixelsPerPoint);
        }
        
        static int windowHeightPixels()
        {
            return static_cast<int>(m_windowHeight * m_pixelsPerPoint);
        }
        
        static float pixelsPerPoint()
        {
            return m_pixelsPerPoint;
        }

        static void Play();
        static void Stop();
        static bool InPlayMode()
        {
            return m_inPlayMode;
        }

    private:

        static int m_windowWidth;
        static int m_windowHeight;
        
        // This value is the number of screen pixels per point of interface space. For instance, 2.0 on retina displays.
        static float m_pixelsPerPoint;

        static GLFWwindow* m_window;
        static std::vector<AppPtr> m_apps;

        static float m_fixedFrameRate;
        static bool m_inPlayMode;

        // glfw callback
        static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);
    };
}

#endif /* FishEditorWindow_hpp */
