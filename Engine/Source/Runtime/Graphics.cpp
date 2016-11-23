#include "Graphics.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

namespace FishEngine
{
    void Graphics::DrawMesh(const MeshPtr& mesh, const Matrix4x4& matrix, const MaterialPtr& material)
    {
        Pipeline::UpdatePerDrawUniforms(matrix);
        DrawMesh(mesh, material);
    }

    void Graphics::DrawMesh(const MeshPtr& mesh, const MaterialPtr& material)
    {
        if (material->IsKeywordEnabled(ShaderKeyword::Shadow))
        {
            auto& lights = Light::lights();
            if (lights.size() > 0)
            {
                auto& l = lights.front();
                if (l->transform() != nullptr)
                {
                    material->SetTexture("ShadowMap", l->m_shadowMap);
                }
            }
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

