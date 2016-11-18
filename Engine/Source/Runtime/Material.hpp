#ifndef Material_hpp
#define Material_hpp

#include "Object.hpp"
#include "ShaderProperty.hpp"

namespace FishEngine
{
    class Material : public Object
    {
    public:
        Material() = default;

        Material(ShaderPtr shader)
        {
            SetShader(shader);
        }

        Color color() const;

        // The shader used by the material.
        ShaderPtr shader() const
        {
            return m_shader;
        }

        // The shader used by the material.
        void SetShader(ShaderPtr shader);

        void DisableKeyword(ShaderKeyword keyword);

        void EnableKeyword(ShaderKeyword keyword);


        // Set a named float value.
        void SetFloat(const std::string& name, const float value);

        // Set a named Vector3 value.
        void SetVector3(const std::string& name, const Vector3& value);

        void SetVector4(const std::string& name, const Vector4& value);

        // Set a named matrix for the shader.
        void SetMatrix(const std::string& name, const Matrix4x4& value) {
            m_uniforms.mat4s[name] = value;
        }

        // Set a named texture
        void SetTexture(const std::string& name, TexturePtr& texture);

        // The material's texture.
        // The same as using GetTexture or SetTexture with "_MainTex" name.
        void setMainTexture(TexturePtr& texture);

        // The main material's color.
        // The same as using GetColor or SetColor with "_Color" name.
        void setColor(const Color& color);

        void BindTextures(const std::map<std::string, TexturePtr>& textures);

        //    auto uniforms() const {
        //        return m_uniforms;
        //    }

        //    ShaderUniforms& uniforms() {
        //        return m_uniforms;
        //    }

        void Update();

        //void OnInspectorGUI();

        //========== Static Region ==========

        static void Init();

        static MaterialPtr CreateMaterial() {
            return std::make_shared<Material>();
        }

        static MaterialPtr builtinMaterial(const std::string& name);

        static MaterialPtr defaultMaterial();

    private:
        friend class FishEditor::EditorGUI;
        ShaderPtr m_shader = nullptr;
        std::map<std::string, TexturePtr> m_textures;

        ShaderUniforms m_uniforms;

        static std::map<std::string, MaterialPtr> m_builtinMaterial;
        static MaterialPtr s_defaultMaterial;
    };
}


#endif /* Material_hpp */
