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

    static void setWireFrameMode(bool value) {
        m_isWireFrameMode = value;
    }

    static void setGammaCorrection(bool value) {
        m_useGammaCorrection = value;
    }
    
    static void setShowShadowMap(bool value) {
        m_showShadowMap = value;
    }
    
    static void setHightlightSelections(bool value) {
        m_highlightSelections = value;
    }

    static void SaveScreenShot(const std::string& path);

private:
    friend class FishEditorWindow;
    
    static bool m_isWireFrameMode;
    static bool m_useGammaCorrection;
    static bool m_showShadowMap;
    static bool m_highlightSelections;
    
    static void OnWindowSizeChanged(const int width, const int height);
};

NAMESPACE_FISHEDITOR_END

#endif // EditorRenderSystem_hpp
