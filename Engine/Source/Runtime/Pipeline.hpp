#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "FishEngine.hpp"
#include "Matrix4x4.hpp"
#include "ShaderVariables_gen.hpp"

namespace FishEngine
{
    class FE_EXPORT Pipeline
    {
    public:
        Pipeline() = delete;

        static void Init();

        static void BindCamera(const CameraPtr& camera);
        static void BindLight(const LightPtr& light);

        static void UpdatePerDrawUniforms(const Matrix4x4& modelMatrix);

        static void UpdateBonesUniforms(const std::vector<Matrix4x4>& bones);

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
    };
}

#endif // Pipeline_hpp
