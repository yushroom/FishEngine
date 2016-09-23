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
    void SetFloat(const std::string& name, const float value) {
        //m_shader->BindUniformFloat(name.c_str(), value);
        m_uniforms.floats[name] = value;
    }
    
    // Set a named Vector3 value.
    void SetVector3(const std::string& name, const Vector3& value) {
        //m_shader->BindUniformVec3(name.c_str(), value);
        m_uniforms.vec3s[name] = value;
    }

    void SetVector4(const std::string& name, const Vector4& value) {
        m_uniforms.vec4s[name] = value;
    }
    
    // Set a named matrix for the shader.
    void SetMatrix(const std::string& name, const Matrix4x4& value) {
        m_uniforms.mat4s[name] = value;
    }
    
    void BindTextures(const std::map<std::string, Texture::PTexture>& textures) {
        //m_textures = textures;
        for (auto& pair : textures) {
            m_textures[pair.first] = pair.second;
        }
    }

//    auto uniforms() const {
//        return m_uniforms;
//    }
    
//    ShaderUniforms& uniforms() {
//        return m_uniforms;
//    }
    
    void Update();
    
    void OnInspectorGUI();
    
    //========== Static Region ==========
    
    static void Init();
    
    static PMaterial CreateMaterial() {
        return std::make_shared<Material>();
    }
    
    static PMaterial builtinMaterial(const std::string& name);

private:
    std::shared_ptr<Shader> m_shader = nullptr;
    std::map<std::string, Texture::PTexture> m_textures;
    
    ShaderUniforms m_uniforms;

    static std::map<std::string, PMaterial> m_builtinMaterial;
};

NAMESPACE_FISHENGINE_END


#endif /* Material_hpp */
