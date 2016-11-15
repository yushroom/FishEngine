#ifndef Shader_hpp
#define Shader_hpp

#include <set>
#include "GLEnvironment.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "Common.hpp"

#define PositionIndex 0
#define NormalIndex 1
#define UVIndex 2
#define TangentIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

namespace FishEngine
{

    // Unity Built-in shader variables
    // http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
    //static std::set<std::string> builtinUniformNames{
    //"MATRIX_MVP", "MATRIX_V", "MATRIX_P", "MATRIX_VP", "MATRIX_IT_MV", "MATRIX_IT_M"
    //    "_Object2World", "_WorldSpaceCameraPos"
    //};

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

    // https://docs.unity3d.com/Manual/SL-CullAndDepth.html
    enum class Cullface {
        Back = GL_BACK,
        Front = GL_FRONT,
        Off = GL_NONE,
    };

    inline const char* ToString(Cullface e)
    {
        switch (e) {
        case Cullface::Back: return "Back"; break;
        case Cullface::Front: return "Front"; break;
        case Cullface::Off: return "Off"; break;
        default: abort(); break;
        }
    }


    template<>
    inline Cullface ToEnum<Cullface>(const std::string& s)
    {
        if (s == "Back") return Cullface::Back;
        if (s == "Front") return Cullface::Front;
        if (s == "Off") return Cullface::Off;
        abort();
    }

    enum class ZWrite {
        On,
        Off,
    };

    inline const char* ToString(ZWrite e)
    {
        switch (e) {
        case ZWrite::On: return "On"; break;
        case ZWrite::Off: return "Off"; break;
        default: abort(); break;
        }
    }

    template<>
    inline ZWrite ToEnum<ZWrite>(const std::string& s)
    {
        if (s == "On") return ZWrite::On;
        if (s == "Off") return ZWrite::Off;
        abort();
    }

    enum class ZTest {
        Less = GL_LESS,
        Greater = GL_GREATER,
        LEqual = GL_LEQUAL,
        GEqual = GL_GEQUAL,
        Equal = GL_EQUAL,
        NotEqual = GL_NOTEQUAL,
        Always = GL_ALWAYS,
    };

    inline const char* ToString(ZTest e)
    {
        switch (e) {
        case ZTest::Less: return "Less"; break;
        case ZTest::Greater: return "Greater"; break;
        case ZTest::LEqual: return "LEqual"; break;
        case ZTest::GEqual: return "GEqual"; break;
        case ZTest::Equal: return "Equal"; break;
        case ZTest::NotEqual: return "NotEqual"; break;
        case ZTest::Always: return "Always"; break;
        default: abort(); break;
        }
    }

    template<>
    inline ZTest ToEnum<ZTest>(const std::string& s)
    {
        if (s == "Less") return ZTest::Less;
        if (s == "Greater") return ZTest::Greater;
        if (s == "LEqual") return ZTest::LEqual;
        if (s == "GEqual") return ZTest::GEqual;
        if (s == "Equal") return ZTest::Equal;
        if (s == "NotEqual") return ZTest::NotEqual;
        if (s == "Always") return ZTest::Always;
        abort();
    }
    
//    enum class ShaderType
//    {
//        VertexShader,
//        FragmentShader,
//        GeometryShader,
//        VertexAndFragmentShader,
//    };

    enum class ShaderFeature
    {
        NormalMap,
        Shadow,
    };

    class Shader
    {
    public:
        Shader() = default;
        Shader(const Shader&) = delete;
        //Shader& operator=(const Shader&) = delete;
        Shader(Shader&&);

        ~Shader();

        static void Init();
        
        static PShader CreateFromString(const std::string& vs_str, const std::string& fs_str);

        static PShader CreateFromString(const std::string& vs_str, const std::string& fs_str, const std::string& gs_str);

        static PShader CreateFromFile(const std::string& path);
        
        static PShader CreateFromFile(const std::string& vs_path, const std::string& fs_path);

        static PShader CreateFromFile(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path);

        void FromString(const std::string& vsfs_str);
        void FromSurfaceShaderString(const std::string& surfaceShaderString);
        void FromString(const std::string& vs_str, const std::string& fs_str);
        void FromString(const std::string& vs_str, const std::string& fs_str, const std::string& gs_str);
        void FromString(
            const std::string& vs_str,
            const std::string& tcs_str,
            const std::string& tes_str,
            const std::string& gs_str,
            const std::string& fs_str);
        // vsfs or surface
        void FromFile(const std::string& path);
        void FromFile(const std::string& vs_path, const std::string& fs_path);
        void FromFile(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path);
        //Shader(const std::string& vs_path, const std::string ps_path);

        void Use() const;

        GLuint program() const
        {
            return m_program;
        }

        //GLuint getAttribLocation(const char* name) const;

        //void BindUniformFloat(const char* name, const float value) const;
        //void BindUniformVec3(const char* name, const glm::vec3& value) const;
        //void BindUniformMat3(const char* name, const glm::mat3& value) const;
        void BindUniformVec4(const char* name, const Vector4& value);
        void BindUniformMat4(const char* name, const Matrix4x4& value);

        //void BindUniformTexture(const char* name, const GLuint texture, const GLuint id, GLenum textureType = GL_TEXTURE_2D) const;

        void BindMatrixArray(const std::string& name, const std::vector<Matrix4x4>& matrixArray);
        void BindUniforms(const ShaderUniforms& uniforms);

        void BindTexture(const std::string& name, PTexture& texture);
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
        
        static const std::map<std::string, PShader>& allShaders()
        {
            // TODO
            return m_builtinShaders;
        }
        
        static GLuint LoadShader(GLenum shaderType, const std::string& filePath);

        //static GLuint LoadShaderCombined(const std::string& filePath);
        //static GLuint LoadShaderSurface(const std::string& filePath);
        
        bool IsTransparent() const {
            return m_blend;
        }

        std::shared_ptr<Shader> m_skinnedShader = nullptr;

        void Enable(ShaderFeature feature)
        {
            SetFeature(feature, true);
        }

        void Disable(ShaderFeature feature)
        {
            SetFeature(feature, false);
        }

        void Compile();

    private:
        friend class Material;

        Shader& operator=(const Shader&) = default;

        GLuint m_program = 0;
        std::string m_shaderString;

        void GetAllUniforms();
        GLint GetUniformLocation(const char* name) const;

        std::vector<UniformInfo> m_uniforms;

        Cullface m_cullface = Cullface::Back;
        bool m_ZWrite = true;
        bool m_blend = false;
        bool m_applyNormalMap = false;
        bool m_receiveShadow = true;

        friend class RenderSystem;

        //static std::string m_shaderVariables;
        static std::map<std::string, PShader> m_builtinShaders;

        //static std::map<std::string, std::string> m_fileToShaderString;

        void SetFeature(ShaderFeature feature, bool enabled)
        {
            if (feature == ShaderFeature::NormalMap)
                m_applyNormalMap = enabled;
            else if (feature == ShaderFeature::Shadow)
                m_receiveShadow = enabled;
        }
    };
}

#endif

