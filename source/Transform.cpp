#include "Transform.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"

NAMESPACE_FISHENGINE_BEGIN

Transform* Transform::GetChild(const int index) {
    if (index < -0 || index >= m_children.size()) {
        Debug::LogWarning("%s %d %s index out of range", __FILE__, __LINE__, __FUNCTION__);
        return nullptr;
    }
    
    auto p = m_children.begin();
    for (int i = 0; i < index; ++i) {
        p++;
    }
    return *p;
}

NAMESPACE_FISHENGINE_END
