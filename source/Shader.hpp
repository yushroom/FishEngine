#ifndef Shader_hpp
#define Shader_hpp

#include "GLEnvironment.hpp"
#include <set>
#include "Vector3.hpp"
#include "Matrix4x4.hpp"

#define PositionIndex 0
#define NormalIndex 1
#define UVIndex 2
#define TangentIndex 3

NAMESPACE_FISHENGINE_BEGIN

// Unity Built-in shader variables
// http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
static std::set<std::string> builtinUniformNames{
"MATRIX_MVP", "MATRIX_V", "MATRIX_P", "MATRIX_VP", "MATRIX_IT_MV", "MATRIX_IT_M"
    "_Object2World", "_WorldSpaceCameraPos"
};

struct ShaderUniforms
{
    std::map<std::string, Matrix4x4> mat4s;
    std::map<std::string, Vector3> vec3s;
    std::map<std::string, Vector4> vec4s;
    std::map<std::string, float> floats;
};

struct UniformInfo {
    //GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)
    //GLchar name[32];
    std::string name;  // variable name in GLSL
    GLuint location;
    bool binded;
};

class RenderSystem;
class Texture;

class Shader
{
public:
    Shader() = default;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&);

    typedef std::shared_ptr<Shader> PShader;

    static void Init();

    static PShader CreateFromString(const std::string& vs_str, const std::string& fs_str);
    
    static PShader CreateFromString(const std::string& vs_str, const std::string& fs_str, const std::string& gs_str);
    
    static PShader CreateFromFile(const std::string& vs_path, const std::string& fs_path);
    
    static PShader CreateFromFile(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path);

    void FromString(const std::string& vs_str, const std::string& fs_str);
    void FromString(const std::string& vs_str, const std::string& fs_str, const std::string& gs_str);
    void FromString(const std::string& vs_str,
                    const std::string& tcs_str,
                    const std::string& tes_str,
                    const std::string& gs_str,
                    const std::string& fs_str);
    void FromFile(const std::string& vs_path, const std::string& fs_path);
    void FromFile(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path);
    //Shader(const std::string& vs_path, const std::string ps_path);
    ~Shader();
    
    void Use() const;
    
    GLuint program() const {
        return m_program;
    }
    
    //GLuint getAttribLocation(const char* name) const;
    
    //void BindUniformFloat(const char* name, const float value) const;
    //void BindUniformVec3(const char* name, const glm::vec3& value) const;
    //void BindUniformMat3(const char* name, const glm::mat3& value) const;
    //void BindUniformMat4(const char* name, const glm::mat4& value) const;

    //void BindUniformTexture(const char* name, const GLuint texture, const GLuint id, GLenum textureType = GL_TEXTURE_2D) const;

    void BindUniforms(const ShaderUniforms& uniforms);
    void BindTextures(const std::map<std::string, std::shared_ptr<Texture>>& textures);
    
    void PreRender() const;
    void PostRender() const;
    
    void CheckStatus() const;
    
    const std::vector<UniformInfo>& uniforms() const {
        return m_uniforms;
    }
    
    std::vector<UniformInfo>& uniforms() {
        return m_uniforms;
    }

    static PShader builtinShader(const std::string& name);
    
private:
    GLuint m_program = 0;
    
    GLint GetUniformLocation(const char* name) const;
    

    std::vector<UniformInfo> m_uniforms;
    
    enum class Cullface {
        Back = GL_BACK,
        Front = GL_FRONT,
        Off = GL_FRONT_AND_BACK,
    };
    
    Cullface m_cullface = Cullface::Back;
    bool m_ZWrite = true;
    
    friend class RenderSystem;
    
    static std::string m_shaderVariables;
    static std::map<std::string, PShader> m_builtinShaders;
};

NAMESPACE_FISHENGINE_END

#endif

