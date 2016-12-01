#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "FishEngine.hpp"
#include "Matrix4x4.hpp"
#include "ShaderVariables_gen.hpp"
#include <stack>

namespace FishEngine
{
    class FE_EXPORT RenderTarget
    {
    public:
        RenderTarget() = default;

        void Set(ColorBufferPtr colorBuffer, DepthBufferPtr depthBuffer);
        void Set(ColorBufferPtr colorBuffer1, ColorBufferPtr colorBuffer2, ColorBufferPtr colorBuffer3, DepthBufferPtr depthBuffer);

        void Attach();
        void Detach();

    private:
        uint32_t  m_activeColorBufferCount = 1;
        ColorBufferPtr m_colorBuffers[3];
        DepthBufferPtr m_depthBuffer;
        unsigned int m_fbo = 0;

        void Init();
    };

    class FE_EXPORT Pipeline
    {
    public:
        Pipeline() = delete;

        static void Init();

        static void BindCamera(const CameraPtr& camera);
        static void BindLight(const LightPtr& light);

        static void UpdatePerDrawUniforms(const Matrix4x4& modelMatrix);

        static void UpdateBonesUniforms(const std::vector<Matrix4x4>& bones);

        static RenderTargetPtr CurrentRenderTarget()
        {
            s_renderTargetStack.top();
        }

        static void PushRenderTarget(const RenderTargetPtr& renderTarget);

        static void PopRenderTarget();

        static constexpr unsigned int PerCameraUBOBindingPoint  = 0;
        static constexpr unsigned int PerDrawUBOBindingPoint    = 1;
        static constexpr unsigned int LightingUBOBindingPoint   = 2;
        static constexpr unsigned int BonesUBOBindingPoint      = 3;

    private:
        static unsigned int         s_perCameraUBO;
        static unsigned int         s_perDrawUBO;
        static unsigned int         s_lightingUBO;
        static unsigned int         s_bonesUBO;
        static PerCameraUniforms    s_perCameraUniforms;
        static PerDrawUniforms      s_perDrawUniforms;
        static LightingUniforms     s_lightingUniforms;
        //static Bones        s_bonesUniformData;

        static RenderTargetPtr      s_currentRenderTarget;

        static std::stack<RenderTargetPtr> s_renderTargetStack;
    };
}

#endif // Pipeline_hpp
