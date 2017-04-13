#include "RenderSystem.hpp"

#include <boost/lexical_cast.hpp>

#include "Pipeline.hpp"
#include "Shader.hpp"
#include "Material.hpp"
//#include "ModelImporter.hpp"
#include "Mesh.hpp"
#include "Gizmos.hpp"
#include "RenderSettings.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Graphics.hpp"
#include "GameObject.hpp"
#include "MeshRenderer.hpp"
#include "SkinnedMeshRenderer.hpp"
#include "RenderTarget.hpp"
#include "Timer.hpp"
#include "MeshFilter.hpp"

using namespace FishEngine;

struct RenderObject
{
	int				renderQueue;
	RendererPtr		renderer;
	MaterialPtr		material;
	MeshPtr			mesh;
	int				subMeshID = -1;

	RenderObject(int renderQueue, RendererPtr renderer, MaterialPtr material, MeshPtr mesh, int subMeshID = -1)
		: renderer(renderer), renderQueue(renderQueue), material(material), mesh(mesh), subMeshID(subMeshID)
	{

	}
};

namespace FishEngine
{
	//FishEngine::GBuffer RenderSystem::m_GBuffer;
	FishEngine::ColorBufferPtr      RenderSystem::m_GBuffer[3];
	FishEngine::DepthBufferPtr      RenderSystem::m_mainDepthBuffer;
	FishEngine::RenderTargetPtr     RenderSystem::m_deferredRenderTarget;

	FishEngine::ColorBufferPtr      RenderSystem::m_screenShadowMap;
	FishEngine::RenderTargetPtr     RenderSystem::m_screenShadowMapRenderTarget;

	FishEngine::ColorBufferPtr      RenderSystem::m_mainColorBuffer;
	FishEngine::RenderTargetPtr     RenderSystem::m_mainRenderTarget;

	FishEngine::RenderTargetPtr     RenderSystem::m_colorOnlyRenderTarget;

	//FishEngine::ColorBufferPtr      RenderSystem::m_blurredScreenShadowMap;
	//FishEngine::RenderTargetPtr     RenderSystem::m_blurScreenShadowMapRenderTarget1;
	//FishEngine::RenderTargetPtr     RenderSystem::m_blurScreenShadowMapRenderTarget2;


	void RenderSystem::InitializeGL()
	{
#if FISHENGINE_PLATFORM_WINDOWS
		glewExperimental = GL_TRUE;
		// Initialize GLEW to setup the OpenGL Function pointers
		auto err = glewInit();
		if (err != GLEW_OK)
		{
			//LogInfo(std::string(glewGetErrorString(err)));
			LogError("GLEW not initialized");
			abort();
		}
#endif
	}

	void RenderSystem::Init()
	{
		TextureSampler::Init();
		Pipeline::Init();
		//Shader::Init();
		Material::Init();
		//Mesh::Init();
		Gizmos::Init();
		Scene::Init();
		glFrontFace(GL_CW);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		//glEnable(GL_LINE_SMOOTH);

		const int w = Screen::width();
		const int h = Screen::height();

		m_mainDepthBuffer = DepthBuffer::Create(w, h);
		m_mainDepthBuffer->setName("MainDepthBuffer");
		for (int i = 0; i < 3; ++i)
		{
			m_GBuffer[i] = ColorBuffer::Create(w, h);
			m_GBuffer[i]->setName("GBuffer-RT" + boost::lexical_cast<std::string>(i));
		}
		m_deferredRenderTarget = std::make_shared<RenderTarget>();
		m_deferredRenderTarget->Set(m_GBuffer[0], m_GBuffer[1], m_GBuffer[2], m_mainDepthBuffer);

		m_screenShadowMap = ColorBuffer::Create(w, h, TextureFormat::R8);
		m_screenShadowMap->setName("ScreenShadowMap");
		m_screenShadowMapRenderTarget = std::make_shared<RenderTarget>();
		m_screenShadowMapRenderTarget->SetColorBufferOnly(m_screenShadowMap);

		m_mainColorBuffer = ColorBuffer::Create(w, h);
		m_mainColorBuffer->setName("MainColorBuffer");
		m_mainRenderTarget = std::make_shared<RenderTarget>();
		m_mainRenderTarget->Set(m_mainColorBuffer, m_mainDepthBuffer);

		m_colorOnlyRenderTarget = std::make_shared<RenderTarget>();
		m_colorOnlyRenderTarget->SetColorBufferOnly(m_mainColorBuffer);

		//m_blurredScreenShadowMap = ColorBuffer::Create(w, h, TextureFormat::R8);
		//m_blurredScreenShadowMap->setFilterMode(FilterMode::Bilinear);
		//m_blurScreenShadowMapRenderTarget1 = std::make_shared<RenderTarget>();
		//m_blurScreenShadowMapRenderTarget2 = std::make_shared<RenderTarget>();
		//m_blurScreenShadowMapRenderTarget1->SetColorBufferOnly(m_blurredScreenShadowMap);
		//m_blurScreenShadowMapRenderTarget2->SetColorBufferOnly(m_screenShadowMap);
	}

