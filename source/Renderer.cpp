#include "Renderer.hpp"

NAMESPACE_FISHENGINE_BEGIN


void Renderer::OnEditorGUI()
{
    for (auto& m : m_materials) {
        m->OnEditorGUI();
    }
}

NAMESPACE_FISHENGINE_END
