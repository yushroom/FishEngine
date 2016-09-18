#include "Transform.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include <imgui/imgui.h>

NAMESPACE_FISHENGINE_BEGIN

Transform::Transform() : m_localPosition(0, 0, 0), m_localScale(1, 1, 1), m_localRotation(1, 0, 0, 0)
{

}

Transform::~Transform()
{
    Debug::Log("~Transform");
    SetParent(nullptr);
    for (auto child : m_children) {
        //Scene::des
    }
}

void Transform::OnInspectorGUI()
{
    if (ImGui::InputFloat3("Position", glm::value_ptr(m_localPosition))) {
        //m_isDirty = true;
    }
    if (ImGui::InputFloat3("Rotation", glm::value_ptr(m_localEulerAngles))) {
        m_localRotation = glm::quat(glm::radians((m_localEulerAngles)));
        //m_isDirty = true;
    }
    if (ImGui::InputFloat3("Scale", glm::value_ptr(m_localScale))) {
        //m_isDirty = true;
    }
}

void Transform::Update() const
{
    m_localEulerAngles = glm::degrees(glm::eulerAngles(m_localRotation));
    m_localToWorldMatrix = glm::scale(glm::translate(glm::mat4(1.0f), m_localPosition) * glm::mat4_cast(m_localRotation), m_localScale);
    auto p = m_parent.lock();
    if (p != nullptr)
        m_localToWorldMatrix = p->localToWorldMatrix() * m_localToWorldMatrix;
    m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);
}

void Transform::LookAt(const Vector3& target, const Vector3& worldUp /*= Vector3(0, 1, 0)*/)
{
    m_worldToLocalMatrix = glm::lookAt(m_localPosition, target, worldUp);
    m_localToWorldMatrix = glm::inverse(m_worldToLocalMatrix);
    m_localRotation = glm::quat_cast(m_localToWorldMatrix);
    //m_isDirty = true;
}

Vector3 Transform::TransformDirection(const Vector3& direction) const
{
    return m_localToWorldMatrix * Vector4(direction, 0);
}

void Transform::Translate(const Vector3& translation, Space relativeTo /*= Space::Self*/)
{
    if (relativeTo == Space::World)
        m_localPosition += translation;
    else
        m_localPosition += TransformDirection(translation);
    //m_isDirty = true;
}

void Transform::Rotate(Vector3 eulerAngles, Space relativeTo /*= Space::Self*/)
{
    Update();
    Quaternion lhs(glm::radians(eulerAngles));
    if (Space::Self == relativeTo) {
        auto r = this->rotation();
        m_localRotation = glm::inverse(r) * lhs * r * m_localRotation;
    }
    else {
        m_localRotation = lhs * m_localRotation;
    }
    //        m_isDirty = true;
}

void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
{
    auto rotation = angleAxis(angle, axis);
    m_localPosition = point + rotation * (m_localPosition - point);
    LookAt(point);
}

void Transform::SetParent(std::shared_ptr<Transform> parent)
{
    auto p = m_parent.lock();
    if (parent == p) {
        return;
    }
    // remove from old parent
    if (p != nullptr) {
        //p->m_children.remove(this);
        p->m_children.remove_if([this](std::weak_ptr<Transform> c) {
            return c.lock().get() == this;
        });
    }

    m_parent = parent;
    if (parent == nullptr) {
        return;
    }
    parent->m_children.push_back(m_gameObject.lock()->transform());
    //m_isDirty = true;
}

std::shared_ptr<Transform> Transform::GetChild(const int index) {
    if (index < -0 || index >= m_children.size()) {
        Debug::LogWarning("%s %d %s index out of range", __FILE__, __LINE__, __FUNCTION__);
        return nullptr;
    }
    
    auto p = m_children.begin();
    for (int i = 0; i < index; ++i) {
        p++;
    }
    return (*p).lock();
}

NAMESPACE_FISHENGINE_END
