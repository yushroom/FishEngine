#ifndef Material_hpp
#define Material_hpp

#include "Object.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Material : public Object
{
public:
    Material() {

    }

    Material(Shader::PShader shader) {
        SetShader(shader);
    }

    void SetShader(std::shared_ptr<Shader> shader);

    typedef std::shared_ptr<Material> PMaterial;

    std::shared_ptr<Shader> shader() const {
        return m_shader;
    }


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
    void SetTexture(const std::string& name, std::shared_ptr<Texture>& texture);
    
    void BindTextures(const std::map<std::string, Texture::PTexture>& textures);

//    auto uniforms() const {
//        return m_uniforms;
//    }
    
//    ShaderUniforms& uniforms() {
//        return m_uniforms;
//    }
    
    void Update(bool skinned = false);
    
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
    std::shared_ptr<Shader> m_shader = nullptr;
    std::map<std::string, Texture::PTexture> m_textures;
    
    ShaderUniforms m_uniforms;

    static std::map<std::string, PMaterial> m_builtinMaterial;
    static PMaterial s_defaultMaterial;
};

NAMESPACE_FISHENGINE_END


#endif /* Material_hpp */
