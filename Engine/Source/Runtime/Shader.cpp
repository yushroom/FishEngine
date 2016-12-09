#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <set>
#include <regex>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "Texture.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "Pipeline.hpp"
#include "ShaderCompiler.hpp"

using namespace std;
using namespace FishEngine;

GLuint
CompileShader(
    GLenum             shader_type,
    const std::string& shader_str)
{
    const GLchar* shader_c_str = shader_str.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_c_str, NULL);
    glCompileShader(shader);
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint infoLogLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength + 1);
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog.data());
        throw std::runtime_error(infoLog.data());
    }
    return shader;
};


GLuint
LinkShader(GLuint vs,
    GLuint tcs,
    GLuint tes,
    GLuint gs,
    GLuint fs)
{
    glCheckError();
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    if (gs != 0)
        glAttachShader(program, gs);
    if (tes != 0) {
        if (tcs != 0) glAttachShader(program, tcs);
        glAttachShader(program, tes);
    }
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint infoLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog.data());
        throw std::runtime_error(infoLog.data());
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    if (gs != 0) {
        glDetachShader(program, gs);
    }
    if (tes != 0) {
        if (tcs != 0) glDetachShader(program, tcs);
        glDetachShader(program, tes);
    }

    glCheckError();
    return program;
}

std::string AddLineNumber(const std::string& str)
{
    stringstream ss;
    int line_number = 2;
    ss << "#1\t";
    string::size_type last_pos = 0;
    auto pos = str.find('\n');
    while (pos != string::npos) {
        ss << str.substr(last_pos, pos - last_pos) << "\n#" << line_number << "\t";
        last_pos = pos + 1;
        pos = str.find('\n', last_pos);
        line_number++;
    };
    ss << str.substr(last_pos);
    return ss.str();
};


const char* GLenumToString(GLenum e)
{
    switch (e) {
    case GL_FLOAT:
        return "GL_FLOAT";
    case GL_FLOAT_VEC2:
        return "GL_FLOAT_VEC2";
    case GL_FLOAT_VEC3:
        return "GL_FLOAT_VEC3";
    case GL_FLOAT_VEC4:
        return "GL_FLOAT_VEC4";
    case GL_FLOAT_MAT4:
        return "GL_FLOAT_MAT4";
    case GL_SAMPLER_2D:
        return "GL_SAMPLER_2D";
    case GL_SAMPLER_2D_ARRAY:
        return "GL_SAMPLER_2D_ARRAY";
    case GL_SAMPLER_3D:
        return "GL_SAMPLER_3D";
    case GL_SAMPLER_CUBE:
        return "GL_SAMPLER_CUBE";
    default:
        return "UNKNOWN";
        break;
    }
}


namespace FishEngine
{
    class ShaderImpl
    {
    public:

        ShaderImpl() = default;

        ~ShaderImpl()
        {
            for (auto& e : m_keywordToGLPrograms)
            {
                glDeleteProgram(e.second);
            }
        }

        void set(const std::string& shaderText)
        {
            m_shaderTextRaw = shaderText;
        }

        GLuint CompileAndLink(ShaderKeywords keywords)
        {
            //Debug::LogWarning("CompileAndLink %s", m_filePath.c_str());
            auto vs = Compile(ShaderType::VertexShader, keywords);
            GLuint gs = 0;
            if (m_hasGeometryShader)
                gs = Compile(ShaderType::GeometryShader, keywords);
            auto fs = Compile(ShaderType::FragmentShader, keywords);
            auto glsl_program = LinkShader(vs, 0, 0, gs, fs);
            m_keywordToGLPrograms[keywords] = glsl_program;
            GetAllUniforms(glsl_program);
            glDeleteShader(vs);
            glDeleteShader(fs);
            if (gs != 0) glDeleteShader(gs);
            glCheckError();
            return glsl_program;
        }

        GLuint glslProgram(ShaderKeywords keywords, std::vector<UniformInfo>& uniforms)
        {
            GLuint program = 0;
            auto it = m_keywordToGLPrograms.find(keywords);
            if (it != m_keywordToGLPrograms.end())
            {
                program = it->second;
            }
            else
            {
                program = CompileAndLink(keywords);
            }
            uniforms = m_GLProgramToUniforms[program];
            return program;
        }

        const std::string& shaderTextRaw() const
        {
            return m_shaderTextRaw;
        }

        bool m_hasGeometryShader = false;
        uint32_t m_lineCount;   // for error message

    private:
        //std::string                         m_filePath;
        std::string                         m_shaderTextRaw;
        std::map<ShaderKeywords, GLuint>    m_keywordToGLPrograms;
        std::map<GLuint, std::vector<UniformInfo>>
                                            m_GLProgramToUniforms;

