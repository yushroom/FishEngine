#include "Pipeline.hpp"
#include "GLEnvironment.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Light.hpp"

namespace FishEngine
{
    PerDrawUniforms     Pipeline::s_perDrawUniforms;
    LightingUniforms    Pipeline::s_lightingUniforms;
    PerCameraUniforms   Pipeline::s_perCameraUniforms;

    unsigned int        Pipeline::s_perCameraUBO = 0;
    unsigned int        Pipeline::s_perDrawUBO = 0;
    unsigned int        Pipeline::s_lightingUBO = 0;
    unsigned int        Pipeline::s_bonesUBO = 0;

    void Pipeline::Init()
    {
        glGenBuffers(1, &s_perCameraUBO);
        glGenBuffers(1, &s_perDrawUBO);
        glGenBuffers(1, &s_lightingUBO);
        glGenBuffers(1, &s_bonesUBO);
    }

    void Pipeline::BindCamera(const CameraPtr& camera)
    {
        auto proj = camera->projectionMatrix();
        auto view = camera->worldToCameraMatrix();
        s_perCameraUniforms.MATRIX_P = proj;
        s_perCameraUniforms.MATRIX_V = view;
        s_perCameraUniforms.MATRIX_I_V = view.inverse();
        s_perCameraUniforms.MATRIX_VP = proj * view;
        s_perCameraUniforms.WorldSpaceCameraPos = Vector4(camera->transform()->position(), 1);
        float t = Time::time();
        s_perCameraUniforms.Time = Vector4(t / 20.f, t, t*2.f, t*3.f);

        glBindBuffer(GL_UNIFORM_BUFFER, s_perCameraUBO);
        //auto size = sizeof(perFrameUniformData);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(s_perCameraUniforms), (void*)&s_perCameraUniforms, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, PerCameraUBOBindingPoint, s_perCameraUBO);
        glCheckError();
    }

    void Pipeline::BindLight(const LightPtr& light)
    {
        s_lightingUniforms.LightColor = light->m_color;
        s_lightingUniforms.WorldSpaceLightPos = Vector4(-light->transform()->forward(), 0);
        s_lightingUniforms.LightMatrix = light->m_projectMatrixForShadowMap * light->m_viewMatrixForShadowMap;
        
        glBindBuffer(GL_UNIFORM_BUFFER, s_lightingUBO);
        //auto size = sizeof(perFrameUniformData);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(s_lightingUniforms), (void*)&s_lightingUniforms, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, LightingUBOBindingPoint, s_lightingUBO);
        glCheckError();
    }

    void Pipeline::UpdatePerDrawUniforms(const Matrix4x4& modelMatrix)
    {
        auto mv = Pipeline::s_perCameraUniforms.MATRIX_V * modelMatrix;
        s_perDrawUniforms.MATRIX_MVP = Pipeline::s_perCameraUniforms.MATRIX_VP * modelMatrix;
        s_perDrawUniforms.MATRIX_MV = mv;
        s_perDrawUniforms.MATRIX_M = modelMatrix;
        s_perDrawUniforms.MATRIX_IT_MV = mv.transpose().inverse();
        s_perDrawUniforms.MATRIX_IT_M = modelMatrix.transpose().inverse();

        glBindBuffer(GL_UNIFORM_BUFFER, s_perDrawUBO);
        //auto size = sizeof(perDrawUniformData);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(s_perDrawUniforms), (void*)&s_perDrawUniforms, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, PerDrawUBOBindingPoint, s_perDrawUBO);
        glCheckError();
    }

    void Pipeline::UpdateBonesUniforms(const std::vector<Matrix4x4>& bones)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, s_bonesUBO);
        //auto size = sizeof(perFrameUniformData);
        glBufferData(GL_UNIFORM_BUFFER, bones.size() * sizeof(Matrix4x4), (void*)bones.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, BonesUBOBindingPoint, s_bonesUBO);
        glCheckError();
    }

}
