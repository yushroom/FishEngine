#include "Selection.hpp"

namespace FishEditor
{
    bool Selection::m_isActiveGameObjectLocked = false;
    std::weak_ptr<FishEngine::GameObject> Selection::m_selectedGameObjectInHierarchy;
    std::weak_ptr<FishEngine::GameObject> Selection::m_activeGameObject;
}