        GLuint Compile(ShaderType type, ShaderKeywords keywords)
        {
            std::string text = "#version 410 core\n";
            m_lineCount = 1;

            auto add_macro_definition = [&text, this](const string& d)
            {
                text += "#define " + d + "\n";
                this->m_lineCount++;
            };

            GLenum t = GL_VERTEX_SHADER;

            if (type == ShaderType::VertexShader)
            {
                add_macro_definition("VERTEX_SHADER");
            }
            else if (type == ShaderType::FragmentShader)
            {
                add_macro_definition("FRAGMENT_SHADER");
                t = GL_FRAGMENT_SHADER;
            }
            else if (type == ShaderType::GeometryShader)
            {
                t = GL_GEOMETRY_SHADER;
                add_macro_definition("GEOMETRY_SHADER");
            }

            if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::Shadow))
            {
                add_macro_definition("_SHADOW");
            }
            if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::SkinnedAnimation))
            {
                add_macro_definition("_SKINNED");
            }
            if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::AmbientIBL))
            {
                add_macro_definition("_AMBIENT_IBL");
            }

            text += m_shaderTextRaw;

            return CompileShader(t, text);
        }

        void GetAllUniforms(GLuint program) noexcept
        {
            std::vector<UniformInfo> uniforms;
            GLuint blockID = glGetUniformBlockIndex(program, "PerCameraUniforms");
            GLint blockSize = 0;
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::PerCameraUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(PerCameraUniforms));
            }

            blockID = glGetUniformBlockIndex(program, "PerDrawUniforms");
            blockSize = 0;
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::PerDrawUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(PerDrawUniforms));
            }

            blockID = glGetUniformBlockIndex(program, "LightingUniforms");
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::LightingUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(LightingUniforms));
            }

            blockID = glGetUniformBlockIndex(program, "Bones");
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::BonesUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(Bones));
            }

            GLint count;
            GLint size; // size of the variable
            GLenum type; // type of the variable (float, vec3 or mat4, etc)
            const GLsizei bufSize = 32; // maximum name length
            GLchar name[bufSize]; // variable name in GLSL
            GLsizei length; // name length

            glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

            for (int i = 0; i < count; i++)
            {
                glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
                GLint loc = glGetUniformLocation(program, name);
                if (loc != GL_INVALID_INDEX)
                {
                    Debug::Log("Uniform #%d Type: %s Name: %s Loc: %d", i, GLenumToString(type), name, loc);
                    uniforms.push_back(UniformInfo{ type, string(name), (GLuint)loc, false });
                }
            }
            m_GLProgramToUniforms[program] = uniforms;
        }
    };

} /* FishEngine */



namespace FishEngine
{

    std::map<std::string, ShaderPtr> Shader::m_builtinShaders;

    Shader::Shader()
    {
        m_impl = std::make_unique<ShaderImpl>();
    }

    ShaderPtr Shader::CreateFromFile(const Path& path)
    {
        Debug::Log("Compiling %s", path.string().c_str());
        auto s = std::make_shared<Shader>();
        if (s->FromFile(path))
            return s;
        return nullptr;
    }
    
    bool Shader::FromFile(const Path& path)
    {
        try
        {
            ShaderCompiler compiler(path);
            std::string parsed_shader_text = compiler.Preprocess();
            std::map<std::string, std::string> settings = compiler.m_settings;
            m_impl->m_hasGeometryShader = compiler.m_hasGeometryShader;
            m_cullface = ToEnum<Cullface>(getValueOrDefault<string, string>(settings, "cull", "back"));
            m_ZWrite = getValueOrDefault<string, string>(settings, "zwrite", "on") == "on";
            m_blend = getValueOrDefault<string, string>(settings, "blend", "off") == "on";
            m_applyNormalMap = getValueOrDefault<string, string>(settings, "normalmap", "off") == "on";
            m_receiveShadow = getValueOrDefault<string, string>(settings, "shadow", "on") == "on";
            m_deferred = getValueOrDefault<string, string>(settings, "deferred", "off") == "on";

            //SetLocalKeywords(ShaderKeyword::Shadow, m_receiveShadow);
            auto k = static_cast<ShaderKeywords>(ShaderKeyword::Shadow);
            if (m_receiveShadow)
            {
                m_keywords |= k;
            }
            else
            {
                m_keywords &= ~k;;
            }

            m_impl->set(parsed_shader_text);
            m_impl->CompileAndLink(m_keywords);
            m_program = m_impl->glslProgram(m_keywords, m_uniforms);
        }
        catch (const std::exception& e)
        {
            //std::cout << AddLineNumber(m_impl->shaderTextRaw()) 
            //    << std::endl << m_impl->m_lineCount << std::endl;
            std::string str = string(e.what());
            auto begin = str.find_first_of('(');
            auto end = str.find_first_of(')');
            uint32_t line_number = boost::lexical_cast<uint32_t>(str.substr(begin + 1, end - begin - 1));
            //cout << line_number << endl;
            uint32_t start_line = m_impl->m_lineCount;
            auto& text = m_impl->shaderTextRaw();

            string::size_type cursor = 0;

            int first = line_number - start_line - 5;
            if (first < 0)
                first = 0;

            for (int i = 0; i < first-1; ++i)
            {
                cursor = text.find_first_of('\n', cursor) + 1;
            }

            auto total = std::count(text.begin(), text.end(), '\n') + 1;
            size_t last = line_number - start_line + 5;
            if (last >= total)
                last = total - 1;

            for (int i = first; i < last; ++i)
            {
                auto new_cursor = text.find_first_of('\n', cursor);
                cout << '#' << i+ start_line << '\t' << text.substr(cursor, new_cursor-cursor) << '\n';
                cursor = new_cursor+1;
            }

            Debug::LogError("%s", e.what());
            return false;
        }
        return true;
    }

