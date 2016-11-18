#ifndef Selection_hpp
#define Selection_hpp

#include "FishEditor.hpp"

namespace FishEditor
{
    // Access to the selection in the editor.
    class Selection
    {
    public:
        // Returns the active game object. (The one shown in the inspector).
        static FishEngine::GameObjectPtr
        activeGameObject() {
            return m_activeGameObject.lock();
        }

        static void
        setActiveGameObject(FishEngine::GameObjectPtr gameObject) {
            m_activeGameObject = gameObject;
        }

        static FishEngine::GameObjectPtr
        selectedGameObjectInHierarchy() {
            return m_selectedGameObjectInHierarchy.lock();
        }

        static void
        setSelectedGameObjectInHierarchy(FishEngine::GameObjectPtr gameObject) {
            m_selectedGameObjectInHierarchy = gameObject;
        }

    private:
        //friend class EditorGUI;
        static std::weak_ptr<FishEngine::GameObject> m_activeGameObject;
        static std::weak_ptr<FishEngine::GameObject> m_selectedGameObjectInHierarchy;
    };
}

#endif // Selection_hpp
