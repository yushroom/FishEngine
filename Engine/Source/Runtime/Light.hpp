#ifndef Light_hpp
#define Light_hpp

#include "Behaviour.hpp"
#include "Color.hpp"
#include "RenderTexture.hpp"
#include "Common.hpp"
#include "Matrix4x4.hpp"

// reference:
// https://docs.unity3d.com/Manual/Lighting.html
// https://docs.unity3d.com/Manual/class-Light.html
// https://docs.unity3d.com/Manual/LightPerformance.html
// https://docs.unity3d.com/Manual/DirLightShadows.html

namespace FishEngine
{
    enum class LightType
    {
        Directional,
        Point,
        Spot,
        Area,
    };

    template<>
    constexpr int EnumCount<LightType>() { return 4; }

    // string array
    static const char* LightTypeStrings[] = {
        "Directional",
        "Point",
        "Spot",
        "Area",
    };

    // index to enum
    template<>
    inline LightType ToEnum<LightType>(const int index)
    {
        switch (index) {
        case 0: return LightType::Directional; break;
        case 1: return LightType::Point; break;
        case 2: return LightType::Spot; break;
        case 3: return LightType::Area; break;
        default: abort(); break;
        }
    }

    // enum to index
    inline int ToIndex(LightType e)
    {
        switch (e) {
        case LightType::Directional: return 0; break;
        case LightType::Point: return 1; break;
        case LightType::Spot: return 2; break;
        case LightType::Area: return 3; break;
        default: abort(); break;
        }
    }

    // enum to string
    inline const char* ToString(LightType e)
    {
        switch (e) {
        case LightType::Directional: return "Directional"; break;
        case LightType::Point: return "Point"; break;
        case LightType::Spot: return "Spot"; break;
        case LightType::Area: return "Area"; break;
        default: abort(); break;
        }
    }

    // string to enum
    template<>
    inline LightType ToEnum<LightType>(const std::string& s)
    {
        if (s == "Directional") return LightType::Directional;
        if (s == "Point") return LightType::Point;
        if (s == "Spot") return LightType::Spot;
        if (s == "Area") return LightType::Area;
        abort();
    }

    class FE_EXPORT Light : public Behaviour
    {
    public:

        InjectClassName(Light);
        Light() {
            m_shadowMap = RenderTexture::CreateShadowMap();
        }

        static LightPtr Create();

        static std::list<LightPtr>& lights() {
            return m_lights;
        }

        float shadowNearPlane() const {
            return m_shadowNearPlane;
        }

        virtual void OnDrawGizmos() override;
        virtual void OnDrawGizmosSelected() override;

    private:
        friend class Scene;
        //friend class FishEditor::EditorRenderSystem;
        //friend class RenderSystem;
        friend class FishEditor::EditorGUI;
        friend class MeshRenderer;
        friend class SkinnedMeshRenderer;
        friend class Graphics;
        friend class Pipeline;
        friend class Serialization;

        // The current type of light. Possible values are Directional, Point, Spot and Area
        LightType m_type = LightType::Directional;

        // How far light is emitted from the center of the object (Point and Spot lights only).
        float m_range;

        // Determines the angle(in degrees) at the base of a spot light¡¯s cone(Spot light only).
        float m_spotAngle;

        // The color of the light emitted.
        Color m_color;

        // Brightness of the light. The default value for a Point, Spot or Area light is 1 but for a Directional light, it is 0.5.
        float m_intensity           = 1.0f;

        float m_shadowBias          = 0.2f;
        float m_shadowNormalBias    = 0.4f;
        float m_shadowNearPlane     = 0.2f;

        RenderTexturePtr m_shadowMap;
        Matrix4x4 m_viewMatrixForShadowMap;
        Matrix4x4 m_projectMatrixForShadowMap;

        static std::list<LightPtr> m_lights;
    };
}

#endif // Light_hpp
