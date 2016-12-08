#include "Graphics.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "RenderSettings.hpp"
#include "RenderSystem.hpp"

namespace FishEngine
{
    void Graphics::DrawMesh(const MeshPtr& mesh, const Matrix4x4& matrix, const MaterialPtr& material)
    {
        Pipeline::UpdatePerDrawUniforms(matrix);
        DrawMesh(mesh, material);
    }

    void Graphics::DrawMesh(const MeshPtr& mesh, const MaterialPtr& material)
    {
        //if (material->IsKeywordEnabled(ShaderKeyword::Shadow))
        //{
        //    if (material->shader()->IsDeferred())
        //    {
        //        material->SetTexture("ScreenShadowMap", RenderSystem::m_screenShadowMap);
        //    }
        //    else
        //    {
        //        auto& lights = Light::lights();
        //        if (lights.size() > 0)
        //        {
        //            auto& l = lights.front();
        //            if (l->transform() != nullptr)
        //            {
        //                material->SetTexture("CascadedShadowMap", l->m_shadowMap);
        //            }
        //        }
        //    }
        //}
        if (material->IsKeywordEnabled(ShaderKeyword::AmbientIBL))
        {
            material->SetTexture("AmbientCubemap", RenderSettings::ambientCubemap());
            material->SetTexture("PreIntegratedGF", RenderSettings::preintegratedGF());
        }

        auto shader = material->shader();
        shader->Use();
        shader->PreRender();
        material->BindProperties();
        shader->CheckStatus();
        mesh->Render();
        shader->PostRender();
    }
}

