#include "Renderer.hpp"

NAMESPACE_FISHENGINE_BEGIN


void Renderer::OnInspectorGUI()
{
    for (auto& m : m_materials) {
        m->OnInspectorGUI();
    }
}

NAMESPACE_FISHENGINE_END
