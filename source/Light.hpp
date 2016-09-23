#ifndef Light_hpp
#define Light_hpp

#include "Behaviour.hpp"
#include "Color.hpp"
#include "RenderTexture.hpp"

// reference:
// https://docs.unity3d.com/Manual/Lighting.html
// https://docs.unity3d.com/Manual/class-Light.html


NAMESPACE_FISHENGINE_BEGIN

enum class LightType
{
    Directional,
    Point,
    Spot,
    Area,
};

class Light : public Behaviour
{
public:

    InjectClassName(Light);
    Light() {
        m_shadowMap = RenderTexture::CreateShadowMap();
    }

    static std::shared_ptr<Light> Create();

    static std::list<std::shared_ptr<Light>>& lights() {
        return m_lights;
    }
    
    float shadowNearPlane() const {
        return m_shadowNearPlane;
    }

    virtual void OnInspectorGUI() override;

private:
    friend class Scene;
    friend class FishEditor::EditorRenderSystem;
    friend class MeshRenderer;
    // The current type of light. Possible values are Directional, Point, Spot and Area
    LightType m_type;

    // How far light is emitted from the center of the object (Point and Spot lights only).
    float m_range;

    // Determines the angle(in degrees) at the base of a spot light¡¯s cone(Spot light only).
    float m_spotAngle;

    // The color of the light emitted.
    Color m_color;

    // Brightness of the light. The default value for a Point, Spot or Area light is 1 but for a Directional light, it is 0.5.
    float m_intensity;
    
    float m_shadowBias = 0.2f;
    float m_shadowNormalBias = 0.4f;
    float m_shadowNearPlane = 0.2f;
    
    std::shared_ptr<RenderTexture> m_shadowMap;

    static std::list<std::shared_ptr<Light>> m_lights;
};

NAMESPACE_FISHENGINE_END

#endif // Light_hpp
