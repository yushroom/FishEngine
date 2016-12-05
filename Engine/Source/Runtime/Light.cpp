#include "Light.hpp"

// #include <imgui/imgui.h>
// #include <imgui/imgui_internal.h>

#include "Gizmos.hpp"
#include "Transform.hpp"
#include "RenderTarget.hpp"

namespace FishEngine
{
    std::list<LightPtr> Light::m_lights;

    void Light::OnDrawGizmos()
    {
        Gizmos::DrawIcon(transform()->position(), "Light");
    }

    void Light::OnDrawGizmosSelected()
    {
        Gizmos::setColor(Color::yellow);
        Vector3 center = transform()->position();
        Vector3 dir = transform()->forward();
        Gizmos::DrawLight(center, dir);
    }

    Light::Light()
    {
        m_shadowMap = LayeredColorBuffer::Create(1024, 1024, 4, TextureFormat::R32);
        m_depthBuffer = LayeredDepthBuffer::Create(1024, 1024, 4);
        m_renderTarget = std::make_shared<RenderTarget>();
        m_renderTarget->Set(m_shadowMap, m_depthBuffer);
    }

    LightPtr Light::Create()
    {
        auto l = std::make_shared<Light>();
        m_lights.push_back(l);
        return l;
    }
}
