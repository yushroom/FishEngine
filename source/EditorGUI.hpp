#ifndef EditorGUI_hpp
#define EditorGUI_hpp

#include "FishEngine.hpp"
#include <string>
#include <functional>

NAMESPACE_FISHENGINE_BEGIN

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
    
    static void SelectMeshDialogBox(std::function<void(std::shared_ptr<Mesh>)> callback);
    
private:
    static int m_idCount;
    static void HierarchyItem(GameObject* gameObject);
    static void DrawSceneGizmo();
    
    static bool m_showAssectSelectionDialogBox;
};

NAMESPACE_FISHENGINE_END

#endif // EditorGUI_hpp
