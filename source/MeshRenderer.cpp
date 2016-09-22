#include "MeshRenderer.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

FishEngine::MeshRenderer::MeshRenderer(Material::PMaterial material) : Renderer(material)
{

}

NAMESPACE_FISHENGINE_BEGIN

void MeshRenderer::Render() const
{
    //Debug::Log("Rendere %s", m_gameObject->name().c_str());
    auto meshFilter = gameObject()->GetComponent<MeshFilter>();
    if (meshFilter == nullptr) {
        Debug::LogWarning("This GameObject has no MeshFilter");
        return;
    }

    auto model = transform()->localToWorldMatrix();
    auto camera = Scene::mainCamera();
    auto view = camera->worldToCameraMatrix();
    auto proj = camera->projectionMatrix();
    auto mv = view * model;
    auto mvp = proj * mv;

    ShaderUniforms uniforms;
    //std::map<std::string, Matrix4x4> matrices;
    uniforms.mat4s["_Object2World"] = model;
    uniforms.mat4s["MATRIX_V"] = view;
    uniforms.mat4s["MATRIX_P"] = proj;
    uniforms.mat4s["MATRIX_VP"] = proj * view;
    uniforms.mat4s["MATRIX_MVP"] = mvp;
    uniforms.mat4s["MATRIX_MV"] = mv;
    uniforms.mat4s["MATRIX_IT_M"] = model.transpose().inverse();
    uniforms.mat4s["MATRIX_IT_MV"] = mv.transpose().inverse();
    //auto camera = Scene::getMainCamera();
    uniforms.vec3s["_WorldSpaceCameraPos"] = camera->transform()->position();

    Vector4 lightPos(0, 0, 0, 0);
    auto& lights = Light::lights();
    if (lights.size() > 0) {
        auto& l = lights.front();
        if (l->transform() != nullptr) {
            lightPos = Vector4(l->transform()->position(), 0);
        }
    }
    uniforms.vec4s["_WorldSpaceLightPos0"] = lightPos;

    for (auto& m : m_materials) {
        auto shader = m->shader();
        shader->Use();
        shader->PreRender();
        shader->BindUniforms(uniforms);
        m->Update();
        shader->CheckStatus();
        meshFilter->mesh()->Render();
        shader->PostRender();
    }
}

NAMESPACE_FISHENGINE_END
