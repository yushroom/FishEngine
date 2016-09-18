#include "Camera.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include <imgui/imgui.h>

NAMESPACE_FISHENGINE_BEGIN

FishEngine::Camera::Camera(float fov, float aspect, float zNear, float zFar) : m_fov(fov), m_aspect(aspect), m_zNear(zNear), m_zFar(zFar)
{

}


FishEngine::Matrix4x4 FishEngine::Camera::projectionMatrix() const
{
    //return glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
    if (m_isDirty) {
        m_projectMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        m_isDirty = false;
    }
    return m_projectMatrix;
}

void FishEngine::Camera::OnEditorGUI()
{
    if (ImGui::SliderFloat("Field of View", &m_fov, 1, 179)) {
        m_isDirty = true;
    }
    if (ImGui::InputFloat("Clipping Planes(Near)", &m_zNear)) {
        m_isDirty = true;
    }
    if (ImGui::InputFloat("Clipping Planes(Far)", &m_zFar)) {
        m_isDirty = true;
    }
    ImGui::InputFloat4("Viewport Rect", glm::value_ptr(m_viewport));
}

FishEngine::Matrix4x4 FishEngine::Camera::worldToCameraMatrix() const
{
    return m_gameObject->transform()->worldToLocalMatrix();
}

std::shared_ptr<Camera> Camera::main()
{
    return Scene::mainCamera();
}

NAMESPACE_FISHENGINE_END
