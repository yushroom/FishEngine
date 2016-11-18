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
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/grammars/cpp_grammar.hpp>
#include <boost/wave/grammars/cpp_defined_grammar.hpp>

#include "Texture.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "Pipeline.hpp"
#include "Resources.hpp"

using namespace std;
using namespace FishEngine;

typedef boost::wave::cpplexer::lex_iterator<
        boost::wave::cpplexer::lex_token<> >
    lex_iterator_type;
typedef boost::wave::context<
        std::string::iterator,
        lex_iterator_type>
    context_type;

#ifdef CACHE_INCLUDE_HEADER
std::map<std::string, std::string> pathToShaderString;
#endif

std::string 
ReadFile(
    const std::string& path)
{
#ifdef CACHE_INCLUDE_HEADER
    auto it = pathToShaderString.find(path);
    if (it != pathToShaderString.end())
    {
        return it->second;
    }
#endif
    std::ifstream fin(path);
    if (!fin.is_open()) {
        FishEngine::Debug::LogError("Can not open shader file: %s", path.c_str());
        throw exception();
    }

#ifdef CACHE_INCLUDE_HEADER
    const auto& str = std::string(
        std::istreambuf_iterator<char>(fin.rdbuf()),
        std::istreambuf_iterator<char>());
    pathToShaderString[path] = str;
    return str;
#else 
    return std::string(
        std::istreambuf_iterator<char>(fin.rdbuf()),
        std::istreambuf_iterator<char>());
#endif
}

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


