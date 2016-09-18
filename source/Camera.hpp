#ifndef Camera_hpp
#define Camera_hpp

#include "Behaviour.hpp"
#include "GameObject.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Scene;

enum class CameraType {
    Game,
    SceneView,
    Preview,
};

class Camera : public Behaviour
{
public:
    InjectClassName(Camera)

    Camera(float fov, float aspect, float zNear, float zFar) 
        : m_fov(fov), m_aspect(aspect), m_zNear(zNear), m_zFar(zFar) {
    }

    // The aspect ratio (width divided by height).
    float aspect() const {
        return m_aspect;
    }

    void setAspect(float aspect) {
        m_aspect = aspect;
        m_isDirty = true;
    }

    // Matrix that transforms from world to camera space (i.e. view matrix).
    Matrix4x4 worldToCameraMatrix() const {
        return m_gameObject->transform()->worldToLocalMatrix();
    }

    // projection matrix.
    Matrix4x4 projectionMatrix() const {
        //return glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        if (m_isDirty) {
            m_projectMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
            m_isDirty = false;
        }
        return m_projectMatrix;
    }

    virtual void OnEditorGUI() override {
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
    
    // TODO
    // The first enabled camera tagged "MainCamera" (Read Only).
    static std::shared_ptr<Camera> main();

private:
    friend class RenderSystem;
    friend class EditorGUI;
    
    float m_fov;
    float m_aspect; // The aspect ratio (width divided by height).
    float m_zFar;
    float m_zNear;
    Vector4 m_viewport{0, 0, 1, 1};
    mutable bool m_isDirty = true;
    
    CameraType m_cameraType = CameraType::Game;

    mutable Matrix4x4 m_projectMatrix;
};

NAMESPACE_FISHENGINE_END

#endif /* Camera_hpp */
