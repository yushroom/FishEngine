#include "MeshRenderer.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Animator.hpp"

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
    auto view = camera->worldToCameraMatrix();
    auto proj = camera->projectionMatrix();
    auto mv = view * model;
    auto mvp = proj * mv;

    ShaderUniforms uniforms;
    //std::map<std::string, Matrix4x4> matrices;
    uniforms.mat4s["MATRIX_M"] = model;
    uniforms.mat4s["MATRIX_V"] = view;
    uniforms.mat4s["MATRIX_P"] = proj;
    uniforms.mat4s["MATRIX_VP"] = proj * view;
    uniforms.mat4s["MATRIX_MVP"] = mvp;
    uniforms.mat4s["MATRIX_MV"] = mv;
    uniforms.mat4s["MATRIX_IT_M"] = model.transpose().inverse();
    uniforms.mat4s["MATRIX_IT_MV"] = mv.transpose().inverse();
    //auto camera = Scene::getMainCamera();
    uniforms.vec3s["WorldSpaceCameraPos"] = camera->transform()->position();

    Vector4 lightDir(0, 0, 0, 0);
    std::map<std::string, Texture::PTexture> textures;
    Matrix4x4 lightVP;
    auto& lights = Light::lights();
    if (lights.size() > 0) {
        auto& l = lights.front();
        if (l->transform() != nullptr) {
            lightDir = Vector4(l->transform()->forward(), 0); 
            auto view = l->gameObject()->transform()->worldToLocalMatrix();
            auto proj = Matrix4x4::Ortho(-10.f, 10.f, -10.f, 10.f, l->shadowNearPlane(), 100.f);
            lightVP = proj * view;
            textures["shadowMap"] = l->m_shadowMap;
        }
        uniforms.vec4s["LightColor0"] = l->m_color;
    }
    uniforms.vec4s["WorldSpaceLightPos0"] = lightDir;
    uniforms.mat4s["LightMatrix0"] = lightVP;

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
        shader->BindUniforms(uniforms);
        if (m_avatar != nullptr)
            shader->BindMatrixArray("BoneTransformations", boneTransformation);
        m->BindTextures(textures);
        //shader->BindTextures(textures);
        m->Update();
        shader->CheckStatus();
        mesh->Render();
        shader->PostRender();
    }
}

NAMESPACE_FISHENGINE_END
