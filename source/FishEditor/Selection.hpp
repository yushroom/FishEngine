#ifndef Selection_hpp
#define Selection_hpp

#include "FishEditor.hpp"

NAMESPACE_FISHEDITOR_BEGIN

// Access to the selection in the editor.
class Selection
{
public:
    // Returns the active game object. (The one shown in the inspector).
    static std::shared_ptr<FishEngine::GameObject> activeGameObject() {
        return m_activeGameObject.lock();
    }

    static void setActiveGameObject(std::shared_ptr<FishEngine::GameObject> gameObject) {
        m_activeGameObject = gameObject;
    }

private:
    //friend class EditorGUI;
    static std::weak_ptr<FishEngine::GameObject> m_activeGameObject;
};


NAMESPACE_FISHEDITOR_END

#endif // Selection_hpp