const char* GLenumToString(GLenum e)
{
    switch (e) {
    case GL_FLOAT:
        return "GL_FLOAT";
    case GL_FLOAT_VEC3:
        return "GL_FLOAT_VEC3";
    case GL_FLOAT_MAT4:
        return "GL_FLOAT_MAT4";
    case GL_SAMPLER_2D:
        return "GL_SAMPLER_2D";
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
    enum class ShaderFileType
    {
        Surface,
        Combined,
    };

    enum class ShaderType
    {
        VertexShader,
        FragmentShader,
        GeometryShader,
    };

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

        void set(std::string shaderText, ShaderFileType type, std::string filePath = "placeholder.surf")
        {
            m_shaderTextRaw = shaderText + "\n";
            m_filePath = filePath;
            m_shaderTextRaw = DecorateShaderText(type, m_shaderTextRaw);
        }

        GLuint CompileAndLink(ShaderKeywords keywords)
        {
            Debug::Log("CompileAndLink %s", m_filePath.c_str());
            auto vs = Compile(ShaderType::VertexShader, keywords);
            GLuint gs = 0;
            if (getValueOrDefault<string, string>(m_settings, "GeometryShader", "Off") == "On")
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

        std::map<std::string, std::string>  m_settings;

    private:
        std::string                         m_filePath;
        std::string                         m_shaderTextRaw;
        std::map<ShaderKeywords, GLuint>    m_keywordToGLPrograms;
        std::map<GLuint, std::vector<UniformInfo>>
                                            m_GLProgramToUniforms;

        GLuint Compile(ShaderType type, ShaderKeywords keywords)
        {
            std::string text = Preprocess(type, keywords);
            GLenum t = GL_VERTEX_SHADER;
            if (type == ShaderType::FragmentShader)
                t = GL_FRAGMENT_SHADER;
            else if (type == ShaderType::GeometryShader)
                t = GL_GEOMETRY_SHADER;
            return CompileShader(t, "#version 410 core\n" + text);
        }

        std::string Preprocess(ShaderType type, ShaderKeywords keywords)
        {
            const auto& include_dir = Resources::shaderRootDirectory() + "include/";
            //auto text = DecorateShaderText(type, m_shaderTextRaw);
            auto& text = m_shaderTextRaw;
            context_type ctx(text.begin(), text.end(), m_filePath.c_str());
            ctx.set_language(boost::wave::language_support::support_normal);
            ctx.add_sysinclude_path(include_dir.c_str());

            if (type == ShaderType::VertexShader)
            {
                ctx.add_macro_definition("VERTEX_SHADER");
            }
            else if (type == ShaderType::FragmentShader)
            {
                ctx.add_macro_definition("FRAGMENT_SHADER");
            }
            else if (type == ShaderType::GeometryShader)
            {
                ctx.add_macro_definition("GEOMETRY_SHADER");
            }

            if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::Shadow))
            {
                ctx.add_macro_definition("_SHADOW");
            }
            if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::SkinnedAnimation))
            {
                ctx.add_macro_definition("_SKINNED");
            }
            if (keywords & static_cast<ShaderKeywords>(ShaderKeyword::AmbientIBL))
            {
                ctx.add_macro_definition("_AMBIENT_IBL");
            }

            auto first = ctx.begin();
            auto last = ctx.end();
            std::string parsed_text;

            parsed_text.reserve(text.size());

            int intent_level = 0;
            string last_tok = "";
            bool next_tok_is_uniform_var_type = false;
            bool next_tok_is_uniform_var_name = false;

            bool next_tok_is_pragma_type = false;
            bool next_tok_is_pragma_setting = false;
            string pragma_type = "";
            string pragma_setting = "";

            while (first != last)
            {
                //std::cout << first->get_value();
                string tok = first->get_value().c_str();

                auto id = boost::wave::token_id(*first);
                if (id == boost::wave::T_SPACE)
                {
                    goto SKIP;
                }

                if (tok == "}")
                {
                    intent_level--;
                }
                if (last_tok == "\n" && tok != "#line")
                {
                    assert(intent_level >= 0);
                    for (int i = 0; i < intent_level; ++i)
                    {
                        parsed_text += "\t";
                    }
                }
                if (tok == "{")
                {
                    intent_level++;
                }

                if (tok == "#pragma")
                {
                    //cout << tok << endl;
                    next_tok_is_pragma_type = true;
                }
                else if (next_tok_is_pragma_type)
                {
                    if (id == boost::wave::T_IDENTIFIER)
                    {
                        pragma_type = tok;
                        next_tok_is_pragma_setting = true;
                    }
                    next_tok_is_pragma_type = false;
                }
                else if (next_tok_is_pragma_setting)
                {
                    if (id == boost::wave::T_IDENTIFIER)
                    {
                        pragma_setting = tok;
                        m_settings[pragma_type] = pragma_setting;
                        cout << "\tsettings: " << pragma_type << " " << pragma_setting << endl;
                    }
                    next_tok_is_pragma_setting = false;
                }

                if (tok == "InternalUniform")
                {
                    tok = "uniform";
                }
                else if (tok == "uniform")
                {
                    next_tok_is_uniform_var_type = true;
                }
                else if (next_tok_is_uniform_var_type)
                {
                    //if (uniform_types.find(tok) != uniform_types.end())
                    //{
                    //cout << tok << " ";
                    next_tok_is_uniform_var_name = true;
                    //}
                    next_tok_is_uniform_var_type = false;
                }
                else if (next_tok_is_uniform_var_name)
                {
                    cout << tok << endl;
                    next_tok_is_uniform_var_name = false;
                }

            SKIP:
                parsed_text += tok;
                last_tok = tok;
                ++first;
            }

            return parsed_text;
        }

        static std::string DecorateShaderText(ShaderFileType type, std::string& shaderText)
        {
            std::string left = "", right = "\n";

            switch (type)
            {
            case FishEngine::ShaderFileType::Surface:
                left = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n";
                right = "#endif\n";
                break;
            }

            return left + "#line 1\n" + shaderText + right;
        }

        void GetAllUniforms(GLuint program)
        {
            std::vector<UniformInfo> uniforms;
            GLuint blockID = glGetUniformBlockIndex(program, "PerDraw");
            GLint blockSize = 0;
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::PerDrawUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(Pipeline::s_perDrawUniformData));
            }

            blockID = glGetUniformBlockIndex(program, "PerFrame");
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::PerFrameUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(Pipeline::s_perFrameUniformData));
            }

            //    blockID = glGetUniformBlockIndex(m_program, "Bones");
            //    //assert(blockID != GL_INVALID_INDEX);
            //    if (blockID != GL_INVALID_INDEX)
            //    {
            //        glUniformBlockBinding(m_program, blockID, Pipeline::BonesUBOBindingPoint);
            //        glGetActiveUniformBlockiv(m_program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
            //        assert(blockSize == sizeof(Pipeline::bonesUniformData));
            //    }

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
                //GLint location = glGetUniformLocation(m_program, name);
                GLint loc = glGetUniformLocation(program, name);
                //Debug::Log("Uniform #%d Type: %s Name: %s Loc: %d", i, GLenumToString(type).c_str(), name, loc);
                if (loc != GL_INVALID_INDEX)
                    uniforms.push_back(UniformInfo{ type, string(name), (GLuint)loc, false });
            }
            m_GLProgramToUniforms[program] = uniforms;
        }
    };

} /* FishEngine */



namespace FishEngine {

    std::map<std::string, ShaderPtr> Shader::m_builtinShaders;

    Shader::Shader()
    {
        m_impl = std::make_unique<ShaderImpl>();
    }

    ShaderPtr Shader::CreateFromFile(const std::string& path)
    {
        Debug::Log("Compiling %s", path.c_str());
        auto s = std::make_shared<Shader>();
        if (s->FromFile(path))
            return s;
        return nullptr;
    }
    
