#include "RenderSystem.hpp"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "ModelImporter.hpp"
#include "Gizmos.hpp"
#include "RenderSettings.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Screen.hpp"

namespace FishEngine
{
    void RenderSystem::Init()
    {
        Pipeline::Init();
        Shader::Init();
        Material::Init();
        Model::Init();
        Gizmos::Init();
        //EditorGUI::Init();
        Scene::Init();
        //#ifdef GLM_FORCE_LEFT_HANDED
        glFrontFace(GL_CW);
        //#endif
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        //glEnable(GL_LINE_SMOOTH);
    }

    void RenderSystem::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto camera = Camera::main();
        auto proj = camera->projectionMatrix();
        auto view = camera->worldToCameraMatrix();
        Pipeline::perFrameUniformData.MATRIX_P = proj;
        Pipeline::perFrameUniformData.MATRIX_V = view;
        Pipeline::perFrameUniformData.MATRIX_I_V = view.inverse();
        Pipeline::perFrameUniformData.MATRIX_VP = proj * view;
        Pipeline::perFrameUniformData.WorldSpaceCameraPos = Camera::main()->transform()->position();
        float t = Time::time();
        Pipeline::perFrameUniformData._Time = Vector4(t / 20.f, t, t*2.f, t*3.f);

        Vector4 lightDir(0, 0, 0, 0);
        Matrix4x4 lightVP;
        auto& lights = Light::lights();
        if (lights.size() > 0)
        {
            auto& l = lights.front();
            if (l->transform() != nullptr)
            {
                lightDir = Vector4(-l->transform()->forward(), 0);
                lightVP = l->m_projectMatrixForShadowMap * l->m_viewMatrixForShadowMap;
            }
            Pipeline::perFrameUniformData.LightColor0 = l->m_color;
        }
        Pipeline::perFrameUniformData.WorldSpaceLightPos0 = lightDir;
        Pipeline::perFrameUniformData.LightMatrix0 = lightVP;

        Pipeline::BindPerFrameUniforms();


        /************************************************************************/
        /* Skybox                                                               */
        /************************************************************************/
        Matrix4x4 model = Matrix4x4::Scale(100);
        auto mv = Pipeline::perFrameUniformData.MATRIX_V * model;
        Pipeline::perDrawUniformData.MATRIX_MVP = Pipeline::perFrameUniformData.MATRIX_VP * model;
        Pipeline::perDrawUniformData.MATRIX_MV = mv;
        Pipeline::perDrawUniformData.MATRIX_M = model;
        Pipeline::perDrawUniformData.MATRIX_IT_MV = mv.transpose().inverse();
        Pipeline::perDrawUniformData.MATRIX_IT_M = model.transpose().inverse();
        Pipeline::BindPerDrawUniforms();
        static auto sphere = Model::builtinMesh(PrimitiveType::Sphere);
        auto skybox_material = RenderSettings::skybox();
        auto shader = skybox_material->shader();
        shader->Use();
        //glCullFace(GL_BACK);
        //glDisable(GL_DEPTH_TEST);
        shader->PreRender();
        skybox_material->Update();
        shader->CheckStatus();
        sphere->Render();
        shader->PostRender();
        //glEnable(GL_DEPTH_TEST);
        //glCullFace(GL_BACK);


        /************************************************************************/
        /* Shadow                                                               */
        /************************************************************************/
        for (auto& l : lights) {
            Scene::RenderShadow(l);
        }

        /************************************************************************/
        /* Scene                                                                */
        /************************************************************************/
        //const int w = Screen::width();
        //const int h = Screen::height();
        //auto v = Camera::main()->viewport();
        //auto scene_view_pos_size = EditorGUI::sceneViewPositionAndSize();
        //glViewport(GLint(scene_view_pos_size.x), GLint(scene_view_pos_size.y), GLsizei(scene_view_pos_size.z), GLsizei(scene_view_pos_size.w));
        auto v = Camera::main()->viewport();
        const int w = Screen::width();
        const int h = Screen::height();
        glViewport(GLint(v.x*w), GLint(v.y*h), GLsizei(v.z*w), GLsizei(v.w*h));
        Scene::Render();

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

} // namespace FishEngine

