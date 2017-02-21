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

    FishEngine::ColorBufferPtr      RenderSystem::m_blurredScreenShadowMap;
    FishEngine::RenderTargetPtr     RenderSystem::m_blurScreenShadowMapRenderTarget1;
    FishEngine::RenderTargetPtr     RenderSystem::m_blurScreenShadowMapRenderTarget2;


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

        m_blurredScreenShadowMap = ColorBuffer::Create(w, h, TextureFormat::R8);
        m_blurredScreenShadowMap->setFilterMode(FilterMode::Bilinear);
        m_blurScreenShadowMapRenderTarget1 = std::make_shared<RenderTarget>();
        m_blurScreenShadowMapRenderTarget2 = std::make_shared<RenderTarget>();
        m_blurScreenShadowMapRenderTarget1->SetColorBufferOnly(m_blurredScreenShadowMap);
        m_blurScreenShadowMapRenderTarget2->SetColorBufferOnly(m_screenShadowMap);
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
        auto& lights = Light::lights();
        for (auto& l : lights)
        {
            Scene::RenderShadow(l); // note: RenderShadow will change viewport
        }

        auto v = Camera::main()->viewport();
        const int w = Screen::width();
        const int h = Screen::height();
        glViewport(GLint(v.x*w), GLint(v.y*h), GLsizei(v.z*w), GLsizei(v.w*h));

        /************************************************************************/
        /* Scene                                                                */
        /************************************************************************/
        // 3 color buffer: G-BUffer
        // depth buffer
        Pipeline::PushRenderTarget(m_deferredRenderTarget);
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_COLOR, 1, error_color);
        glClearBufferfv(GL_COLOR, 2, error_color);
        glClearBufferfv(GL_DEPTH, 0, white);
        
        std::vector<GameObjectPtr> transparentQueue;
        std::vector<GameObjectPtr> forwardQueue;

        bool deferred_enabled = false;

		std::list<GameObjectPtr> gameObjects;
		for (auto& go : Scene::m_gameObjects)
		{
			gameObjects.push_back(go);
		}
        while ( !gameObjects.empty() )
        {
			auto go = gameObjects.front();
			gameObjects.pop_front();
			for (auto && child : go->transform()->children())
			{
				gameObjects.push_back(child.lock()->gameObject());
			}
			
            if (!go->activeInHierarchy()) continue;
            RendererPtr renderer = go->GetComponent<MeshRenderer>();

            if (renderer == nullptr)
            {
                renderer = go->GetComponent<SkinnedMeshRenderer>();
            }
			if (renderer == nullptr || !renderer->enabled())
				continue;
			
			if (renderer->material() == nullptr)
			{
				// TODO
				// draw error material;
				continue;
			}

			if (renderer->material()->shader()->IsTransparent())
			{
				transparentQueue.push_back(go);
				continue;
			}
			else if (!renderer->material()->shader()->IsDeferred())
			{
				forwardQueue.push_back(go);
				continue;
			}
			
            // Deferred
            deferred_enabled = true;
            renderer->Render();
        }

        Pipeline::PopRenderTarget();

        // 1 color buffer
        // depth buffer
        Pipeline::PushRenderTarget(m_mainRenderTarget);
        glClearBufferfv(GL_COLOR, 0, error_color);
        //glClearBufferfv(GL_DEPTH, 0, white);

        /************************************************************************/
        /* Deferred Rendering                                                   */
        /************************************************************************/
        if (deferred_enabled)
        {
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
        }

        /************************************************************************/
        /* Forward                                                              */
        /************************************************************************/
        for (auto& go : forwardQueue)
        {
            RendererPtr renderer = go->GetComponent<MeshRenderer>();
            if (renderer == nullptr)
            {
                renderer = go->GetComponent<SkinnedMeshRenderer>();
            }
			if (renderer->enabled())
				renderer->Render();
        }
        forwardQueue.clear();

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
            auto light = Light::lights().front();
            glClearBufferfv(GL_COLOR, 0, white);
            auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
            auto mtl = Material::builtinMaterial("GatherScreenSpaceShadow");
            mtl->SetTexture("CascadedShadowMap", light->m_shadowMap);
            mtl->SetTexture("SceneDepthTexture", m_mainDepthBuffer);
            Graphics::DrawMesh(quad, mtl);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }
        Pipeline::PopRenderTarget();

        // blur shadow map, pass 1
        m_screenShadowMap->setFilterMode(FilterMode::Bilinear);
        Pipeline::PushRenderTarget(m_blurScreenShadowMapRenderTarget1);
        {
            glDepthFunc(GL_ALWAYS);
            glDepthMask(GL_FALSE);
            glClearBufferfv(GL_COLOR, 0, black);
            auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
            auto mtl = Material::builtinMaterial("PostProcessGaussianBlur");
            Vector2 direction(1.0f / static_cast<float>(m_screenShadowMap->width()), 0);
            mtl->SetVector2("Direction", direction);
            mtl->setMainTexture(m_screenShadowMap);
            Graphics::DrawMesh(quad, mtl);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }
        Pipeline::PopRenderTarget();
        m_screenShadowMap->setFilterMode(FilterMode::Point);

        // blur shadow map, pass 2
        Pipeline::PushRenderTarget(m_blurScreenShadowMapRenderTarget2);
        {
            glDepthFunc(GL_ALWAYS);
            glDepthMask(GL_FALSE);
            glClearBufferfv(GL_COLOR, 0, black);
            auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
            auto mtl = Material::builtinMaterial("PostProcessGaussianBlur");
            Vector2 direction(0, 1.0f / static_cast<float>(m_screenShadowMap->height()));
            mtl->SetVector2("Direction", direction);
            mtl->setMainTexture(m_blurredScreenShadowMap);
            Graphics::DrawMesh(quad, mtl);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }
        Pipeline::PopRenderTarget();

        // add shadow
        {
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
        }
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
        for (auto& go : transparentQueue)
        {
            RendererPtr renderer = go->GetComponent<MeshRenderer>();
            if (renderer == nullptr)
            {
                renderer = go->GetComponent<SkinnedMeshRenderer>();
            }
            renderer->Render();
        }
        transparentQueue.clear();

#if 0
        glDepthFunc(GL_ALWAYS);
        auto display_csm_mtl = Material::builtinMaterial("DisplayCSM");
        constexpr float size = 0.25f;
        auto quad = Model::builtinMesh(PrimitiveType::ScreenAlignedQuad);
        for (int i = 0; i < 4; ++i)
        {
            display_csm_mtl->SetFloat("Section", float(i));
            Vector4 v(i*size*2-1, -1, size, size);
            display_csm_mtl->SetVector4("DrawRectParameters", v);
            display_csm_mtl->setMainTexture(lights.front()->m_shadowMap);
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
        m_blurredScreenShadowMap->Resize(width, height);
    }

} // namespace FishEngine

