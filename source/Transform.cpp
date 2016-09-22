#include "Transform.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include <imgui/imgui.h>

NAMESPACE_FISHENGINE_BEGIN

Transform::Transform() : m_localPosition(0, 0, 0), m_localScale(1, 1, 1), m_localRotation(0, 0, 0, 1)
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
    if (ImGui::InputFloat3("Position", m_localPosition.data())) {
        MakeDirty();
    }
    if (ImGui::InputFloat3("Rotation", m_localEulerAngles.data())) {
        m_localRotation.setEulerAngles(m_localEulerAngles);
        MakeDirty();
    }
    if (ImGui::InputFloat3("Scale", m_localScale.data())) {
        MakeDirty();
    }
}

void Transform::Update() const
{
    if (!dirtyInHierarchy())
        return;
    m_localEulerAngles = m_localRotation.eulerAngles();
    //m_localToWorldMatrix.SetTRS(m_localPosition, m_localRotation, m_localScale);
    Matrix4x4::TRS(m_localPosition, m_localRotation, m_localScale, m_localToWorldMatrix, m_worldToLocalMatrix);
    if (!m_parent.expired()) {
        m_localToWorldMatrix = m_parent.lock()->localToWorldMatrix() * m_localToWorldMatrix;
        m_worldToLocalMatrix = m_worldToLocalMatrix * m_parent.lock()->worldToLocalMatrix();
    }
    //m_worldToLocalMatrix = m_localToWorldMatrix.inverse();
    m_rotation = m_localToWorldMatrix.ToRotation();
    m_isDirty = false;
}

void Transform::LookAt(const Vector3& target, const Vector3& worldUp /*= Vector3(0, 1, 0)*/)
{
    auto m = Matrix4x4::LookAt(m_localPosition, target, worldUp);
    m_localRotation = m.inverse().ToRotation();
    MakeDirty();
}

Vector3 Transform::TransformDirection(const Vector3& direction) const
{
    Update();
    return m_localToWorldMatrix.MultiplyVector(direction);
}

Vector3 FishEngine::Transform::InverseTransformDirection(const Vector3& direction) const
{
    Update();
    return m_worldToLocalMatrix.MultiplyVector(direction);
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
    auto rotation = Quaternion::AngleAxis(angle, axis);
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

std::shared_ptr<Transform> Transform::GetChild(const size_t index) {
    if (index < 0 || index >= m_children.size()) {
        Debug::LogWarning("%s %d %s index out of range", __FILE__, __LINE__, __FUNCTION__);
        return nullptr;
    }
    
    auto p = m_children.begin();
    for (size_t i = 0; i < index; ++i) {
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
        m_isDirty = true;
    }
}


NAMESPACE_FISHENGINE_END
