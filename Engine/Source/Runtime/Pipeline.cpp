#include "Pipeline.hpp"
#include "GLEnvironment.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Light.hpp"
#include "Screen.hpp"
#include "RenderTexture.hpp"

namespace FishEngine
{
    PerDrawUniforms     Pipeline::s_perDrawUniforms;
    LightingUniforms    Pipeline::s_lightingUniforms;

    FishEngine::RenderTargetPtr Pipeline::s_currentRenderTarget;

    std::stack<RenderTargetPtr> Pipeline::s_renderTargetStack;

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
        s_perCameraUniforms.WorldSpaceCameraDir = Vector4(camera->transform()->forward(), 0);

        float t = Time::time();
        s_perCameraUniforms.Time = Vector4(t / 20.f, t, t*2.f, t*3.f);
        
        float far = camera->farClipPlane();
        float near = camera->nearClipPlane();

        s_perCameraUniforms.ProjectionParams.x = 1.0f;
        s_perCameraUniforms.ProjectionParams.y = camera->nearClipPlane();
        s_perCameraUniforms.ProjectionParams.z = camera->farClipPlane();
        s_perCameraUniforms.ProjectionParams.w = 1.0f / camera->farClipPlane();

        s_perCameraUniforms.ScreenParams.x = Screen::width();
        s_perCameraUniforms.ScreenParams.y = Screen::height();
        s_perCameraUniforms.ScreenParams.z = 1.0f + 1.0f / Screen::width();
        s_perCameraUniforms.ScreenParams.w = 1.0f + 1.0f / Screen::height();

        s_perCameraUniforms.ZBufferParams.x = 1.0f - far / near;
        s_perCameraUniforms.ZBufferParams.y = far / near;
        s_perCameraUniforms.ZBufferParams.z = s_perCameraUniforms.ZBufferParams.x / far;
        s_perCameraUniforms.ZBufferParams.w = s_perCameraUniforms.ZBufferParams.y / far;

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

    void Pipeline::PushRenderTarget(const RenderTargetPtr& renderTarget)
    {
        s_renderTargetStack.push(renderTarget);
        renderTarget->Attach();
    }

    void Pipeline::PopRenderTarget()
    {
        if (!s_renderTargetStack.empty())
        {
            s_renderTargetStack.top()->Detach();
            s_renderTargetStack.pop();
            glCheckError();
        }
        if (!s_renderTargetStack.empty())
        {
            s_renderTargetStack.top()->Attach();
            glCheckError();
        }
    }

    void RenderTarget::Set(ColorBufferPtr colorBuffer, DepthBufferPtr depthBuffer)
    {
        m_activeColorBufferCount = 1;
        m_colorBuffers[0] = colorBuffer;
        m_depthBuffer = depthBuffer;

        Init();
    }

    void RenderTarget::Set(ColorBufferPtr colorBuffer1, ColorBufferPtr colorBuffer2, ColorBufferPtr colorBuffer3, DepthBufferPtr depthBuffer)
    {
        m_activeColorBufferCount = 3;
        m_colorBuffers[0] = colorBuffer1;
        m_colorBuffers[1] = colorBuffer2;
        m_colorBuffers[2] = colorBuffer3;
        m_depthBuffer = depthBuffer;

        Init();
    }

    void RenderTarget::Attach()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    void RenderTarget::Detach()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::Init()
    {
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        for (int i = 0; i < m_activeColorBufferCount; ++i)
        {
            auto rt = m_colorBuffers[i]->GetNativeTexturePtr();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, rt, 0);
        }
        GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_activeColorBufferCount, attachments);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->GetNativeTexturePtr(), 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glCheckError();
    }

}
