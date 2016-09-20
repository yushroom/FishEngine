#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include "GLEnvironment.hpp"

NAMESPACE_FISHENGINE_BEGIN

class RenderSystem
{
public:
    RenderSystem() = delete;
    //static RenderSystem& GetInstance();

    static void Init();

    static void Render();

    static void Clean();

    
    static int width() {
        return m_width;
    }

    static int height() {
        return m_height;
    }
    
    static void setWireFrameMode(bool value) {
        m_isWireFrameMode = value;
    }
    
    static void setGammaCorrection(bool value) {
        m_useGammaCorrection = value;
    }

    static void SaveScreenShot(const std::string& path);

private:
    friend class FishEditor::EditorRenderSystem;
    static int m_width;
    static int m_height;
    
    static bool m_isWireFrameMode;
    static bool m_useGammaCorrection;

};

NAMESPACE_FISHENGINE_END

#endif // RenderSystem_hpp
