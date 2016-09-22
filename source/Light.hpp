#ifndef Light_hpp
#define Light_hpp

#include "Behaviour.hpp"
#include "Color.hpp"

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
    Light() = default;

    static std::shared_ptr<Light> Create();

    static std::list<std::shared_ptr<Light>>& lights() {
        return m_lights;
    }

    virtual void OnInspectorGUI() override;

private:

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

    static std::list<std::shared_ptr<Light>> m_lights;
};

NAMESPACE_FISHENGINE_END

#endif // Light_hpp