	void RenderSystem::Render()
	{
		glCheckError();
		float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float error_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, error_color);
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		auto camera = Camera::main();
		Pipeline::BindCamera(camera);


		/************************************************************************/
		/* Shadow                                                               */
		/************************************************************************/
//        auto& lights = Light::lights();
//        for (auto& l : lights)
//        {
//            Scene::RenderShadow(l.lock()); // note: RenderShadow will change viewport
//        }

		Scene::RenderShadow(Light::mainLight());

		auto v = Camera::main()->viewport();
		const int w = Screen::width();
		const int h = Screen::height();
		glViewport(GLint(v.x*w), GLint(v.y*h), GLsizei(v.z*w), GLsizei(v.w*h));

		/************************************************************************/
		/* Scene                                                                */
		/************************************************************************/

		// forward
		std::list<RenderObject> forwardRenderQueueGeometry;
		std::list<RenderObject> forwardRenderQueueTransparent;
		
		// deferred
		std::list<RenderObject> deferredRenderQueue;	// for now, geometry only

		bool deferred_enabled = false;

		std::deque<GameObjectPtr> todo;
		for (auto& go : Scene::m_gameObjects)
		{
			todo.push_back(go);
		}
		while (!todo.empty())
		{
			auto go = todo.front();
			todo.pop_front();
			for (auto && child : go->transform()->children())
			{
				todo.push_back(child->gameObject());
			}

			if (!go->activeInHierarchy())
				continue;
			RendererPtr renderer = go->GetComponent<Renderer>();
			if (renderer == nullptr || !renderer->enabled())
				continue;

			MeshPtr mesh;
			if (renderer->ClassID() == ClassID<MeshRenderer>())
			{
				auto meshFilter = go->GetComponent<MeshFilter>();
				if (meshFilter == nullptr)
					continue;
				mesh = meshFilter->mesh();
			}
			else
			{
				mesh = As<SkinnedMeshRenderer>(renderer)->sharedMesh();
			}

			if (mesh == nullptr)
				continue;

			auto & materials = renderer->materials();
			for (int i = 0; i < materials.size(); ++i)
			{
				auto & material = materials[i];
				if (material == nullptr)
				{
					continue;
				}

				// TODO: find correct renderqueue and submeshID

				if (material->shader()->IsTransparent())
				{
					forwardRenderQueueTransparent.emplace_back(0, renderer, material, mesh, i);
					continue;
				}
				else if (material->shader()->IsDeferred())
				{
					// Deferred
					deferred_enabled = true;
					deferredRenderQueue.emplace_back(0, renderer, material, mesh, i);
					continue;
				}
				else
				{
					forwardRenderQueueGeometry.emplace_back(0, renderer, material, mesh, i);
				}
				
			}
		}

		// 1 color buffer
		// depth buffer
		Pipeline::PushRenderTarget(m_mainRenderTarget);
		glClearBufferfv(GL_COLOR, 0, error_color);
		glClearBufferfv(GL_DEPTH, 0, white);

		/************************************************************************/
		/* Deferred Rendering                                                   */
		/************************************************************************/
		if (deferred_enabled)
		{
			// 3 color buffer: G-BUffer
			// depth buffer
			Pipeline::PushRenderTarget(m_deferredRenderTarget);
			glClearBufferfv(GL_COLOR, 0, black);
			glClearBufferfv(GL_COLOR, 1, error_color);
			glClearBufferfv(GL_COLOR, 2, error_color);
			glClearBufferfv(GL_DEPTH, 0, white);

			for (auto & ro : deferredRenderQueue)
			{
				ro.renderer->PreRender();
				Graphics::DrawMesh(ro.mesh, ro.material, ro.subMeshID);
			}

			Pipeline::PopRenderTarget();

			Pipeline::PushRenderTarget(m_colorOnlyRenderTarget);
			glDepthFunc(GL_ALWAYS);
			glDepthMask(GL_FALSE);
			auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
			auto mtl = Material::builtinMaterial("Deferred");
			mtl->SetTexture("DBufferATexture", m_GBuffer[0]);
			mtl->SetTexture("DBufferBTexture", m_GBuffer[1]);
			mtl->SetTexture("DBufferCTexture", m_GBuffer[2]);
			mtl->SetTexture("SceneDepthTexture", m_mainDepthBuffer);
			Graphics::DrawMesh(quad, mtl);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
			Pipeline::PopRenderTarget();
		}

