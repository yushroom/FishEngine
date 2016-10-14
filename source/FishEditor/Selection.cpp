#include "Selection.hpp"

namespace FishEditor
{
    std::weak_ptr<FishEngine::GameObject> Selection::m_selectedGameObjectInHierarchy;
    std::weak_ptr<FishEngine::GameObject> Selection::m_activeGameObject;
}
