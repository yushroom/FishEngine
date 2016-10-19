#include "Light.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Gizmos.hpp"
#include "Transform.hpp"

using namespace FishEngine;

void Light::OnInspectorGUI()
{
    ImGui::ColorEdit4("Color", m_color.m);
    ImGui::DragFloat("Range", &m_range);
}

void FishEngine::Light::OnDrawGizmos()
{
    Gizmos::DrawIcon(transform()->position(), "Light");
}

void FishEngine::Light::OnDrawGizmosSelected()
{
    Gizmos::setColor(Color::yellow);
    Vector3 center = transform()->position();
    Vector3 dir = transform()->forward();
    //Gizmos::DrawLine(center, center + dir);
    //Gizmos::DrawCircle(center, 1.0f, dir);
    Gizmos::DrawLight(center, dir);
}

std::list<std::shared_ptr<Light>> FishEngine::Light::m_lights;

PLight Light::Create()
{
    auto l = std::make_shared<Light>();
    m_lights.push_back(l);
    return l;
}

