#include "Animator.hpp"
//#include <imgui/imgui.h>

#include "GameObject.hpp"
#include "Time.hpp"

using namespace FishEngine;

void Animator::Update() {
    if (!m_playing)
        return;
    m_time += Time::deltaTime();
    if (m_time * m_animation->ticksPerSecond > m_animation->duration) {
        Stop();
        return;
    }
    RecursivelyUpdate(gameObject());
    //Stop();
    //Debug::Log("time %lf", m_time);
    if (m_playOneFrame) {
        m_playing = false;
        m_playOneFrame = false;
    }
}

void Animator::RecursivelyUpdate(const std::shared_ptr<GameObject>& go)
{
    auto name = go->name();
    auto t = go->transform();
    auto it = m_animation->channels.find(name);
    auto tm = m_time * m_animation->ticksPerSecond;
    if (it != m_animation->channels.end()) {
        auto& node = it->second;
        auto pos = t->localPosition();
        for (auto& pk : node.positionKeys) {
            if (tm < pk.time) {
                break;
            }
            pos = pk.value;
        }


        auto rot = t->localRotation();
        for (auto& rk : node.rotationKeys) {
            if (tm < rk.time) {
                break;
            }
            rot = rk.value;
        }


        auto s = t->localScale();
        for (auto& sk : node.scalingKeys) {
            if (tm < sk.time) {
                break;
            }
            s = sk.value;
        }
#if 1
        //auto pos = Vector3::zero;
        //auto rot = Quaternion::identity;
        //auto s = Vector3::one;
        //uint32_t j = 0;
        //for (j = 1; j < node.positionKeys.size() && tm >= node.positionKeys[j].time; ++j) {}
        //pos = node.positionKeys[j - 1].value;
        //for (j = 1; j < node.rotationKeys.size() && tm >= node.rotationKeys[j].time; ++j) {}
        //rot = node.rotationKeys[j - 1].value;
        //for (j = 1; j < node.scalingKeys.size() && tm >= node.scalingKeys[j].time; ++j) {}
        //s = node.scalingKeys[j - 1].value;

        //Matrix4x4 m = Matrix4x4::TRS(pos, rot, s);
        //t->setLocalToWorldMatrix(m);
        t->setLocalPosition(pos);
        t->setLocalRotation(rot);
        t->setLocalScale(s);
#else
        Matrix4x4 m;
        uint32_t j = 0;
        for (j = 1; j < node.transformationKeys.size() && tm > node.transformationKeys[j].time; ++j) {}
        m = node.transformationKeys[j - 1].value;
        t->setLocalToWorldMatrix(m);
#endif
    }
    for (auto child : t->children()) {
        RecursivelyUpdate(child.lock()->gameObject());
    }
}

void FishEngine::Animator::NextFrame()
{
    m_playing = true;
    m_playOneFrame = true;
    //m_currentFrame++;
    //RecursivelyUpdate2(gameObject());
}

void FishEngine::Animator::RecursivelyUpdate2(const std::shared_ptr<GameObject>& go)
{
    auto name = go->name();
    auto t = go->transform();
    auto it = m_animation->channels.find(name);
    //auto tm = m_time * m_animation->ticksPerSecond;
    if (it != m_animation->channels.end()) {
        auto& node = it->second;
        t->setLocalToWorldMatrix(node.transformationKeys[m_currentFrame].value);
    }

    for (auto child : t->children()) {
        RecursivelyUpdate2(child.lock()->gameObject());
    }
}
