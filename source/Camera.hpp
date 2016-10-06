#ifndef Camera_hpp
#define Camera_hpp

#include "Script.hpp"
#include "GLEnvironment.hpp"
#include "Matrix4x4.hpp"
#include "Ray.hpp"

NAMESPACE_FISHENGINE_BEGIN

enum class CameraType {
    Game,
    SceneView,
    Preview,
};

class Camera : public Behaviour
{
public:
    InjectClassName(Camera)

    static std::shared_ptr<Camera>
    Create(float fov,
           float aspect,
           float nearClipPlane,
           float farClipPlane,
           CameraType type = CameraType::Game)
    {
        auto camera = std::make_shared<Camera>(fov, aspect, nearClipPlane, farClipPlane);
        camera->m_cameraType = type;
        m_allCameras.push_back(camera);
        return camera;
    }
 
//private:
    Camera(float fov, float aspect, float nearClipPlane, float farClipPlane);

//public:
    // The aspect ratio (width divided by height).
    float aspect() const {
        return m_aspect;
    }

    bool orghographic() const {
        return m_orthographic;
    }

    void setOrthographic(bool value) {
        m_orthographic = value;
        m_isDirty = true;
    }

    void setAspect(float aspect) {
        m_aspect = aspect;
        m_isDirty = true;
    }

    float nearClipPlane() const {
        return m_nearClipPlane;
    }

    float farClipPlane() const {
        return m_farClipPlane;
    }

    // Matrix that transforms from world to camera space (i.e. view matrix).
    Matrix4x4 worldToCameraMatrix() const;

    // projection matrix.
    Matrix4x4 projectionMatrix() const;

    const Vector4& viewport() const {
        return m_viewport;
    }

    //virtual void OnInspectorGUI() override;

    // Returns a ray going from camera through a screen point.
    Ray ScreenPointToRay(const Vector3& position);
    
    // TODO
    // The first enabled camera tagged "MainCamera" (Read Only).
    static std::shared_ptr<Camera> main();
    
    static std::shared_ptr<Camera> mainGameCamera();

private:
    friend class RenderSystem;
    friend class FishEditor::EditorGUI;
    friend class FishEditor::FishEditorWindow;
    
    float m_fieldOfView;
    float m_orthographicSize = 5.f;   // Projection's half-size(vertical) when in orthographic mode.
    float m_aspect; // The aspect ratio (width divided by height).
    float m_farClipPlane;
    float m_nearClipPlane;
    Vector4 m_viewport{0, 0, 1, 1};
    mutable bool m_isDirty = true;
    
    CameraType  m_cameraType        = CameraType::Game;
    bool        m_orthographic      = false;

    mutable Matrix4x4 m_projectMatrix;

    // temp
    // https://docs.unity3d.com/Manual/SceneViewNavigation.html
    mutable Vector3 m_focusPoint{0, 0, 0};
    void FrameSelected(std::shared_ptr<GameObject>& selected);
    
    static std::shared_ptr<Camera> m_mainCamera;
    static std::vector<std::shared_ptr<Camera>> m_allCameras;
};

NAMESPACE_FISHENGINE_END

#endif /* Camera_hpp */
