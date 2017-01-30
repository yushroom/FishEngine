#ifndef Selection_hpp
#define Selection_hpp

#include "FishEditor.hpp"

using FishEngine::GameObject;
using FishEngine::GameObjectPtr;
using FishEngine::Object;
using FishEngine::ObjectPtr;
using FishEngine::Transform;
using FishEngine::TransformPtr;

namespace FishEditor
{
    // Access to the selection in the editor.
    class Selection
    {
    public:

        Selection() = delete;


        // Returns the guids of the selected assets.
        static std::vector<std::string> assetGUIDs();


        // Returns the actual object selection. Includes prefabs, non-modifyable objects.
        static ObjectPtr        activeObject();

        // Returns the instanceID of the actual object selection. Includes prefabs, non-modifyable objects.
        static int              activeInstanceID();

        // Returns the active game object. (The one shown in the inspector).
        static GameObjectPtr    activeGameObject();

        // Returns the active transform. (The one shown in the inspector).
        static TransformPtr     activeTransform()
        {
            return m_activeTransform.lock();
        }


        // The actual unfiltered selection from the Scene.
        static std::list<ObjectPtr>     objects();

        // The actual unfiltered selection from the Scene returned as instance ids instead of objects.
        static std::vector<int>         instanceIDs();

        // Returns the actual game object selection. Includes prefabs, non-modifyable objects.
        static std::list<GameObjectPtr> gameObjects();

        static std::list<std::weak_ptr<Transform>> const &
        transforms()
        {
            return m_transforms;
        }

        // todo, move
        static void setTransforms(std::list<std::weak_ptr<Transform>> const & transforms);


        static GameObjectPtr    selectedGameObjectInHierarchy();
        //static ObjectPtr        activeAsset();

        static void setActiveGameObject(GameObjectPtr gameObject);
        static void setActiveTransform(TransformPtr transform);
        //static void setSelectedGameObjectInHierarchy(GameObjectPtr gameObject);
        //static void setActiveAsset(std::weak_ptr<Object> assetObject);

        static bool Contains(int instanceID);
        static bool Contains(TransformPtr transform);

        static Action selectionChanged;
        static Action activeObjectChanged;

    private:

        friend class HierarchyView;

        static bool m_isActiveGameObjectLocked;

        //static std::weak_ptr<Transform>     m_activeTransform;

        static std::weak_ptr<Object>        m_objectInInspector;

        //friend class EditorGUI;

        // Returns the active transform. (The one shown in the inspector).
        static std::weak_ptr<Transform>     m_activeTransform;

        // Returns the active game object. (The one shown in the inspector).
        static std::weak_ptr<GameObject>    m_activeGameObject;

        //static std::weak_ptr<FishEngine::Object>        m_activeObject;

        static std::weak_ptr<GameObject>    m_selectedGameObjectInHierarchy;
        static std::weak_ptr<Object>        m_activeAsset;

        // This is the most common selection type when working with scene objects.
        static std::list<std::weak_ptr<Transform>> m_transforms;

    };
}

#endif // Selection_hpp
