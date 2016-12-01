#include "RenderSystem.hpp"

#include <boost/lexical_cast.hpp>

#include "Pipeline.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "ModelImporter.hpp"
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

namespace FishEngine
{
    //FishEngine::GBuffer RenderSystem::m_GBuffer;
    FishEngine::ColorBufferPtr      RenderSystem::m_GBuffer[3];
    FishEngine::DepthBufferPtr      RenderSystem::m_depthBuffer;
    FishEngine::RenderTargetPtr     RenderSystem::m_deferredRenderTarget;

    void RenderSystem::Init()
    {
        Pipeline::Init();
        Shader::Init();
        Material::Init();
        Model::Init();
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

        m_depthBuffer = DepthBuffer::Create(w, h);
        m_depthBuffer->setName("DepthBuffer");
        for (int i = 0; i < 3; ++i)
        {
            m_GBuffer[i] = ColorBuffer::Create(w, h);
            m_GBuffer[i]->setName("GBuffer-RT" + boost::lexical_cast<std::string>(i));
        }
        m_deferredRenderTarget = std::make_shared<RenderTarget>();
        m_deferredRenderTarget->Set(m_GBuffer[0], m_GBuffer[1], m_GBuffer[2], m_depthBuffer);
    }

    void RenderSystem::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
        
        if (lights.size() > 0)
        {
            auto& l = lights.front();
            Pipeline::BindLight(l);
        }

        auto v = Camera::main()->viewport();
        const int w = Screen::width();
        const int h = Screen::height();
        glViewport(GLint(v.x*w), GLint(v.y*h), GLsizei(v.z*w), GLsizei(v.w*h));


#if 0
        /************************************************************************/
        /* Skybox                                                               */
        /************************************************************************/
        Matrix4x4 model = Matrix4x4::Scale(100);
        Graphics::DrawMesh(Model::builtinMesh(PrimitiveType::Sphere), model, RenderSettings::skybox());

        /************************************************************************/
        /* Scene                                                                */
        /************************************************************************/
        Scene::Render();
#else
        /************************************************************************/
        /* Scene                                                                */
        /************************************************************************/
        Pipeline::PushRenderTarget(m_deferredRenderTarget);
        glClear(GL_COLOR_BUFFER_BIT);
        std::vector<GameObjectPtr> transparentQueue;
        std::vector<GameObjectPtr> forwardQueue;

        for (auto& go : Scene::m_gameObjects)
        {
            if (!go->activeInHierarchy()) continue;
            RendererPtr renderer = go->GetComponent<MeshRenderer>();

            if (renderer == nullptr)
            {
                renderer = go->GetComponent<SkinnedMeshRenderer>();
                if (renderer == nullptr)
                    continue;
            }

            if (renderer->material() != nullptr)
            {
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
            }
            // Deferred
            renderer->Render();
        }

        Pipeline::PopRenderTarget();

        glDepthFunc(GL_ALWAYS);
        glDepthMask(GL_FALSE);
        auto quad = Model::builtinMesh(PrimitiveType::Quad);
        auto mtl = Material::builtinMaterial("Deferred");
        mtl->SetTexture("DBufferATexture", m_GBuffer[0]);
        mtl->SetTexture("DBufferBTexture", m_GBuffer[1]);
        mtl->SetTexture("DBufferCTexture", m_GBuffer[2]);
        mtl->SetTexture("SceneDepthTexture", m_depthBuffer);
        Graphics::DrawMesh(quad, mtl);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

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
            renderer->Render();
        }
        forwardQueue.clear();


        /************************************************************************/
        /* Skybox                                                               */
        /************************************************************************/
        Matrix4x4 model = Matrix4x4::Scale(100);
        Graphics::DrawMesh(Model::builtinMesh(PrimitiveType::Sphere), model, RenderSettings::skybox());


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
#endif

        //if (m_isWireFrameMode)
        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /************************************************************************/
        /* Gizmos                                                               */
        /************************************************************************/
        //glEnable(GL_POLYGON_OFFSET_LINE);
        //glPolygonOffset(-1.0, -1.0f);
        glDepthFunc(GL_LEQUAL);
        //glDisable(GL_DEPTH_TEST);
        //auto go = Selection::selectedGameObjectInHierarchy();
        //if (go != nullptr)
        //    go->OnDrawGizmosSelected();
        Scene::OnDrawGizmos();
        glDepthFunc(GL_LESS);
        //glEnable(GL_DEPTH_TEST);
        //glDisable(GL_POLYGON_OFFSET_LINE);

        Gizmos::setColor(Color::red);
        auto& b = Scene::m_bounds;
        Gizmos::DrawWireCube(b.center(), b.size());
    }

    void RenderSystem::Clean()
    {

    }

    void RenderSystem::ResizeBufferSize(const int width, const int height)
    {
        m_depthBuffer->Resize(width, height);
        for (auto& gb : m_GBuffer)
            gb->Resize(width, height);
    }

} // namespace FishEngine