    Shader::~Shader()
    {
    }

    void Shader::Use() const
    {
        glUseProgram(m_program);
    }

    void Shader::BindUniformVec4(const char* name, const Vector4& value)
    {
        for (auto& u : m_uniforms)
        {
            //if (boost::starts_with(u.name, name))
            if (u.name == name)
            {
                glUniform4fv(u.location, 1, value.data());
                u.binded = true;
                return;
            }
        }
        Debug::LogWarning("Uniform %s not found!", name);
    }

    void Shader::BindUniformMat4(const char* name, const Matrix4x4& value)
    {
        for (auto& u : m_uniforms) {
            if (boost::starts_with(u.name, name)) {
                glUniformMatrix4fv(u.location, 1, GL_TRUE, value.data());
                u.binded = true;
                return;
            }
        }
        Debug::LogWarning("Uniform %s not found!", name);
    }

    void Shader::BindMatrixArray(const std::string& name, const std::vector<Matrix4x4>& matrixArray)
    {
        //auto loc = GetUniformLocation(name.c_str());
        //glUniformMatrix4fv(loc, matrixArray.size(), GL_TRUE, matrixArray.data()->data());
        for (auto& u : m_uniforms)
        {
            if (boost::starts_with(u.name, name))
            {
                glUniformMatrix4fv(u.location, static_cast<GLsizei>(matrixArray.size()), GL_TRUE, matrixArray.data()->data());
                u.binded = true;
                return;
            }
        }
        Debug::LogWarning("Uniform %s not found!", name.c_str());
    }

    void Shader::BindUniforms(const ShaderUniforms& uniforms)
    {
        for (auto& u : m_uniforms)
        {
            if (u.type == GL_FLOAT_MAT4)
            {
                auto it = uniforms.mat4s.find(u.name);
                if (it != uniforms.mat4s.end())
                {
                    glUniformMatrix4fv(u.location, 1, GL_TRUE, it->second.data());
                    u.binded = true;
                }
                else {
                    Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                }
            }
            else if (u.type == GL_FLOAT_VEC2)
            {
                auto it = uniforms.vec2s.find(u.name);
                if (it != uniforms.vec2s.end())
                {
                    glUniform2fv(u.location, 1, it->second.data());
                    u.binded = true;
                }
                else {
                    Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                }
            }
            else if (u.type == GL_FLOAT_VEC3)
            {
                auto it = uniforms.vec3s.find(u.name);
                if (it != uniforms.vec3s.end())
                {
                    glUniform3fv(u.location, 1, it->second.data());
                    u.binded = true;
                }
                else {
                    Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                }
            }
            else if (u.type == GL_FLOAT)
            {
                auto it = uniforms.floats.find(u.name);
                if (it != uniforms.floats.end())
                {
                    glUniform1f(u.location, it->second);
                    u.binded = true;
                }
            }
            else if (u.type == GL_FLOAT_VEC4)
            {
                auto it = uniforms.vec4s.find(u.name);
                if (it != uniforms.vec4s.end())
                {
                    glUniform4fv(u.location, 1, it->second.data());
                    u.binded = true;
                }
            }
        }
        glCheckError();
    }

