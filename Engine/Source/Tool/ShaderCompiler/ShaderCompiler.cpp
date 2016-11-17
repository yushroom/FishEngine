#if 0
#include <iostream>
#include <set>
#include <cassert>
#include <map>
#include <memory>

#include <Debug.hpp>
#include <Shader.hpp>
#include <Resources.hpp>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

#include <GLEnvironment.hpp>
#include <glfw/glfw3.h>

using namespace std;
using namespace FishEngine;

std::string ReadFile(const std::string& path)
{
    std::ifstream fin(path);
    if (!fin.is_open()) {
        FishEngine::Debug::LogError("Can not open shader header file: %s", path.c_str());
        throw exception();
    }
    return std::string(
        std::istreambuf_iterator<char>(fin.rdbuf()),
        std::istreambuf_iterator<char>());
}

GLuint CompileShader(
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
        throw exception(infoLog.data());
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
        throw std::exception(infoLog.data());
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

typedef boost::wave::cpplexer::lex_iterator<
    boost::wave::cpplexer::lex_token<> >
    lex_iterator_type;
typedef boost::wave::context<
    std::string::iterator,
    lex_iterator_type>
    context_type;


namespace FishEngine
{
    enum class ShaderType
    {
        VertexShader,
        FragmentShader,
        GeometryShader,
        SurfaceShader,
        CombinedShader,
    };

    class ShaderImpl
    {
    public:

        ShaderImpl() = default;

        ShaderImpl(std::string shaderText, ShaderType type, std::string filePath = "placeholder.surf")
            : m_shaderTextRaw(shaderText+"\n"), m_filePath(filePath)
        {
            if (type == ShaderType::SurfaceShader)
                m_shaderTextRaw = DecorateShaderText(ShaderType::SurfaceShader, m_shaderTextRaw);
            //m_shaderTextRaw = DecorateShaderText(type, shaderText);
        }

        GLuint CompileAndLink()
        {
            auto vs = Compile(ShaderType::VertexShader);
            auto fs = Compile(ShaderType::FragmentShader);
            return LinkShader(vs, 0, 0, 0, fs);
        }

        GLuint Compile(ShaderType type)
        {
            std::string text = Preprocess(type);
            GLenum t = GL_VERTEX_SHADER;
            if (type == ShaderType::FragmentShader)
                t = GL_FRAGMENT_SHADER;
            else if (type == ShaderType::GeometryShader)
                t = GL_GEOMETRY_SHADER;
            return CompileShader(t, "#version 410 core\n" + text);
        }

        std::string Preprocess(ShaderType type)
        {
            const auto& include_dir = Resources::shaderRootDirectory() + "include/";
            auto text = DecorateShaderText(type, m_shaderTextRaw);
            //auto& text = m_shaderTextRaw;
            context_type ctx(text.begin(), text.end(), m_filePath.c_str());
            ctx.add_sysinclude_path(include_dir.c_str());

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

        static std::string DecorateShaderText(ShaderType type, std::string& shaderText)
        {
            std::string left = "", right = "\n";

            switch (type)
            {
            case FishEngine::ShaderType::VertexShader:
                left = "#define VERTEX_SHADER 1\n";
                break;
            case FishEngine::ShaderType::FragmentShader:
                left = "#define FRAGMENT_SHADER 1\n";
                break;
            case FishEngine::ShaderType::GeometryShader:
                left = "#define GEOMETRY_SHADER 1\n";
                break;
            case FishEngine::ShaderType::SurfaceShader:
                left = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n";
                right = "#endif\n";
                break;
            default:
                abort();
                break;
            }

            return left + "#line 1\n" + shaderText + right;
        }

        const std::string& shaderTextRaw() const
        {
            return m_shaderTextRaw;
        }

    private:
        std::string m_filePath;
        std::string m_shaderTextRaw;
        //std::string m_shaderTextPreprocessed;
        std::map<std::string, UniformInfo> m_uniforms;
        std::map<std::string, std::string> m_settings;
    };

} /* FishEngine */

int main(int argc, char* argv[])
{
    //Debug::Init();
    Debug::setColorMode(false);
    Debug::Log("Compiling...");
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    constexpr int WIDTH = 1;
    constexpr int HEIGHT = 1;

    // Create a GLFWwindow object that we can use for GLFW's functions
    auto window = glfwCreateWindow(WIDTH, HEIGHT, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glCheckError();

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    std::string path = R"(D:\program\FishEngine\Engine\Shaders\PBR.surf)";
    if (argc == 2)
        path = argv[1];

    auto ext = getExtensionWithoutDot(path);
    auto shader_text = ReadFile(path);
    
    try
    {
        if (ext == "surf")
        {
            ShaderImpl impl(shader_text, ShaderType::SurfaceShader, path);
            impl.CompileAndLink();
        }
        else if (ext == "vsfs")
        {
            ShaderImpl impl(shader_text, ShaderType::CombinedShader, path);
            impl.CompileAndLink();
        }
        else if (ext == "geom")
        {
            auto t = ShaderType::GeometryShader;
            ShaderImpl impl(shader_text, t, path);
            impl.Compile(t);
        }
        else
        {
            auto t = ext == "vert" ? ShaderType::VertexShader : ShaderType::FragmentShader;
            ShaderImpl impl(shader_text, t, path);
            impl.Compile(t);
        }
    }
    catch (const boost::wave::preprocess_exception& e)
    {
        //cout.flush();
        printf("%s(%d) : %s\n", e.file_name(), e.line_no(), e.description());
        return 1;
    }
    catch (const boost::wave::cpplexer::lexing_exception& e)
    {
        //cout.flush();
        printf("%s(%d) : %s\n", e.file_name(), e.line_no(), e.description());
        return 1;
    }
    catch (const std::exception& e)
    {
        cout << e.what();
        return 1;
    }
    Debug::Log("OK");
    return 0;
}

#else

#include <Shader.hpp>
#include <GLEnvironment.hpp>
#include <glfw/glfw3.h>
#include <Debug.hpp>

using namespace FishEngine;

int main(int argc, char* argv[])
{
    //Debug::Init();
    Debug::setColorMode(false);
    Debug::Log("Compiling...");
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    constexpr int WIDTH = 1;
    constexpr int HEIGHT = 1;

    // Create a GLFWwindow object that we can use for GLFW's functions
    auto window = glfwCreateWindow(WIDTH, HEIGHT, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glCheckError();

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    std::string path = R"(D:\program\FishEngine\Engine\Shaders\SkinnedPass.surf)";
    if (argc == 2)
        path = argv[1];

    auto shader = Shader::CreateFromFile(path);
    if (shader == nullptr)
        return 1;

    Debug::Log("OK");
    return 0;
}

#endif