#include "Material.hpp"

#include <cassert>

#include "Debug.hpp"
#include "Color.hpp"
#include "Shader.hpp"

namespace FishEngine
{
    std::map<std::string, MaterialPtr> Material::m_builtinMaterial;
    MaterialPtr Material::s_defaultMaterial = nullptr;

    // https://www.opengl.org/sdk/docs/man/html/glGetActiveUniform.xhtml
    template<typename T>
    constexpr GLenum GLType();
    //{
    //    static_assert(false, "Not implemented");
    //}

    template<>
    constexpr GLenum GLType<float>()
    {
        return GL_FLOAT;
    }

    template<>
    constexpr GLenum GLType<Vector3>()
    {
        return GL_FLOAT_VEC3;
    }

    template<>
    constexpr GLenum GLType<Vector4>()
    {
        return GL_FLOAT_VEC4;
    }

    template<>
    constexpr GLenum GLType<Matrix4x4>()
    {
        return GL_FLOAT_MAT4;
    }

    template<typename T>
    bool FindUniformInShader(std::vector<UniformInfo>& uniforms, const std::string& name)
    {
        constexpr auto type = GLType<T>();
        for (auto& u : uniforms) {
            if (u.name == name && u.type == type) {
                return true;
            }
        }
        return false;
    }

    void Material::SetShader(ShaderPtr shader)
    {
        m_shader = shader;
        //for (auto& u : m_shader->uniforms()) {
        //    if (u.type == GL_FLOAT) {
        //        m_uniforms.floats[u.name] = 0.5f;
        //    }
        //    else if (u.type == GL_FLOAT_VEC3) {
        //        //m_uniforms.vec3s[u.name] = Vector3(1, 1, 1);
        //    }
        //}
    }

    void Material::DisableKeyword(ShaderKeyword keyword)
    {
        m_shader->DisableLocalKeyword(keyword);
    }

    void Material::EnableKeyword(ShaderKeyword keyword)
    {
        m_shader->EnableLocalKeyword(keyword);
    }

    void Material::Update()
    {
        m_shader->BindUniforms(m_uniforms);
        m_shader->BindTextures(m_textures);
    }

    void Material::SetFloat(const std::string& name, const float value)
    {
        if (FindUniformInShader<float>(m_shader->m_uniforms, name)) {
            m_uniforms.floats[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[float] not found.", name.c_str());
    }


    void Material::SetVector3(const std::string& name, const Vector3& value)
    {
        //m_shader->BindUniformVec3(name.c_str(), value);
        if (FindUniformInShader<Vector3>(m_shader->m_uniforms, name)) {
            m_uniforms.vec3s[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[vec3] not found.", name.c_str());
    }


    void Material::SetVector4(const std::string& name, const Vector4& value)
    {
        if (FindUniformInShader<Vector4>(m_shader->m_uniforms, name)) {
            m_uniforms.vec4s[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[vec4] not found.", name.c_str());
    }


    void Material::SetTexture(const std::string& name, TexturePtr& texture)
    {
        for (auto& u : m_shader->m_uniforms)
        {
            if (u.name == name && (u.type == GL_SAMPLER_2D || u.type == GL_SAMPLER_CUBE))
            {
                m_textures[name] = texture;
                return;
            }
        }
        Debug::LogWarning("Uniform %s[sampler2D] not found.", name.c_str());
    }


    void Material::setMainTexture(TexturePtr& texture)
    {
        SetTexture("_MainTex", texture);
    }

    void Material::setColor(const Color& color)
    {
        SetVector4("_Color", Vector4(color.r, color.g, color.b, color.a));
    }

    MaterialPtr Material::builtinMaterial(const std::string& name)
    {
        auto shader = Shader::builtinShader(name);
        assert(shader != nullptr);
        auto material = CreateMaterial();
        material->setName(name);
        material->SetShader(shader);
        return material;
    }


    void Material::BindTextures(const std::map<std::string, TexturePtr>& textures)
    {
        //m_textures = textures;
        for (auto& pair : textures) {
            m_textures[pair.first] = pair.second;
        }
    }


    MaterialPtr Material::defaultMaterial()
    {
        return s_defaultMaterial;
    }


    void Material::Init()
    {
        s_defaultMaterial = CreateMaterial();
        s_defaultMaterial->name() = "DefaultMaterial";
        s_defaultMaterial->SetShader(Shader::builtinShader("PBR"));
        s_defaultMaterial->SetFloat("Metallic", 0);
        s_defaultMaterial->SetFloat("Roughness", 0.5f);
        s_defaultMaterial->SetFloat("Specular", 0.5f);
        s_defaultMaterial->SetVector3("BaseColor", Vector3(1, 1, 1));
    }
}
