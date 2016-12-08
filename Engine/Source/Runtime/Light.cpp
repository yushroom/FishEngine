#include "Light.hpp"

// #include <imgui/imgui.h>
// #include <imgui/imgui_internal.h>

#include "Gizmos.hpp"
#include "Transform.hpp"
#include "RenderTarget.hpp"
#include "QualitySettings.hpp"

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
        //auto shadow_map_size = QualitySettings::CalculateShadowMapSize();
        constexpr uint32_t shadow_map_size = 2048;
        
        m_renderTarget = std::make_shared<RenderTarget>();
        //m_renderTarget->Set(m_shadowMap, m_depthBuffer);

        m_shadowMap = LayeredDepthBuffer::Create(shadow_map_size, shadow_map_size, 4, false);
        //m_shadowMap = LayeredColorBuffer::Create(shadow_map_size, shadow_map_size, 4, TextureFormat::R32);
        m_shadowMap->setFilterMode(FilterMode::Bilinear);
        m_shadowMap->setWrapMode(TextureWrapMode::Clamp);
        m_renderTarget->SetDepthBufferOnly(m_shadowMap);

        //m_depthBuffer = LayeredDepthBuffer::Create(shadow_map_size, shadow_map_size, 4);
        //m_shadowMap = LayeredColorBuffer::Create(shadow_map_size, shadow_map_size, 4, TextureFormat::R32);
        //m_renderTarget->Set(m_shadowMap, m_depthBuffer);
    }

    LightPtr Light::Create()
    {
        auto l = std::make_shared<Light>();
        m_lights.push_back(l);
        return l;
    }
}
