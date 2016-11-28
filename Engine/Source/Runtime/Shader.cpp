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


std::string ReadFile(const Path& path)
{
    std::ifstream fin(path.string());
    if (!fin.is_open()) {
        FishEngine::Debug::LogError("Can not open shader file: %s", path.string().c_str());
        throw exception();
    }
    return std::string(
        std::istreambuf_iterator<char>(fin.rdbuf()),
        std::istreambuf_iterator<char>());
}

std::map<std::string, std::string> pathToShaderString;

std::string GetHeader(const Path& path)
{
    auto path_str = path.string();
    auto it = pathToShaderString.find(path_str);
    if (it != pathToShaderString.end())
    {
        return it->second;
    }
    auto str = ReadFile(path_str);
    pathToShaderString[path_str] = str;
    return str;
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
    case GL_FLOAT_VEC3:
        return "GL_FLOAT_VEC3";
    case GL_FLOAT_VEC4:
        return "GL_FLOAT_VEC4";
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
    //enum class ShaderFileType
    //{
    //    Surface,
    //    Combined,
    //};

    enum class ShaderType
    {
        VertexShader,
        FragmentShader,
        GeometryShader,
    };
    
    enum class ShaderCompilerError
    {
        EarlyEOF,
        Grammar,
    };
    
    class ShaderCompiler
    {
    public:
        ShaderCompiler() = delete;
        
        static void Preprocess(const Path& path,
                               std::string& out_parsedShaderText,
                               std::map<std::string, std::string>& out_settings,
                               bool& out_hasGeometryShader)
        {
            std::string& shaderText = ReadFile(path);
            if (path.extension() == ".surf")
            {
                shaderText = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n" +
                    shaderText + "\n#endif\n";
            }

            PreprocessImpl(shaderText,
                out_parsedShaderText,
                out_settings,
                out_hasGeometryShader,
                path.parent_path());
            //cout << out_parsedShaderText << endl;
        }

    private:

        static void PreprocessImpl(
            const std::string& shaderText,
            std::string& out_parsedShaderText,
            std::map<std::string, std::string>& out_settings,
            bool& out_hasGeometryShader,
            const Path& localDir)
        {
            std::string parsed;
            parsed.reserve(shaderText.size());

            size_t begin = 0;
            size_t end = shaderText.size();

            while (begin < end)
            {
                size_t begin_of_this_tok = begin;
                string tok = nextTok(shaderText, begin);
                if (tok == "//")
                {
                    readToNewline(shaderText, begin);
                    out_parsedShaderText += shaderText.substr(begin_of_this_tok, begin - begin_of_this_tok);
                }
                else if (tok == "/*")
                {
                    bool found_end_of_comment = false;
                    size_t i = begin;
                    for (; i < shaderText.size() - 1; ++i)
                    {
                        if (shaderText[i] == '*' && shaderText[i + 1] == '/')
                        {
                            found_end_of_comment = true;
                            break;
                        }
                    }
                    assert(found_end_of_comment);
                    begin = i + 2;
                    out_parsedShaderText += shaderText.substr(begin_of_this_tok, begin - begin_of_this_tok);
                }
                else if (tok == "#include")
                {
                    ignoreSpace(shaderText, begin);
                    auto tok = nextTok(shaderText, begin);
                    bool is_system_include = tok[0] == '<';
                    auto header = tok.substr(1, tok.size() - 2);
                    cout << "include " << header << " " << is_system_include << endl;
                    Path header_path = is_system_include ? 
                        (Resources::shaderHeaderDirectory() / header) : (localDir / header);
                    //string header_text = GetHeader(header_path);
                    string parsed_header_text;
                    ShaderCompiler::Preprocess(header_path, parsed_header_text, out_settings, out_hasGeometryShader);
                    //out_parsedShaderText += shaderText.substr(begin_of_this_tok, begin-begin_of_this_tok);
                    out_parsedShaderText += parsed_header_text + "\n";
                }
                else if (tok == "uniform")
                {
                    ignoreSpace(shaderText, begin);
                    auto type = nextTok(shaderText, begin);
                    ignoreSpace(shaderText, begin);
                    auto name = nextTok(shaderText, begin);
                    //cout << "uniform " << type << " " << name << endl;
                    out_parsedShaderText += shaderText.substr(begin_of_this_tok, begin - begin_of_this_tok);
                }
                else if (tok.size() > 0 && tok[0] == '@')
                {
                    if (tok == "@vertex")
                    {
                        out_parsedShaderText += parseSubShader(shaderText, begin, "VERTEX_SHADER", out_settings, localDir);
                    }
                    else if (tok == "@geometry")
                    {
                        out_parsedShaderText += parseSubShader(shaderText, begin, "GEOMETRY_SHADER", out_settings, localDir);
                    }
                    else if (tok == "@fragment")
                    {
                        out_parsedShaderText += parseSubShader(shaderText, begin, "FRAGMENT_SHADER", out_settings, localDir);
                    }
                    else    // keyword
                    {
                        auto name = tok.substr(1);
                        ignoreSpace(shaderText, begin);
                        auto setting = nextTok(shaderText, begin);
                        readToNewline(shaderText, begin);
                        cout << "Keyword " << name << " " << setting << endl;
                        // do NOT override settings;
                        auto it = out_settings.find(name);
                        if (it == out_settings.end())
                            out_settings[name] = setting;
                    }
                }
                else
                {
                    out_parsedShaderText += tok;
                }
            }
        }

        static std::string parseSubShader(
            const std::string& shaderText,
            size_t& begin,
            const std::string& str,
            std::map<std::string,
            std::string>& out_settings,
            const Path& localDir)
        {
            std::string out_parsedShaderText;
            ignoreSpace(shaderText, begin);
            expect(shaderText, begin, "{");
            //cout << "vertex begin"  << endl;
            out_parsedShaderText += "#ifdef " + str;
            size_t right = findPair(shaderText, begin);
            string parsed_vertex;
            bool ignore;
            //std::cout << shaderText.substr(begin, right - begin) << std::endl;
            ShaderCompiler::PreprocessImpl(shaderText.substr(begin, right - begin), parsed_vertex, out_settings, ignore, localDir);
            out_parsedShaderText += parsed_vertex;
            out_parsedShaderText += "#endif /*" + str + "*/\n";
            begin = right + 1;
            return out_parsedShaderText;
        }

        static std::string nextTok(const std::string &shaderText, size_t& pos)
        {
            size_t start = pos;
            size_t end = shaderText.size();
            
            string test = shaderText.substr(start, 2);
            if (test == "//" || test == "/*" || test == "*/")
            {
                return test;
            }

            test = shaderText.substr(start, 8);
            if (test == "#include")
            {
                pos += 8;
                return test;
            }
            bool first_is_space = std::isspace(shaderText[start]);
            while (pos < end)
            {
                char c = shaderText[pos];
                bool is_space = std::isspace(c);
                if ((first_is_space && !is_space) ||
                    (!first_is_space && is_space))
                    break;
                pos++;
            }
            if (start == pos)
                pos ++;
            return shaderText.substr(start, pos-start);
        };
        
        static void readToNewline(const std::string& shaderText, size_t& pos)
        {
            for (; pos < shaderText.size(); pos++)
            {
                if (shaderText[pos] == '\n')
                    return;
            }
        }
        
//        static void readUntil(const std::string& shaderText, size_t& pos, const string& target)
//        {
//            size_t end = shaderText.size();
//            while (pos < end)
//            {
//                if (nextTok(shaderText, pos) == target)
//                    break;
//            }
//        };
        
        static void ignoreSpace(const std::string& text, size_t& pos)
        {
            if (std::isspace(text[pos]))
                nextTok(text, pos);
        }
        
        static bool expect(const std::string& text, size_t& pos, const std::string& target)
        {
            for (int i = 0; i < target.size(); ++i)
            {
                if (text[pos+i] != target[i])
                    return false;
            }
            pos += target.size();
            return true;
        }
        
        static size_t findPair(const std::string& text, const size_t pos)
        {
            int left_count = 1;
            int i = pos;
            for (i = pos; i < text.size(); ++i)
            {
                if (text[i] == '{')
                {
                    left_count++;
                }
                else if (text[i] == '}')
                {
                    left_count--;
                    if (left_count == 0)
                    {
                        return i;
                    }
                }
            }
            return i;
        }
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
        std::string m_vertexShaderText;
        std::string m_fragmentShaderText;
        std::string m_geometryShaderText;
        //std::map<std::string, std::string>  m_settings;

    private:
        //std::string                         m_filePath;
        std::string                         m_shaderTextRaw;
        std::map<ShaderKeywords, GLuint>    m_keywordToGLPrograms;
        std::map<GLuint, std::vector<UniformInfo>>
                                            m_GLProgramToUniforms;

        GLuint Compile(ShaderType type, ShaderKeywords keywords)
        {
            std::string text = Preprocess(type, keywords);
            GLenum t = GL_VERTEX_SHADER;
            if (type == ShaderType::FragmentShader)
            {
                t = GL_FRAGMENT_SHADER;
                m_fragmentShaderText = text;
            }
            else if (type == ShaderType::GeometryShader)
            {
                t = GL_GEOMETRY_SHADER;
                m_geometryShaderText = text;
            }
            else {
                m_vertexShaderText = text;
            }
            return CompileShader(t, "#version 410 core\n" + text);
        }

        std::string Preprocess(ShaderType type, ShaderKeywords keywords)
        {
            const auto& include_dir = Resources::shaderHeaderDirectory();
            //auto text = DecorateShaderText(type, m_shaderTextRaw);
            auto& text = m_shaderTextRaw;
            context_type ctx(text.begin(), text.end());
#if FISHENGINE_PLATFORM_APPLE
            ctx.set_language(boost::wave::language_support::support_option_emit_pragma_directives);
#endif
            //ctx.add_sysinclude_path(include_dir.string().c_str());

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
                    //cout << tok << " ";
                    next_tok_is_uniform_var_name = true;
                    next_tok_is_uniform_var_type = false;
                }
                else if (next_tok_is_uniform_var_name)
                {
                    //cout << tok << endl;
                    next_tok_is_uniform_var_name = false;
                }

            SKIP:
                parsed_text += tok;
                last_tok = tok;
                ++first;
            }

            return parsed_text;
        }

        void GetAllUniforms(GLuint program) noexcept
        {
            std::vector<UniformInfo> uniforms;
            GLuint blockID = glGetUniformBlockIndex(program, "PerDraw");
            GLint blockSize = 0;
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::PerDrawUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(PerDraw));
            }

            blockID = glGetUniformBlockIndex(program, "PerFrame");
            //assert(blockID != GL_INVALID_INDEX);
            if (blockID != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(program, blockID, Pipeline::PerFrameUBOBindingPoint);
                glGetActiveUniformBlockiv(program, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                assert(blockSize == sizeof(PerFrame));
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
            std::string parsed_shader_text;
            std::map<std::string, std::string> settings;
            ShaderCompiler::Preprocess(path, parsed_shader_text, settings, m_impl->m_hasGeometryShader);
            m_cullface = ToEnum<Cullface>(getValueOrDefault<string, string>(settings, "Cull", "Back"));
            m_ZWrite = getValueOrDefault<string, string>(settings, "ZWrite", "On") == "On";
            m_blend = getValueOrDefault<string, string>(settings, "Blend", "Off") == "On";
            m_applyNormalMap = getValueOrDefault<string, string>(settings, "Normalmap", "Off") == "On";
            m_receiveShadow = getValueOrDefault<string, string>(settings, "Shadow", "On") == "On";
            
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
        catch (const boost::wave::preprocess_exception& e)
        {
            Debug::LogError("%s(%lu) : %s\n", e.file_name(), e.line_no(), e.description());
            return false;
        }
        catch (const boost::wave::cpplexer::lexing_exception& e)
        {
            Debug::LogError("%s(%lu) : %s\n", e.file_name(), e.line_no(), e.description());
            return false;
        }
        catch (const std::exception& e)
        {
            //printf("VERTEX\n==========\n%s\n==========\n", AddLineNumber(m_impl->m_vertexShaderText).c_str());
            //printf("Geometry\n==========\n%s\n==========\n", AddLineNumber(m_impl->m_geometryShaderText).c_str());
            //printf("FRAGMENT\n==========\n%s\n==========\n", AddLineNumber(m_impl->m_fragmentShaderText).c_str());
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
                glUniformMatrix4fv(u.location, matrixArray.size(), GL_TRUE, matrixArray.data()->data());
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
                //            else {
                //                Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                //            }
            }
            else if (u.type == GL_FLOAT_VEC3)
            {
                auto it = uniforms.vec3s.find(u.name);
                if (it != uniforms.vec3s.end())
                {
                    glUniform3fv(u.location, 1, it->second.data());
                    u.binded = true;
                }
                //            else {
                //                Debug::LogWarning("%s of type %u not found", u.name.c_str(), u.type);
                //            }
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
    }

    void Shader::BindTextures(const std::map<std::string, TexturePtr>& textures)
    {
        int texture_id = 0;
        for (auto& u : m_uniforms)
        {
            if (u.type != GL_SAMPLER_2D && u.type != GL_SAMPLER_CUBE) continue;
            auto it = textures.find(u.name);
            if (it != textures.end())
            {
                GLenum type = u.type == GL_SAMPLER_2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
                //BindUniformTexture(u.name.c_str(), it->second->GLTexuture(), texture_id, type);
                glActiveTexture(GLenum(GL_TEXTURE0 + texture_id));
                glBindTexture(type, it->second->GetNativeTexturePtr());
                //GLuint loc = _getUniformLocation(name);
                glUniform1i(u.location, texture_id);
                texture_id++;
                u.binded = true;
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
        for (auto& n : { "Texture", "TextureDoubleSided", "Transparent", "PBR", "PBR-Reference", "Diffuse"})
        {
            m_builtinShaders[n] = Shader::CreateFromFile(root_dir / (string(n) + ".surf"));
        }

        for (auto& n : { "Outline", "ScreenTexture", "ShadowMap", "SolidColor", "VertexLit", "VisualizeNormal", "NormalMap"})
        {
            m_builtinShaders[n] = Shader::CreateFromFile(root_dir / (string(n) + ".shader"));
        }

        m_builtinShaders["SkyboxCubed"]         = Shader::CreateFromFile(root_dir / "Skybox-Cubed.shader");
        m_builtinShaders["SkyboxProcedural"]    = Shader::CreateFromFile(root_dir / "Skybox-Procedural.shader");
        m_builtinShaders["SolidColor-Internal"] = Shader::CreateFromFile(root_dir / "Editor/SolidColor.shader");
        m_builtinShaders["Alpha-Internal"]      = Shader::CreateFromFile(root_dir / "Editor/Alpha.shader");
        m_builtinShaders["VertexLit-Internal"]  = Shader::CreateFromFile(root_dir / "Editor/VertexLit.shader");
    }
}
