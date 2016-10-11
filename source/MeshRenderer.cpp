#include "MeshRenderer.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Animator.hpp"
#include "Pipeline.hpp"

FishEngine::MeshRenderer::MeshRenderer(Material::PMaterial material) : Renderer(material)
{

}

NAMESPACE_FISHENGINE_BEGIN

void RecursivelyGetTransformation(
    const std::shared_ptr<Transform>&     transform,
    std::vector<Matrix4x4>&         transformation, 
    std::map<std::string, int>&     nameToIndex,
    std::vector<Matrix4x4>&         bindposes,
    const Matrix4x4& invGlobalTransform)
{
    const auto& name = transform->name();
    const auto& it = nameToIndex.find(name);
    if (it != nameToIndex.end()) {
        const auto boneIndex = it->second;
        transformation[boneIndex] = invGlobalTransform * transform->localToWorldMatrix() * bindposes[boneIndex];
    }
    for (auto& child : transform->children()) {
        RecursivelyGetTransformation(child.lock(), transformation, nameToIndex, bindposes, invGlobalTransform);
    }
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
    if (m_avatar != nullptr) {
        boneTransformation.resize(mesh->m_boneNameToIndex.size());
        RecursivelyGetTransformation(m_rootBone.lock(), boneTransformation, mesh->m_boneNameToIndex, mesh->bindposes(), gameObject()->transform()->worldToLocalMatrix());
    }
    
    for (auto& m : m_materials) {
        auto shader = m->shader();
        shader->Use();
        shader->PreRender();
        if (m_avatar != nullptr)
            shader->BindMatrixArray("BoneTransformations", boneTransformation);
        //m->BindTextures(textures);
        m->Update();
        shader->CheckStatus();
        mesh->Render();
        shader->PostRender();
    }
}

NAMESPACE_FISHENGINE_END
