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

    class FE_EXPORT Light : public Behaviour
    {
    public:

        InjectClassName(Light);

        Light();

        static LightPtr Create();

        static std::list<LightPtr>& lights()
        {
            return m_lights;
        }

        float shadowNearPlane() const
        {
            return m_shadowNearPlane;
        }

        virtual void OnDrawGizmos() override;
        virtual void OnDrawGizmosSelected() override;

    private:
        friend class Scene;
        //friend class FishEditor::EditorRenderSystem;
        friend class RenderSystem;
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

        //LayeredColorBufferPtr m_shadowMap;
        LayeredDepthBufferPtr m_shadowMap;
        RenderTargetPtr m_renderTarget;
        Matrix4x4 m_viewMatrixForShadowMap[4];
        Matrix4x4 m_projectMatrixForShadowMap[4];

        Vector4 m_cascadesNear;
        Vector4 m_cascadesFar;
        Vector4 m_cascadesSplitPlaneNear;
        Vector4 m_cascadesSplitPlaneFar;

        static std::list<LightPtr> m_lights;
    };
}

#endif // Light_hpp
