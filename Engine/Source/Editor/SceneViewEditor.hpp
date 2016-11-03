#pragma once

#include "FishEditor.hpp"
#include "Vector3.hpp"

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

    class SceneViewEditor
    {
    public:
        FishEngine::Int2 m_size{128, 128};
        FishEngine::PRenderTexture m_sceneViewRenderTexture;

        void Init();

        void Update();

        void Render();

        void Clean();

        FishEngine::PCamera camera()
        {
            return m_camera;
        }

    private:
        friend class EditorGUI;

        FishEngine::PCamera m_camera;
        FishEngine::PGameObject m_cameraGameObject;

        bool m_isWireFrameMode;
        bool m_useGammaCorrection;
        bool m_showShadowMap;
        bool m_highlightSelections;

        bool s_mouseEventHandled;

        TransformToolType m_transformToolType = TransformToolType::Translate;
        TransformSpace m_transformSpace = TransformSpace::Global;

        int m_selectedAxis = -1; // temp
        std::weak_ptr<FishEngine::GameObject> m_lastSelectedGameObject; // temp
        void DrawTranslateGizmo();
        void DrawRotateGizmo();
        void DrawScaleGizmo();
        void DrawSceneGizmo();
    };

    typedef std::shared_ptr<SceneViewEditor> PSceneViewEditor;
}