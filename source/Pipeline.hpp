#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "FishEngine.hpp"
#include "Matrix4x4.hpp"
#include "GLEnvironment.hpp"

#define CPP
#define layout(...)
#define uniform struct
#define mat4 FishEngine::Matrix4x4
#define vec3 FishEngine::Vector3
#define vec4 FishEngine::Vector4
#include "../assets/shaders/include/ShaderVariables.inc"
#undef uniform
#undef vec4
#undef vec3
#undef mat4
#undef layout
#undef CPP

namespace FishEngine
{
    class Pipeline
    {
    public:
        Pipeline() = delete;

        static void Init()
        {
            glGenBuffers(1, &perDrawUBO);
            glGenBuffers(1, &perFrameUBO);
            //glGenBuffers(1, &bonesUBO);
        }

        static void BindPerDrawUniforms()
        {
            glBindBuffer(GL_UNIFORM_BUFFER, perDrawUBO);
            //auto size = sizeof(perDrawUniformData);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(perDrawUniformData), (void*)&perDrawUniformData, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, PerDrawUBOBindingPoint, perDrawUBO);
            glCheckError();
        }

        static void BindPerFrameUniforms()
        {
            glBindBuffer(GL_UNIFORM_BUFFER, perFrameUBO);
            //auto size = sizeof(perFrameUniformData);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(perFrameUniformData), (void*)&perFrameUniformData, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, PerFrameUBOBindingPoint, perFrameUBO);
            glCheckError();
        }
        
//        static void BindBonesUniforms()
//        {
//            glBindBuffer(GL_UNIFORM_BUFFER, bonesUBO);
//            //auto size = sizeof(perFrameUniformData);
//            glBufferData(GL_UNIFORM_BUFFER, sizeof(bonesUniformData), (void*)&bonesUniformData, GL_DYNAMIC_DRAW);
//            glBindBufferBase(GL_UNIFORM_BUFFER, BonesUBOBindingPoint, bonesUBO);
//            glCheckError();
//        }

        static const GLuint PerDrawUBOBindingPoint = 0;
        static const GLuint PerFrameUBOBindingPoint = 1;
        //static const GLuint BonesUBOBindingPoint = 2;

        static PerDraw perDrawUniformData;
        static PerFrame perFrameUniformData;
        //static Bones bonesUniformData;

    private:
        static GLuint perDrawUBO;
        static GLuint perFrameUBO;
        //static GLuint bonesUBO;
    };
}

#endif // Pipeline_hpp
