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
    SetParent(nullptr); // remove from parent
    for (auto child : m_children) {
        //Scene::des
    }
}

void Transform::OnInspectorGUI()
{
    if (ImGui::InputFloat3("Position", glm::value_ptr(m_localPosition))) {
        MakeDirty();
    }
    if (ImGui::InputFloat3("Rotation", glm::value_ptr(m_localEulerAngles))) {
        m_localRotation = glm::quat(glm::radians((m_localEulerAngles)));
        MakeDirty();
    }
    if (ImGui::InputFloat3("Scale", glm::value_ptr(m_localScale))) {
        MakeDirty();
    }
}

void Transform::Update() const
{
    if (!dirtyInHierarchy())
        return;
    m_localEulerAngles = glm::degrees(glm::eulerAngles(m_localRotation));
    m_localToWorldMatrix = glm::scale(glm::translate(glm::mat4(1.0f), m_localPosition) * glm::mat4_cast(m_localRotation), m_localScale);
    if (!m_parent.expired())
        m_localToWorldMatrix = m_parent.lock()->localToWorldMatrix() * m_localToWorldMatrix;
    m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);
    m_rotation = glm::quat_cast(m_localToWorldMatrix);
    m_isDirty = false;
}

void Transform::LookAt(const Vector3& target, const Vector3& worldUp /*= Vector3(0, 1, 0)*/)
{
    auto m = glm::lookAt(m_localPosition, target, worldUp);
    m_localRotation = glm::quat_cast(glm::inverse(m));
    MakeDirty();
}

Vector3 Transform::TransformDirection(const Vector3& direction) const
{
    Update();
    return m_localToWorldMatrix * Vector4(direction, 0);
}

Vector3 FishEngine::Transform::InverseTransformDirection(const Vector3& direction) const
{
    Update();
    return m_worldToLocalMatrix * Vector4(direction, 0);
}

void Transform::Translate(const Vector3& translation, Space relativeTo /*= Space::Self*/)
{
    if (relativeTo == Space::World) {
        setPosition(position() + translation);
    }
    else {
        setPosition(position() + TransformDirection(translation));
    }
    MakeDirty();
}

//void Transform::Rotate(Vector3 eulerAngles, Space relativeTo /*= Space::Self*/)
//{
//    Quaternion lhs(glm::radians(eulerAngles));
//    if (Space::Self == relativeTo) {
//        //auto r = this->rotation();
//        //m_localRotation = glm::inverse(r) * lhs * r * m_localRotation;
//        m_localRotation = lhs * m_localRotation;
//    }
//    else {
//        //m_localRotation = lhs * m_localRotation;
//        lhs * rotation();
//        // TODO
//    }
//}

void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
{
    // step1: update position
    auto vector = this->position();
    auto rotation = angleAxis(angle, axis);
    Vector3 vector2 = vector - point;
    vector2 = rotation * vector2;
    vector = point + vector2;
    setPosition(vector);

    // step2: update rotation
    m_localRotation = rotation * m_localRotation;
    //RotateAroundInternal(axis, angle);
    MakeDirty();
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
    parent->m_children.push_back(gameObject()->transform());
    MakeDirty();
}

std::shared_ptr<Transform> Transform::GetChild(const int index) {
    if (index < 0 || index >= m_children.size()) {
        Debug::LogWarning("%s %d %s index out of range", __FILE__, __LINE__, __FUNCTION__);
        return nullptr;
    }
    
    auto p = m_children.begin();
    for (int i = 0; i < index; ++i) {
        p++;
    }
    return p->lock();
}


bool Transform::dirtyInHierarchy() const
{
    if (!m_isDirty && !m_parent.expired()) { // not dirty and has a parent
        return m_parent.lock()->dirtyInHierarchy();
    }
    return m_isDirty;
}


void FishEngine::Transform::MakeDirty() const
{
    if (!m_isDirty) {
        for (auto& c : m_children) {
            c.lock()->MakeDirty();
        }
    }
    m_isDirty = true;
}


NAMESPACE_FISHENGINE_END
