#include "Light.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace FishEngine;

void Light::OnInspectorGUI()
{
    ImGui::ColorEdit4("Color", m_color.m);
    ImGui::DragFloat("Range", &m_range);
}

std::list<std::shared_ptr<Light>> FishEngine::Light::m_lights;

std::shared_ptr<Light> Light::Create()
{
    auto l = std::make_shared<Light>();
    m_lights.push_back(l);
    return l;
}

