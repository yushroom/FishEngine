#include "Selection.hpp"

namespace FishEditor
{
    std::weak_ptr<FishEngine::Object> Selection::m_objectInInspector;

    bool Selection::m_isActiveGameObjectLocked = false;
    std::weak_ptr<FishEngine::GameObject> Selection::m_selectedGameObjectInHierarchy;

    std::weak_ptr<FishEngine::Object> Selection::m_activeAsset;

    std::weak_ptr<FishEngine::GameObject> Selection::m_activeGameObject;
}
