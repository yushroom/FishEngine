#ifndef FishEditorWindow_hpp
#define FishEditorWindow_hpp

#include "FishEditor.hpp"
#include "FishEngine.hpp"
#include "GLEnvironment.hpp"

NAMESPACE_FISHEDITOR_BEGIN

class FishEditorWindow
{
public:
    FishEditorWindow() = delete;
    
    static void Init();
    
    static void Run();
    
    static void Clean();
    
    static void AddApp(std::shared_ptr<FishEngine::App> app) {
        m_apps.push_back(app);
    }
    
    static GLFWwindow* window() {
        return m_window;
    }
    
    static void Play();
    static void Stop();
    static bool InPlayMode() {
        return m_inPlayMode;
    }
  
private:
    
    static int m_windowWidth;
    static int m_windowHeight;
//    static float m_pixelScaleX;
//    static float m_pixelScaleY;
    
    static GLFWwindow* m_window;
    static std::vector<std::shared_ptr<FishEngine::App>> m_apps;
    
    static float m_fixedFrameRate;
    static bool m_inPlayMode;
    
    // glfw callback
    static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void WindowSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);
};

NAMESPACE_FISHEDITOR_END

#endif /* FishEditorWindow_hpp */
