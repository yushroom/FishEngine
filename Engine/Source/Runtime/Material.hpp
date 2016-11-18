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
        void SetTexture(const std::string& name, PTexture& texture);

        // The material's texture.
        // The same as using GetTexture or SetTexture with "_MainTex" name.
        void setMainTexture(PTexture& texture);

        // The main material's color.
        // The same as using GetColor or SetColor with "_Color" name.
        void setColor(const Color& color);

        void BindTextures(const std::map<std::string, PTexture>& textures);

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

        static PMaterial CreateMaterial() {
            return std::make_shared<Material>();
        }

        static PMaterial builtinMaterial(const std::string& name);

        static PMaterial defaultMaterial();

    private:
        friend class FishEditor::EditorGUI;
        ShaderPtr m_shader = nullptr;
        std::map<std::string, PTexture> m_textures;

        ShaderUniforms m_uniforms;

        static std::map<std::string, PMaterial> m_builtinMaterial;
        static PMaterial s_defaultMaterial;
    };
}


#endif /* Material_hpp */
