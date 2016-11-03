#ifndef EditorGUI_hpp
#define EditorGUI_hpp

#include "FishEditor.hpp"
#include <functional>

#include <Vector3.hpp>
#include <Command.hpp>

#include "SceneViewEditor.hpp"

namespace FishEditor
{
    enum class AssetType
    {
        Model,
        Material,
        Texture,
        AudioClip,
        Font,
        Script,
        Shader,
    };

    class EditorGUI
    {
    public:
        EditorGUI() = delete;

        static void Init();

        static void Update();

        static void Clean();

        static bool Button(const char* text);

        static void Matrix4x4(const std::string& label, FishEngine::Matrix4x4& mat);

        static void SelectMeshDialogBox(std::function<void(std::shared_ptr<FishEngine::Mesh>)> callback);

        static void OnWindowSizeChanged(const int width, const int height);
        
        // return Vector4(x, y, w, h), in pixels
        // the origin is at the left-bottom corner of the window(OpenGL)
        static FishEngine::Int2 sceneViewSize();

    private:
        friend class FishEditorWindow;
        static int m_idCount;   // temp

        static PSceneViewEditor m_mainSceneViewEditor;

        static void HierarchyItem(std::shared_ptr<FishEngine::GameObject> gameObject);

        // https://docs.unity3d.com/Manual/LearningtheInterface.html
        static void DrawInspectorWindow();
        static void DrawHierarchyWindow();
        static void DrawProjectWindow();
        static void DrawSceneView();
        static void DrawMainToolbar();
        static void DrawMainMenu();

        static bool s_locked;       // temp

        // hierarchy
        static bool s_isAnyItemClicked;
        static bool s_openMenuPopup;

        static bool m_showAssectSelectionDialogBox;

        template<class T>
        static void OnInspectorGUI(const std::shared_ptr<T>& component);

        static bool s_mouseEventHandled;

        static FishEngine::Int2 m_sceneSize;
    };

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Component>& component);

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Transform>& transform);
}

#endif // EditorGUI_hpp
