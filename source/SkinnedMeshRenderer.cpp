#include "SkinnedMeshRenderer.hpp"

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

namespace FishEngine
{
    SkinnedMeshRenderer::
        SkinnedMeshRenderer(Material::PMaterial material)
        : Renderer(material)
    {

    }


    Bounds SkinnedMeshRenderer::
        localBounds() const {
        return m_sharedMesh->bounds();
    }


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


    void SkinnedMeshRenderer::Update()
    {
        m_boneTransformation.resize(m_sharedMesh->m_boneNameToIndex.size());
        RecursivelyGetTransformation(m_rootBone.lock(), m_boneTransformation, m_sharedMesh->m_boneNameToIndex, m_sharedMesh->bindposes(), gameObject()->transform()->worldToLocalMatrix());
    }


    void SkinnedMeshRenderer::Render() const
    {
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

        std::map<std::string, Texture::PTexture> textures;
        auto& lights = Light::lights();
        if (lights.size() > 0) {
            auto& l = lights.front();
            if (l->transform() != nullptr) {
                textures["shadowMap"] = l->m_shadowMap;
            }
        }

        // hack
        // TODO: remove this block
        bool skinned = m_avatar != nullptr;
        if (skinned && m_boneTransformation.size() == 0) {
            m_boneTransformation.resize(m_sharedMesh->m_boneNameToIndex.size());
            RecursivelyGetTransformation(m_rootBone.lock(), m_boneTransformation, m_sharedMesh->m_boneNameToIndex, m_sharedMesh->bindposes(), gameObject()->transform()->worldToLocalMatrix());
        }

        for (auto& m : m_materials) {
            auto shader = m->shader();
            if (skinned) {
                shader = shader->m_skinnedShader;
            }
            assert(shader != nullptr);
            shader->Use();
            shader->PreRender();
            if (m_avatar != nullptr)
                shader->BindMatrixArray("BoneTransformations", m_boneTransformation);
            m->BindTextures(textures);
            m->Update(skinned);
            shader->CheckStatus();
            m_sharedMesh->Render();
            shader->PostRender();
        }
    }
}
