#include "MeshRenderer.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Animator.hpp"
#include "Pipeline.hpp"
#include <cassert>


using namespace FishEngine;

FishEngine::MeshRenderer::MeshRenderer(Material::PMaterial material) : Renderer(material)
{

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

    //std::map<std::string, Texture::PTexture> textures;
    //auto& lights = Light::lights();
    //if (lights.size() > 0) {
    //    auto& l = lights.front();
    //    if (l->transform() != nullptr) {
    //        textures["shadowMap"] = l->m_shadowMap;
    //    }
    //}

    //auto mesh = meshFilter->mesh();
    //auto animator = gameObject()->GetComponent<Animator>();
    std::vector<Matrix4x4> boneTransformation;
    const auto& mesh = meshFilter->mesh();
    
    for (auto& m : m_materials) {
        auto shader = m->shader();
        assert(shader != nullptr);
        shader->Use();
        shader->PreRender();
        //m->BindTextures(textures);
        m->Update();
        shader->CheckStatus();
        mesh->Render();
        shader->PostRender();
    }
}
