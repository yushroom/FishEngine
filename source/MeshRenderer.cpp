#include "MeshRenderer.hpp"

#include <cassert>

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Animator.hpp"
#include "Pipeline.hpp"
#include "Camera.hpp"
#include "Gizmos.hpp"


namespace FishEngine
{
    MeshRenderer::MeshRenderer(PMaterial material) : Renderer(material)
    {
        
    }
    
    Bounds MeshRenderer::localBounds() const
    {
        auto mf = gameObject()->GetComponent<MeshFilter>();
        if (mf == nullptr)
            return Bounds();
        return mf->mesh()->bounds();
    }
    
    void MeshRenderer::Render() const
    {
        //Debug::Log("Rendere %s", m_gameObject->name().c_str());
        auto meshFilter = gameObject()->GetComponent<MeshFilter>();
        if (meshFilter == nullptr) {
            Debug::LogWarning("This GameObject has no MeshFilter");
            return;
        }
        
        
        auto model = transform()->localToWorldMatrix();
        auto camera = Camera::main();
        auto mv = Pipeline::perFrameUniformData.MATRIX_V * model;
        
        //ShaderUniforms uniforms;
        Pipeline::perDrawUniformData.MATRIX_MVP = Pipeline::perFrameUniformData.MATRIX_P * mv;
        Pipeline::perDrawUniformData.MATRIX_MV = mv;
        Pipeline::perDrawUniformData.MATRIX_M = model;
        Pipeline::perDrawUniformData.MATRIX_IT_MV = mv.transpose().inverse();
        Pipeline::perDrawUniformData.MATRIX_IT_M = model.transpose().inverse();
        
        Pipeline::BindPerDrawUniforms();
        
        std::map<std::string, PTexture> textures;
        auto& lights = Light::lights();
        if (lights.size() > 0) {
            auto& l = lights.front();
            if (l->transform() != nullptr) {
                textures["shadowMap"] = l->m_shadowMap;
            }
        }
        
        std::vector<Matrix4x4> boneTransformation;
        const auto& mesh = meshFilter->mesh();
        
        for (auto& m : m_materials) {
            auto shader = m->shader();
            assert(shader != nullptr);
            shader->Use();
            shader->PreRender();
            m->BindTextures(textures);
            m->Update();
            shader->CheckStatus();
            mesh->Render();
            shader->PostRender();
        }
    }
    
    void MeshRenderer::OnDrawGizmosSelected()
    {
        Gizmos::setColor(Color::blue);
        auto b = bounds();
        Gizmos::DrawWireCube(b.center(), b.size());
    }
}