    bool Shader::FromFile(const std::string& path)
    {
        try
        {
            auto ext = getExtensionWithoutDot(path);
            auto shader_text = ReadFile(path);
            auto t = ext == "surf" ? ShaderFileType::Surface : ShaderFileType::Combined;
            m_impl->set(shader_text, t, path);
            m_impl->CompileAndLink(static_cast<ShaderKeywords>(ShaderKeyword::All));
            m_program = m_impl->glslProgram(m_keywords, m_uniforms);
            m_cullface = ToEnum<Cullface>(getValueOrDefault<string, string>(m_impl->m_settings, "Cull", "Back"));
            m_ZWrite = getValueOrDefault<string, string>(m_impl->m_settings, "ZWrite", "On") == "On";
            m_blend = getValueOrDefault<string, string>(m_impl->m_settings, "Blend", "Off") == "On";
            m_applyNormalMap = getValueOrDefault<string, string>(m_impl->m_settings, "Normalmap", "Off") == "On";
            m_receiveShadow = getValueOrDefault<string, string>(m_impl->m_settings, "Shadow", "On") == "On";
        }
        catch (const boost::wave::preprocess_exception& e)
        {
            printf("%s(%lu) : %s\n", e.file_name(), e.line_no(), e.description());
            return false;
        }
        catch (const boost::wave::cpplexer::lexing_exception& e)
        {
            printf("%s(%lu) : %s\n", e.file_name(), e.line_no(), e.description());
            return false;
        }
        catch (const std::exception& e)
        {
            printf("%s", e.what());
            return false;
        }

        //GetAllUniforms();
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
        for (auto& u : m_uniforms) {
            if (boost::starts_with(u.name, name)) {
                glUniformMatrix4fv(u.location, matrixArray.size(), GL_TRUE, matrixArray.data()->data());
                u.binded = true;
                return;
            }
        }
        Debug::LogWarning("Uniform %s not found!", name.c_str());
    }

    void Shader::BindUniforms(const ShaderUniforms& uniforms)
    {
        for (auto& u : m_uniforms) {
            if (u.type == GL_FLOAT_MAT4) {
                auto it = uniforms.mat4s.find(u.name);
                if (it != uniforms.mat4s.end()) {
                    glUniformMatrix4fv(u.location, 1, GL_TRUE, it->second.data());
                    u.binded = true;
                }
                //            else {
                //                Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                //            }
            }
            else if (u.type == GL_FLOAT_VEC3) {
                auto it = uniforms.vec3s.find(u.name);
                if (it != uniforms.vec3s.end()) {
                    glUniform3fv(u.location, 1, it->second.data());
                    u.binded = true;
                }
                //            else {
                //                Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                //            }
            }
            else if (u.type == GL_FLOAT) {
                auto it = uniforms.floats.find(u.name);
                if (it != uniforms.floats.end()) {
                    glUniform1f(u.location, it->second);
                    u.binded = true;
                }
            }
            else if (u.type == GL_FLOAT_VEC4) {
                auto it = uniforms.vec4s.find(u.name);
                if (it != uniforms.vec4s.end()) {
                    glUniform4fv(u.location, 1, it->second.data());
                    u.binded = true;
                }
            }
        }
    }

    void Shader::BindTextures(const std::map<std::string, TexturePtr>& textures)
    {
        int texture_id = 0;
        for (auto& u : m_uniforms) {
            if (u.type != GL_SAMPLER_2D && u.type != GL_SAMPLER_CUBE) continue;
            auto it = textures.find(u.name);
            if (it != textures.end()) {
                GLenum type = u.type == GL_SAMPLER_2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
                //BindUniformTexture(u.name.c_str(), it->second->GLTexuture(), texture_id, type);
                glActiveTexture(GLenum(GL_TEXTURE0 + texture_id));
                glBindTexture(type, it->second->GLTexuture());
                //GLuint loc = _getUniformLocation(name);
                glUniform1i(u.location, texture_id);
                texture_id++;
                u.binded = true;
            }
            else {
                Debug::LogWarning("%s of type %s not found", u.name.c_str(), GLenumToString(u.type));
            }
        }
    }

    void Shader::PreRender() const
    {
        if (m_cullface == Cullface::Off) {
            glDisable(GL_CULL_FACE);
        }
        else {
            glCullFace((GLenum)m_cullface);
        }
        glDepthMask(m_ZWrite);
        if (m_blend) {
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
        for (auto& u : m_uniforms) {
            if (!u.binded) {
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
        for (auto& n : { "Texture", "TextureDoubleSided", "Transparent", "PBR", "PBR-Reference", "Diffuse" })
        {
            m_builtinShaders[n] = Shader::CreateFromFile(root_dir + n + ".surf");
        }

        for (auto& n : { "Outline", "ScreenTexture", "ShadowMap", "SolidColor", "VertexLit", "VisualizeNormal" })
        {
            m_builtinShaders[n] = Shader::CreateFromFile(root_dir + n + ".shader");
        }

        m_builtinShaders["NormalMap"]           = Shader::CreateFromFile(root_dir + "NormalMap.vsfs");
        m_builtinShaders["SkyboxCubed"]         = Shader::CreateFromFile(root_dir + "Skybox-Cubed.vsfs");
        m_builtinShaders["SkyboxProcedural"]    = Shader::CreateFromFile(root_dir + "Skybox-Procedural.vsfs");
        m_builtinShaders["SolidColor-Internal"] = Shader::CreateFromFile(root_dir + "Editor/SolidColor.vsfs");
        m_builtinShaders["Alpha-Internal"]      = Shader::CreateFromFile(root_dir + "Editor/Alpha.vsfs");
        m_builtinShaders["VertexLit-Internal"]  = Shader::CreateFromFile(root_dir + "Editor/VertexLit.vsfs");
    }
}
