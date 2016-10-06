#include "SkinnedMeshRenderer.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include <imgui/imgui.h>

namespace FishEngine {
    SkinnedMeshRenderer::SkinnedMeshRenderer(Material::PMaterial material) : Renderer(material)
    {
        
    }
    
    void SkinnedMeshRenderer::OnInspectorGUI()
    {
        int boneCount = m_avatar->m_boneToIndex.size();
        ImGui::InputInt("Bone Count", &boneCount);
    }

    void SkinnedMeshRenderer::Render() const
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
            uniforms.vec4s["_LightColor0"] = l->m_color;
        }
        uniforms.vec4s["_WorldSpaceLightPos0"] = lightDir;
        uniforms.mat4s["_LightMatrix0"] = lightVP;
        
        for (auto& m : m_materials) {
            auto shader = m->shader();
            shader->Use();
            shader->PreRender();
            shader->BindUniforms(uniforms);
            m->BindTextures(textures);
            //shader->BindTextures(textures);
            m->Update();
            shader->CheckStatus();
            meshFilter->mesh()->Render();
            shader->PostRender();
        }
    }
}


