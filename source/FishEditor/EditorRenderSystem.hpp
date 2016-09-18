#ifndef EditorRenderSystem_hpp
#define EditorRenderSystem_hpp

#include "FishEditor.hpp"

NAMESPACE_FISHEDITOR_BEGIN

class EditorRenderSystem
{
public:
    EditorRenderSystem() = delete;

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
    static int m_width;
    static int m_height;

    static bool m_isWireFrameMode;
    static bool m_useGammaCorrection;
};

NAMESPACE_FISHEDITOR_END

#endif // EditorRenderSystem_hpp