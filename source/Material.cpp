#include "Material.hpp"
#include <imgui/imgui.h>
#include "Debug.hpp"

NAMESPACE_FISHENGINE_BEGIN

std::map<std::string, Material::PMaterial> Material::m_builtinMaterial;

void FishEngine::Material::SetShader(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
    for (auto& u : m_shader->uniforms()) {
        if (u.type == GL_FLOAT) {
            m_uniforms.floats[u.name] = 0.5f;
        }
        else if (u.type == GL_FLOAT_VEC3) {
            //m_uniforms.vec3s[u.name] = Vector3(1, 1, 1);
        }
    }
}

void FishEngine::Material::Update()
{
    m_shader->BindUniforms(m_uniforms);
    m_shader->BindTextures(m_textures);
}

void Material::OnInspectorGUI() {
    //ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize());
    ImGui::Indent();
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto& uniforms = m_shader->uniforms();
        for (auto& u : uniforms) {
            if (builtinUniformNames.find(u.name) != builtinUniformNames.end()) {
                continue;
            }
            if (u.type == GL_FLOAT) {
                ImGui::SliderFloat(u.name.c_str(), &m_uniforms.floats[u.name], 0, 1);
            } else if (u.type == GL_FLOAT_VEC3) {
                ImGui::InputFloat3(u.name.c_str(), m_uniforms.vec3s[u.name].data());
            }
        }
    }
    ImGui::Unindent();
    //ImGui::PopStyleVar();
}

FishEngine::Material::PMaterial FishEngine::Material::builtinMaterial(const std::string& name)
{
    auto it = m_builtinMaterial.find(name);
    if (it != m_builtinMaterial.end()) {
        return it->second;
    }
    Debug::LogWarning("No built-in material called %d", name.c_str());
    return nullptr;
}

NAMESPACE_FISHENGINE_END

void FishEngine::Material::Init()
{
    for (auto& s : std::vector<std::string>{ "SkyBox", "NormalMap", "VisualizeNormal", "PBR", "VertexLit", "Diffuse", "ShadowMap" , "ScreenTexture"})
    {
        auto material = std::make_shared<Material>();
        material->SetShader(Shader::builtinShader(s));
        m_builtinMaterial[s] = material;
    }
}
