#pragma once

#include "FishEditor.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace FishEditor
{
    enum class TransformToolType
    {
        None,
        Translate,
        Rotate,
        Scale,
    };

    enum class TransformSpace
    {
        Global,
        Local,
    };
    
    enum class ShadingMode
    {
        Shaded,
        Wireframe,
        ShadedWireframe,
    };

    class SceneViewEditor
    {
    public:
        FishEngine::Vector2             m_position;
        FishEngine::Int2                m_size{128, 128};

        FishEngine::RenderTargetPtr     m_sceneViewRenderTarget;
        FishEngine::ColorBufferPtr      m_colorBuffer;
        FishEngine::DepthBufferPtr      m_depthBuffer;
        //FishEngine::RenderTexturePtr    m_sceneViewRenderTexture;

        FishEngine::RenderTargetPtr     m_selectionOutlineRT;
        FishEngine::DepthBufferPtr      m_selectionOutlineDepthBuffer;
        FishEngine::RenderTargetPtr     m_selectionOutlineRT2;
        FishEngine::ColorBufferPtr      m_selectionOutlineColorBuffer2;

        void Init();

        void Update();

        void Render();

        void Clean();

        FishEngine::CameraPtr camera()
        {
            return m_camera;
        }

        // this scene view is focused, so it may receive keyboard event.
        bool focused() const
        {
            return m_focused;
        }

        // mouse position is inside of this scene view, so it may receive mouse event.
        bool isMouseHovered() const
        {
            return m_isMouseHovered;
        }
        
        bool highlightSelections() const
        {
            return m_highlightSelections;
        }
        
        void setHighlightSelections(bool value)
        {
            m_highlightSelections = value;
        }

    private:
        friend class EditorGUI;

        FishEngine::CameraPtr m_camera;
        FishEngine::GameObjectPtr m_cameraGameObject;

        bool        m_isWireFrameMode;
        bool        m_useGammaCorrection;
        bool        m_showShadowMap;
        bool        m_highlightSelections   = false;

        bool        m_mouseEventHandled;
        
        ShadingMode m_shadingMode           = ShadingMode::Shaded;
        bool        m_showGizmos            = true;
        

        // this scene view is focused, so it may receive keyboard event.
        bool        m_focused               = false;

        // mouse position is inside of this scene view, so it may receive mouse event.
        bool        m_isMouseHovered        = false;

        TransformToolType m_transformToolType = TransformToolType::Translate;
        TransformSpace m_transformSpace     = TransformSpace::Global;

        int         m_selectedAxis          = -1; // temp
        std::weak_ptr<FishEngine::GameObject> m_lastSelectedGameObject; // temp
        void DrawTranslateGizmo();
        void DrawRotateGizmo();
        void DrawScaleGizmo();
        void DrawSceneGizmo();
    };

    typedef std::shared_ptr<SceneViewEditor> SceneViewEditorPtr;
}
