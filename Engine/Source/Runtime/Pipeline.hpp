#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "FishEngine.hpp"
#include "Matrix4x4.hpp"
#include "ShaderVariables_gen.hpp"

namespace FishEngine
{
    class Pipeline
    {
    public:
        Pipeline() = delete;

        static void Init();

        static void BindCamera(const CameraPtr& camera);
        static void BindLight(const LightPtr& light);

        static void UpdatePerDrawUniforms(const Matrix4x4& modelMatrix);

        static void UpdatePerFrameUniforms();
        
//        static void BindBonesUniforms()
//        {
//            glBindBuffer(GL_UNIFORM_BUFFER, bonesUBO);
//            //auto size = sizeof(perFrameUniformData);
//            glBufferData(GL_UNIFORM_BUFFER, sizeof(bonesUniformData), (void*)&bonesUniformData, GL_DYNAMIC_DRAW);
//            glBindBufferBase(GL_UNIFORM_BUFFER, BonesUBOBindingPoint, bonesUBO);
//            glCheckError();
//        }

        static constexpr unsigned int PerDrawUBOBindingPoint = 0;
        static constexpr unsigned int PerFrameUBOBindingPoint = 1;
        //static const GLuint BonesUBOBindingPoint = 2;

    private:
        static unsigned int s_perDrawUBO;
        static unsigned int s_perFrameUBO;
        //static GLuint bonesUBO;
        static PerDraw      s_perDrawUniformData;
        static PerFrame     s_perFrameUniformData;
        //static Bones bonesUniformData;
    };
}

#endif // Pipeline_hpp
