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
        
        activeGameObject()
        {
            return m_activeGameObject.lock();
        }

        static void
        setActiveGameObject(FishEngine::GameObjectPtr gameObject)
        {
            m_activeGameObject = gameObject;
            m_selectedGameObjectInHierarchy = gameObject;
        }

        static FishEngine::GameObjectPtr
        selectedGameObjectInHierarchy()
        {
            return m_selectedGameObjectInHierarchy.lock();
        }

        static void
        setSelectedGameObjectInHierarchy(FishEngine::GameObjectPtr gameObject)
        {
            m_selectedGameObjectInHierarchy = gameObject;
            if (!m_isActiveGameObjectLocked)
                m_activeGameObject = gameObject;
        }
        
        static void
        setActiveAsset(FishEngine::ObjectPtr assetObject)
        {
            m_activeAsset = assetObject;
            if (!m_isActiveGameObjectLocked)
                m_objectInInspector = assetObject;
        }

    private:
        //friend class EditorGUI;
        static bool m_isActiveGameObjectLocked;
        static std::weak_ptr<FishEngine::Object>        m_objectInInspector;
        
        static std::weak_ptr<FishEngine::GameObject>    m_activeGameObject;
        static std::weak_ptr<FishEngine::GameObject> m_selectedGameObjectInHierarchy;
        
        static std::weak_ptr<FishEngine::Object>        m_activeAsset;
    };
}

#endif // Selection_hpp
