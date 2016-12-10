#include "Material.hpp"

#include <cassert>

#include "Debug.hpp"
#include "Color.hpp"
#include "Shader.hpp"
#include "Vector2.hpp"

namespace FishEngine
{
    std::map<std::string, MaterialPtr> Material::s_builtinMaterialInstance;
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
    constexpr GLenum GLType<Vector2>()
    {
        return GL_FLOAT_VEC2;
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
        for (auto& u : uniforms)
        {
            if (u.name == name && u.type == type)
            {
                return true;
            }
        }
        return false;
    }

    void Material::SetShader(const ShaderPtr& shader)
    {
        m_shader = shader;
        m_uniforms.floats.clear();
        m_uniforms.vec2s.clear();
        m_uniforms.vec3s.clear();
        m_uniforms.vec4s.clear();
        m_uniforms.mat4s.clear();
        m_properties.clear();
        for (auto& u : m_shader->uniforms())
        {
            if (u.type == GL_FLOAT)
            {
                m_uniforms.floats[u.name] = 1.0f;
                m_properties.emplace_back(MaterialProperty{u.name, MaterialPropertyType::Float});
            }
            else if (u.type == GL_FLOAT_VEC2)
            {
                m_uniforms.vec2s[u.name] = Vector2::one;
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::Float2 });
            }
            else if (u.type == GL_FLOAT_VEC3)
            {
                m_uniforms.vec3s[u.name] = Vector3::one;
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::Float3 });
            }
            else if (u.type == GL_FLOAT_VEC4)
            {
                m_uniforms.vec4s[u.name] = Vector4::one;
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::Float4 });
            }
            else if (u.type == GL_FLOAT_MAT4)
            {
                m_uniforms.mat4s[u.name] = Matrix4x4::identity;
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::Mat4 });
            }
            else if (u.type == GL_SAMPLER_2D)
            {
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::Texture2D });
            }
            else if (u.type == GL_SAMPLER_2D_ARRAY)
            {
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::Texture2DArray });
            }
            else if (u.type == GL_SAMPLER_CUBE)
            {
                m_properties.emplace_back(MaterialProperty{ u.name, MaterialPropertyType::TextureCube });
            }
            else {
                Debug::LogError("Unknown shader property type");
            }
        }
    }

    void Material::DisableKeywords(ShaderKeywords keyword)
    {
        m_shader->DisableLocalKeywords(keyword);
    }

    void Material::EnableKeywords(ShaderKeywords keyword)
    {
        auto i = m_shader->m_keywords & keyword;
        if (i == 0)
        {
            //Debug::LogWarning("EnableKeywords");
            m_shader->EnableLocalKeywords(keyword);
            //SetShader(m_shader);
        }
    }

    bool Material::IsKeywordEnabled(ShaderKeyword keyword)
    {
        return m_shader->IsKeywordEnabled(keyword);
    }

    void Material::BindProperties()
    {
        m_shader->BindUniforms(m_uniforms);
        m_shader->BindTextures(m_textures);
    }

    void Material::SetFloat(const std::string& name, const float value)
    {
        if (FindUniformInShader<float>(m_shader->m_uniforms, name))
        {
            m_uniforms.floats[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[float] not found.", name.c_str());
    }


    void Material::SetVector2(const std::string& name, const Vector2& value)
    {
        if (FindUniformInShader<Vector2>(m_shader->m_uniforms, name))
        {
            m_uniforms.vec2s[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[vec2] not found.", name.c_str());
    }

    void Material::SetVector3(const std::string& name, const Vector3& value)
    {
        if (FindUniformInShader<Vector3>(m_shader->m_uniforms, name))
        {
            m_uniforms.vec3s[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[vec3] not found.", name.c_str());
    }


    void Material::SetVector4(const std::string& name, const Vector4& value)
    {
        if (FindUniformInShader<Vector4>(m_shader->m_uniforms, name))
        {
            m_uniforms.vec4s[name] = value;
            return;
        }
        Debug::LogWarning("Uniform %s[vec4] not found.", name.c_str());
    }


    void Material::SetTexture(const std::string& name, TexturePtr texture)
    {
        for (auto& u : m_shader->m_uniforms)
        {
            if (u.name == name && (u.type == GL_SAMPLER_2D || u.type == GL_SAMPLER_CUBE || u.type == GL_SAMPLER_2D_ARRAY))
            {
                m_textures[name] = texture;
                return;
            }
        }
        Debug::LogWarning("Uniform %s[texture] not found.", name.c_str());
    }


    void Material::setMainTexture(TexturePtr texture)
    {
        SetTexture("_MainTex", texture);
    }

    void Material::setColor(const Color& color)
    {
        SetVector4("_Color", Vector4(color.r, color.g, color.b, color.a));
    }

    FishEngine::MaterialPtr Material::builtinMaterial(const std::string& name)
    {
        auto it = s_builtinMaterialInstance.find(name);
        if (it != s_builtinMaterialInstance.end())
        {
            return it->second;
        }
        auto mtl = InstantiateBuiltinMaterial(name);
        s_builtinMaterialInstance[name] = mtl;
        return mtl;
    }

    MaterialPtr Material::InstantiateBuiltinMaterial(const std::string& name)
    {
        Debug::Log("Material::InstantiateBuiltinMaterial, %s", name.c_str());
        auto shader = Shader::builtinShader(name);
        assert(shader != nullptr);
        auto material = CreateMaterial();
        material->setName(name);
        material->SetShader(shader);
        return material;
    }


    void Material::BindTextures(const std::map<std::string, TexturePtr>& textures)
    {
        for (auto& pair : textures)
        {
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
        s_defaultMaterial->setName("DefaultMaterial");
        s_defaultMaterial->SetShader(Shader::builtinShader("PBR"));
        s_defaultMaterial->SetFloat("Metallic", 0);
        s_defaultMaterial->SetFloat("Roughness", 0.5f);
        s_defaultMaterial->SetFloat("Specular", 0.5f);
        s_defaultMaterial->SetVector3("BaseColor", Vector3(1, 1, 1));
    }
}
