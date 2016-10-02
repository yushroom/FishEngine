#include "Animator.hpp"
//#include <imgui/imgui.h>

#include "GameObject.hpp"
#include "Time.hpp"

using namespace FishEngine;

//void Animator::OnInspectorGUI() {
//    int channels = m_animation->channels.size();
//    ImGui::InputInt("Channel count", &channels);
//    if (m_playing) {
//        if (ImGui::Button("Stop")) {
//            Stop();
//        }
//        ImGui::InputFloat("Time", &m_time);
//    } else {
//        if (ImGui::Button("Play")) {
//            PlayOnce();
//        }
//    }
//}

void Animator::Update() {
    if (!m_playing)
        return;
    m_time += Time::deltaTime();
    if (m_time * m_animation->ticksPerSecond > m_animation->duration) {
        Stop();
        return;
    }
    RecursivelyUpdate(gameObject());
}

void Animator::RecursivelyUpdate(const std::shared_ptr<GameObject>& go)
{
    auto name = go->name();
    auto t = go->transform();
    auto it = m_animation->channels.find(name);
    auto tm = m_time * m_animation->ticksPerSecond;
    if (it != m_animation->channels.end()) {
        auto& node = it->second;
        for (auto& pk : node.positionKeys) {
            if (tm <= pk.time) {
                t->setLocalPosition(pk.value);
                break;
            }
        }
        for (auto& rk : node.rotationKeys) {
            if (tm <= rk.time) {
                t->setLocalRotation(rk.value);
                break;
            }
        }
        for (auto& sk : node.scalingKeys) {
            if (tm <= sk.time) {
                t->setLocalPosition(sk.value);
                break;
            }
        }
    }
    for (auto child : t->children()) {
        RecursivelyUpdate(child.lock()->gameObject());
    }
}
