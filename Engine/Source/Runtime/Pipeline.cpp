#include "Pipeline.hpp"
#include "GLEnvironment.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Light.hpp"

namespace FishEngine
{
    PerDraw Pipeline::s_perDrawUniformData;
    PerFrame Pipeline::s_perFrameUniformData;
    //Bones Pipeline::bonesUniformData;
    GLuint Pipeline::s_perDrawUBO = 0;
    GLuint Pipeline::s_perFrameUBO = 0;
    //GLuint Pipeline::bonesUBO = 0;

    void Pipeline::Init()
    {
        glGenBuffers(1, &s_perDrawUBO);
        glGenBuffers(1, &s_perFrameUBO);
        //glGenBuffers(1, &bonesUBO);
    }

    void Pipeline::BindCamera(const CameraPtr& camera)
    {
        auto proj = camera->projectionMatrix();
        auto view = camera->worldToCameraMatrix();
        s_perFrameUniformData.MATRIX_P = proj;
        s_perFrameUniformData.MATRIX_V = view;
        s_perFrameUniformData.MATRIX_I_V = view.inverse();
        s_perFrameUniformData.MATRIX_VP = proj * view;
        s_perFrameUniformData.WorldSpaceCameraPos = camera->transform()->position();
        float t = Time::time();
        s_perFrameUniformData._Time = Vector4(t / 20.f, t, t*2.f, t*3.f);
    }

    void Pipeline::BindLight(const LightPtr& light)
    {
        s_perFrameUniformData.LightColor0 = light->m_color;
        s_perFrameUniformData.WorldSpaceLightPos0 = Vector4(-light->transform()->forward(), 0);
        s_perFrameUniformData.LightMatrix0 = light->m_projectMatrixForShadowMap * light->m_viewMatrixForShadowMap;
    }

    void Pipeline::UpdatePerDrawUniforms(const Matrix4x4& modelMatrix)
    {
        auto mv = Pipeline::s_perFrameUniformData.MATRIX_V * modelMatrix;
        s_perDrawUniformData.MATRIX_MVP = Pipeline::s_perFrameUniformData.MATRIX_VP * modelMatrix;
        s_perDrawUniformData.MATRIX_MV = mv;
        s_perDrawUniformData.MATRIX_M = modelMatrix;
        s_perDrawUniformData.MATRIX_IT_MV = mv.transpose().inverse();
        s_perDrawUniformData.MATRIX_IT_M = modelMatrix.transpose().inverse();

        glBindBuffer(GL_UNIFORM_BUFFER, s_perDrawUBO);
        //auto size = sizeof(perDrawUniformData);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(s_perDrawUniformData), (void*)&s_perDrawUniformData, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, PerDrawUBOBindingPoint, s_perDrawUBO);
        glCheckError();
    }

    void Pipeline::UpdatePerFrameUniforms()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, s_perFrameUBO);
        //auto size = sizeof(perFrameUniformData);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(s_perFrameUniformData), (void*)&s_perFrameUniformData, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, PerFrameUBOBindingPoint, s_perFrameUBO);
        glCheckError();
    }
}
