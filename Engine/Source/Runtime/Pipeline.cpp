#include "Pipeline.hpp"
#include "GLEnvironment.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Light.hpp"
#include "Screen.hpp"
#include "RenderTexture.hpp"
#include "RenderTarget.hpp"

#include <cassert>

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
		assert(s_perCameraUBO > 0);
		glGenBuffers(1, &s_perDrawUBO);
		glGenBuffers(1, &s_lightingUBO);
		glGenBuffers(1, &s_bonesUBO);
	}

	void Pipeline::BindCamera(const CameraPtr& camera)
	{
		auto const & proj = camera->projectionMatrix();
		auto const & view = camera->worldToCameraMatrix();
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

		s_perCameraUniforms.ScreenParams.x = static_cast<float>(Screen::width());
		s_perCameraUniforms.ScreenParams.y = static_cast<float>(Screen::height());
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
		s_lightingUniforms.CascadesNear = light->m_cascadesNear;
		s_lightingUniforms.CascadesFar = light->m_cascadesFar;
		s_lightingUniforms.CascadesSplitPlaneNear = light->m_cascadesSplitPlaneNear;
		s_lightingUniforms.CascadesSplitPlaneFar = light->m_cascadesSplitPlaneFar;
		s_lightingUniforms.fish_LightShadowBias.x = light->m_shadowBias;
		s_lightingUniforms.fish_LightShadowBias.y = 1;
		s_lightingUniforms.fish_LightShadowBias.z = light->m_shadowNormalBias;
		s_lightingUniforms.fish_LightShadowBias.w = 0;
		for (int i = 0; i < 4; ++i)
		{
			s_lightingUniforms.LightMatrix[i] = light->m_projectMatrixForShadowMap[i] * light->m_viewMatrixForShadowMap[i];
			// macOS bug
			s_lightingUniforms.LightMatrix[i] = s_lightingUniforms.LightMatrix[i].transpose();
			//s_lightingUniforms.LightMatrix[i] = Matrix4x4::identity;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, s_lightingUBO);
		//auto size = sizeof(perFrameUniformData);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(s_lightingUniforms), (void*)&s_lightingUniforms, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, LightingUBOBindingPoint, s_lightingUBO);
		glCheckError();
	}

	void Pipeline::UpdatePerDrawUniforms(const Matrix4x4& modelMatrix)
	{
		glCheckError();
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

}