		/************************************************************************/
		/* Forward                                                              */
		/************************************************************************/
		for (auto & ro : forwardRenderQueueGeometry)
		{
			if (ro.mesh->name() == "hair_front")
				LogWarning("here");
			ro.renderer->PreRender();
			Graphics::DrawMesh(ro.mesh, ro.material, ro.subMeshID);
		}

		Pipeline::PopRenderTarget(); // m_mainRenderTarget

#if 1
		/************************************************************************/
		/* Screen Space Shadow                                                  */
		/************************************************************************/
		// 1 color buffer
		// no depth buffer
		Pipeline::PushRenderTarget(m_screenShadowMapRenderTarget);
		{
			glDepthFunc(GL_ALWAYS);
			glDepthMask(GL_FALSE);
			auto light = Light::mainLight();
			LayeredDepthBufferPtr shadowMap;
			if (light != nullptr)
			{
				shadowMap = light->m_shadowMap;
			}
			else
			{
				shadowMap = RenderSettings::defaultShadowMap();
			}
			glClearBufferfv(GL_COLOR, 0, white);
			auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
			auto mtl = Material::builtinMaterial("GatherScreenSpaceShadow");
			mtl->SetTexture("CascadedShadowMap", shadowMap);
			float shadowMapSize = static_cast<float>( shadowMap->width() );
			float shadowMapTexelSize = 1.0f / shadowMapSize;
			mtl->SetVector4("_ShadowMapTexture_TexelSize", Vector4(shadowMapTexelSize, shadowMapTexelSize, shadowMapSize, shadowMapSize));
			mtl->SetTexture("SceneDepthTexture", m_mainDepthBuffer);
			Graphics::DrawMesh(quad, mtl);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
		}
		Pipeline::PopRenderTarget();
		
		// add shadow
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_FALSE);
		//Pipeline::PushRenderTarget(m_addShadowRenderTarget);
		//glClearBufferfv(GL_COLOR, 0, black);
		auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
		auto mtl = Material::builtinMaterial("PostProcessShadow");
		mtl->setMainTexture(m_mainColorBuffer);
		mtl->SetTexture("ScreenShadow", m_screenShadowMap);
		Graphics::DrawMesh(quad, mtl);
		//Pipeline::PopRenderTarget();
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
#else
		m_mainRenderTarget->AttachForRead();
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif

		//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		//m_mainRenderTarget->Attach();
		//auto w = m_mainDepthBuffer->width();
		//auto h = m_mainDepthBuffer->height();
		m_mainRenderTarget->AttachForRead();
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		//m_mainRenderTarget->Detach();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		/************************************************************************/
		/* Skybox                                                               */
		/************************************************************************/
		Matrix4x4 model;
		model.SetTRS(Camera::main()->transform()->position(), Quaternion::identity, Vector3::one * 2000);
		//Matrix4x4 model = Matrix4x4::Scale(1000);
		Graphics::DrawMesh(Mesh::builtinMesh(PrimitiveType::Sphere), model, RenderSettings::skybox());


		/************************************************************************/
		/* Transparent                                                          */
		/************************************************************************/
		for (auto & ro : forwardRenderQueueTransparent)
		{
			ro.renderer->PreRender();
			Graphics::DrawMesh(ro.mesh, ro.material, ro.subMeshID);
		}

#if 0
		glDepthFunc(GL_ALWAYS);
		auto display_csm_mtl = Material::builtinMaterial("DisplayCSM");
		constexpr float size = 0.25f;
		auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
		for (int i = 0; i < 4; ++i)
		{
			display_csm_mtl->SetFloat("Section", float(i));
			Vector4 v(i*size * 2 - 1, -1, size, size);
			display_csm_mtl->SetVector4("DrawRectParameters", v);
			display_csm_mtl->setMainTexture(Light::mainLight()->m_shadowMap);
			Graphics::DrawMesh(quad, display_csm_mtl);
		}
		glDepthFunc(GL_LESS);
#endif

		//if (m_isWireFrameMode)
		//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		///************************************************************************/
		///* Gizmos                                                               */
		///************************************************************************/
		//glDepthFunc(GL_LEQUAL);
		//Scene::OnDrawGizmos();
		//glDepthFunc(GL_LESS);

		//Gizmos::setColor(Color::red);
		//auto& b = Scene::m_bounds;
		//Gizmos::DrawWireCube(b.center(), b.size());
	}

	void RenderSystem::Clean()
	{

	}

	void RenderSystem::ResizeBufferSize(const int width, const int height)
	{
		m_mainDepthBuffer->Resize(width, height);
		m_mainColorBuffer->Resize(width, height);
		for (auto& gb : m_GBuffer)
			gb->Resize(width, height);
		m_screenShadowMap->Resize(width, height);
		//m_blurredScreenShadowMap->Resize(width, height);
	}

} // namespace FishEngine

