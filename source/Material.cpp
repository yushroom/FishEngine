#include "Material.hpp"
#include <imgui/imgui.h>
#include "Debug.hpp"

NAMESPACE_FISHENGINE_BEGIN

std::map<std::string, Material::PMaterial> Material::m_builtinMaterial;
Material::PMaterial Material::s_defaultMaterial = nullptr;

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

void FishEngine::Material::Update(bool skinned /* = false*/)
{
    if (skinned) {
        m_shader->m_skinnedShader->BindUniforms(m_uniforms);
        m_shader->m_skinnedShader->BindTextures(m_textures);
    } else {
        m_shader->BindUniforms(m_uniforms);
        m_shader->BindTextures(m_textures);
    }
}

void Material::OnInspectorGUI() {
    //ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize());
    ImGui::Indent();
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto& uniforms = m_shader->uniforms();
        for (auto& u : uniforms) {
            //if (builtinUniformNames.find(u.name) != builtinUniformNames.end()) {
            //    continue;
            //}
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
    auto shader = Shader::builtinShader(name);
    assert(shader!=nullptr);
    auto material = CreateMaterial();
    material->SetShader(shader);
    return material;
}

Material::PMaterial Material::defaultMaterial()
{
    return s_defaultMaterial;
}

void FishEngine::Material::Init()
{
    s_defaultMaterial = CreateMaterial();
    s_defaultMaterial->name() = "DefaultMaterial";
    s_defaultMaterial->SetShader(Shader::builtinShader("PBR"));
    s_defaultMaterial->SetFloat("metallic", 0.5f);
    s_defaultMaterial->SetFloat("roughness", 0.5f);
    s_defaultMaterial->SetVector3("albedo", Vector3(1, 1, 1));
}

NAMESPACE_FISHENGINE_END
