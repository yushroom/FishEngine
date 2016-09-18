#ifndef EditorGUI_hpp
#define EditorGUI_hpp

#include "FishEditor.hpp"
#include <functional>

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

class EditorGUI
{
public:
    EditorGUI() = delete;

    static void Init();

    static void Update();

    static void Clean();
    
    static void SelectMeshDialogBox(std::function<void(std::shared_ptr<FishEngine::Mesh>)> callback);
    
private:
    static int m_idCount;
    static void HierarchyItem(std::shared_ptr<FishEngine::GameObject> gameObject);
    static void DrawSceneGizmo();
    
    static bool m_showAssectSelectionDialogBox;
};

NAMESPACE_FISHEDITOR_END

#endif // EditorGUI_hpp