    void Shader::BindTextures(const std::map<std::string, TexturePtr>& textures)
    {
        int texture_id = 0;
        for (auto& u : m_uniforms)
        {
            if ( !(u.type == GL_SAMPLER_2D || u.type == GL_SAMPLER_CUBE || u.type == GL_SAMPLER_2D_ARRAY) )
                continue;
            auto it = textures.find(u.name);
            if (it != textures.end())
            {
                glCheckError();
                GLenum type = GL_TEXTURE_2D;
                if (u.type == GL_SAMPLER_CUBE)
                    type = GL_TEXTURE_CUBE_MAP;
                else if (u.type == GL_SAMPLER_2D_ARRAY)
                    type = GL_TEXTURE_2D_ARRAY;
                //BindUniformTexture(u.name.c_str(), it->second->GLTexuture(), texture_id, type);
                glActiveTexture(GLenum(GL_TEXTURE0 + texture_id));
                glCheckError(); 
                glBindTexture(type, it->second->GetNativeTexturePtr());
                //GLuint loc = _getUniformLocation(name);
                glCheckError();
                glUniform1i(u.location, texture_id);
                texture_id++;
                u.binded = true;
                glCheckError();
            }
            else
            {
                Debug::LogWarning("%s of type %s not found", u.name.c_str(), GLenumToString(u.type));
            }
        }
    }

    void Shader::PreRender() const
    {
        if (m_cullface == Cullface::Off)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glCullFace((GLenum)m_cullface);
        }
        glDepthMask(m_ZWrite);
        if (m_blend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }

    void Shader::PostRender() const
    {
        glDepthMask(GL_TRUE);
        if (m_cullface == Cullface::Off)
            glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (m_blend)
            glDisable(GL_BLEND);
    }

    void Shader::CheckStatus() const
    {
        for (auto& u : m_uniforms)
        {
            if (!u.binded)
            {
                Debug::LogWarning("Uniform %s[%s] not binded!", u.name.c_str(), GLenumToString(u.type));
            }
        }
    }

    ShaderPtr Shader::builtinShader(const std::string& name)
    {
        auto it = m_builtinShaders.find(name);
        if (it != m_builtinShaders.end())
        {
            return it->second;
        }
        Debug::LogWarning("No built-in shader called %s", name.c_str());
        abort();
        return nullptr;
    }
    
    void Shader::SetLocalKeywords(ShaderKeyword keyword, bool value)
    {
        auto k = static_cast<ShaderKeywords>(keyword);
        if (value)
        {
            EnableLocalKeywords(k);
        }
        else
        {
            DisableLocalKeywords(k);
        }
    }
    
    void Shader::EnableLocalKeywords(ShaderKeywords keyword)
    {
        m_keywords |= keyword;
        m_program = m_impl->glslProgram(m_keywords, m_uniforms);
    }

    void Shader::DisableLocalKeywords(ShaderKeywords keyword)
    {
        m_keywords &= ~keyword;
        m_program = m_impl->glslProgram(m_keywords, m_uniforms);
    }

    //========== Static Region ==========

    void Shader::Init()
    {
        const auto& root_dir = Resources::shaderRootDirectory();
        for (auto& n : { "Texture", "TextureDoubleSided", "Transparent",
            "PBR", "PBR-Reference", "Diffuse", "DebugCSM"})
        {
            m_builtinShaders[n] = Shader::CreateFromFile(root_dir / (string(n) + ".surf"));
            m_builtinShaders[n]->setName(n);
        }

        for (auto& n : { "Outline", "ScreenTexture", "ShadowMap",
            "SolidColor", "VisualizeNormal", "NormalMap",
            "Deferred", "CascadedShadowMap", "DisplayCSM", "DrawQuad",
            "GatherScreenSpaceShadow", "PostProcessShadow", "PostProcessGaussianBlur" })
        {
            m_builtinShaders[n] = Shader::CreateFromFile(root_dir / (string(n) + ".shader"));
            m_builtinShaders[n]->setName(n);
        }

        m_builtinShaders["SkyboxCubed"]         = Shader::CreateFromFile(root_dir / "Skybox-Cubed.shader");
        m_builtinShaders["SkyboxProcedural"]    = Shader::CreateFromFile(root_dir / "Skybox-Procedural.shader");
        m_builtinShaders["SolidColor-Internal"] = Shader::CreateFromFile(root_dir / "Editor/SolidColor.shader");
        m_builtinShaders["Alpha-Internal"]      = Shader::CreateFromFile(root_dir / "Editor/Alpha.shader");
        m_builtinShaders["VertexLit-Internal"]  = Shader::CreateFromFile(root_dir / "Editor/VertexLit.shader");
        m_builtinShaders["SkyboxCubed"]->setName("SkyboxCubed");
        m_builtinShaders["SkyboxCubed"]->setName("SkyboxCubed");
        m_builtinShaders["SkyboxCubed"]->setName("SkyboxCubed");
        m_builtinShaders["SkyboxCubed"]->setName("SkyboxCubed");
    }
}
