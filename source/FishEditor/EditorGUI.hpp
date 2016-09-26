#ifndef EditorGUI_hpp
#define EditorGUI_hpp

#include "FishEditor.hpp"
#include <functional>

#include <Input.hpp>

NAMESPACE_FISHEDITOR_BEGIN

enum class AssetType {
    Model,
    Material,
    Texture,
    AudioClip,
    Font,
    Script,
    Shader,
};

enum class TransformToolType {
    None,
    Translate,
    Rotate,
    Scale,
};

class EditorGUI
{
public:
    EditorGUI() = delete;

    static void Init();

    static void Update();

    static void Clean();
    
    static bool OnMouseButton(FishEngine::MouseButtonCode button, FishEngine::MouseButtonState action);

    static bool Button(const char* text);
    
    static void Matrix4x4(const std::string& label, FishEngine::Matrix4x4& mat);
    
    static void SelectMeshDialogBox(std::function<void(std::shared_ptr<FishEngine::Mesh>)> callback);
    
private:
    static TransformToolType m_transformToolType;
    static int m_idCount;   // temp
    static void HierarchyItem(std::shared_ptr<FishEngine::GameObject> gameObject);

    static int m_selectedAxis; // temp
    static void DrawTranslateGizmo();
    static void DrawRotateGizmo();
    static void DrawScaleGizmo();
    static void DrawSceneGizmo();
    
    static bool m_showAssectSelectionDialogBox;
};

NAMESPACE_FISHEDITOR_END

#endif // EditorGUI_hpp
