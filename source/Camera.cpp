#include "Camera.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include <imgui/imgui.h>
#include "Screen.hpp"

NAMESPACE_FISHENGINE_BEGIN

Camera::Camera(float fov, float aspect, float zNear, float zFar) : m_fov(fov), m_aspect(aspect), m_zNear(zNear), m_zFar(zFar)
{

}


Matrix4x4 Camera::projectionMatrix() const
{
    if (m_isDirty) {
        m_projectMatrix = Matrix4x4::Perspective(m_fov, m_aspect, m_zNear, m_zFar);
        m_isDirty = false;
    }
    return m_projectMatrix;
}

void Camera::OnInspectorGUI()
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
    ImGui::InputFloat4("Viewport Rect", m_viewport.data());
}

FishEngine::Ray FishEngine::Camera::ScreenPointToRay(const Vector3& position)
{
    //http://antongerdelan.net/opengl/raycasting.html

    // NDC space
    float x = (2.0f * position.x) / Screen::width() - 1.0f;
    float y = (2.0f * position.y) / Screen::height() - 1.0f;
    Vector4 ray_clip(x, y, 1.f, 1.0f);

    Vector4 ray_eye = projectionMatrix().inverse() * ray_clip;
    ray_eye.z = 1.0f; // forward
    ray_eye.w = 0.0f;

    Vector4 ray_world_h = transform()->localToWorldMatrix() * ray_eye;
    Vector3 ray_world(ray_world_h.x, ray_world_h.y, ray_world_h.z);
    return Ray(transform()->position(), ray_world.normalized());
}

Matrix4x4 Camera::worldToCameraMatrix() const
{
    return transform()->worldToLocalMatrix();
}

std::shared_ptr<Camera> Camera::main()
{
    return Scene::mainCamera();
}

NAMESPACE_FISHENGINE_END
