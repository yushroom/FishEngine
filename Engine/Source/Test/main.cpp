#include <iostream>
#include <set>
#include <cassert>
#include <map>
#include <memory>

#include <Debug.hpp>
#include <Shader.hpp>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

using namespace std;
using namespace FishEngine;


enum class FishShaderType
{
    SurfaceShader,
    VSFSShader,
};

enum class ShaderType
{
    VertexShader,
    FragmentShader,
    GeometryShader,
};


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

class ShaderCompiler;

typedef std::shared_ptr<ShaderCompiler> ShaderCompilerPtr;

class ShaderCompiler
{
public:

    static ShaderCompilerPtr FromFile(const std::string& path)
    {
        auto compiler = std::make_shared<ShaderCompiler>();
        compiler->m_rawText = ReadFile(path);
    }

    void Preprocess()
    {

    }

    //void DecorateShaderText()
    //{
    //    shader_text = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n#line 1\n" + shader_text + "#endif\n";
    //}

private:
    FishShaderType m_type;
    std::string m_filePath;
    std::string m_rawText;
    std::string m_preprocessedText;
};


set<string> uniform_types = {
    "float", "vec2", "vec3", "vec4",
    "mat2", "mat3", "mat4",
    "int", "ivec2", "ivec3", "ivec4",
    "bool", "bvec2", "bvec3", "bvec4",
    "samplerCube", "sampler2D",
};

set<string> pragmas = {
    "Normalmap", "Shadow", "AmbientIBL",
};

struct custom_directives_hooks : public boost::wave::context_policies::default_preprocessing_hooks
{
    // new signature
    template <typename ContextT, typename TokenT, typename ContainerT>
    bool
        expanding_object_like_macro(ContextT const& ctx, TokenT const& macro,
            ContainerT const& definition, TokenT const& macrocall)
    {
        auto d = macro.get_value().c_str();
        return false;
    }   // default is to normally expand the macro

    // new signature
    template <typename ContextT, typename ContainerT>
    void expanded_macro(ContextT const& ctx, ContainerT const& result)
    {}

    //template <typename ContextT, typename TokenT, typename ContainerT>
    //bool evaluated_conditional_expression(ContextT const& ctx,
    //        TokenT const& directive, ContainerT const& expression,
    //        bool expression_value)
    //{
    //    auto d = directive.get_value().c_str();
    //    std::string e = expression.front().get_value().c_str();
    //    if (e == "SKINNED")
    //        return true;
    //    return false;
    //}

    //template <typename ContextT, typename TokenT>
    //void skipped_token(ContextT const& ctx, TokenT const& token)
    //{
    //    auto d = token.get_value().c_str();
    //    Debug::Log(d);
    //}
};

typedef boost::wave::cpplexer::lex_iterator<
    boost::wave::cpplexer::lex_token<> >
    lex_iterator_type;
typedef boost::wave::context<
    std::string::iterator,
    lex_iterator_type
    //,boost::wave::iteration_context_policies::load_file_to_string
    //,custom_directives_hooks
>
context_type;


int main()
{
    Debug::Init();
    const char* shader_path = R"(D:\program\FishEngine\Engine\Shaders\PBR.surf)";
    auto path = boost::filesystem::path(shader_path);
    auto shader_dir = path.parent_path();

    cout << shader_dir << endl;

    auto shader_include_dir = R"(D:\program\FishEngine\Engine\Shaders\include)";

    auto shader_text = ReadFile(shader_path);
    if (path.extension() == ".surf")
    {
        shader_text = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n#line 1\n" + shader_text + "#endif\n";
    }
    cout << shader_text << endl;

    map<string, string> settings;

    context_type ctx(shader_text.begin(), shader_text.end(), shader_path);

    ctx.add_sysinclude_path(shader_include_dir);
    ctx.add_macro_definition("VERTEX_SHADER");
    //ctx.add_macro_definition("FRAGMENT_SHADER");

    if (ctx.is_defined_macro(string("FRAGMENT_SHADER")))
    {
        Debug::Log("defined");
    }

    auto first = ctx.begin();
    auto last = ctx.end();

    Debug::LogWarning("==================================================");

#if 0
    std::string parsed_shader_text = R"(#version 410 core
#extension GL_GOOGLE_cpp_style_line_directive : require
)";
#else
    std::string parsed_shader_text;
#endif
    parsed_shader_text.reserve(shader_text.size());
    
    int intent_level = 0;
    string last_tok = "";
    bool next_tok_is_uniform_var_type = false;
    bool next_tok_is_uniform_var_name = false;

    bool next_tok_is_pragma_type = false;
    bool next_tok_is_pragma_setting = false;
    string pragma_type = "";
    string pragma_setting = "";

    try
    {
        while (first != last) {
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
                    parsed_shader_text += "\t";
                }
            }
            if (tok == "{")
            {
                intent_level++;
            }

            if (tok == "#pragma")
            {
                cout << tok << endl;
                next_tok_is_pragma_type = true;
            }
            else if (next_tok_is_pragma_type) {
                if (id == boost::wave::T_IDENTIFIER)
                {
                    pragma_type = tok;
                    next_tok_is_pragma_setting = true;
                }
                next_tok_is_pragma_type = false;
            }
            else if (next_tok_is_pragma_setting) {
                if (id == boost::wave::T_IDENTIFIER)
                {
                    pragma_setting = tok;
                    settings[pragma_type] = pragma_setting;
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
            else if (next_tok_is_uniform_var_type) {
                if (uniform_types.find(tok) != uniform_types.end())
                {
                    cout << tok << " ";
                    next_tok_is_uniform_var_name = true;
                }
                next_tok_is_uniform_var_type = false;
            }
            else if (next_tok_is_uniform_var_name) {
                cout << tok << endl;
                next_tok_is_uniform_var_name = false;
            }

SKIP:
            parsed_shader_text += tok;
            last_tok = tok;
            ++first;
        }
    }
    catch (const boost::wave::preprocess_exception& e)
    {
        //cout.flush();
        Debug::LogError("%s(%d) : %s\n", e.file_name(), e.line_no(), e.description());
        return 1;
    }
    catch (const boost::wave::cpplexer::lexing_exception& e)
    {
        //cout.flush();
        Debug::LogError("%s(%d) : %s\n", e.file_name(), e.line_no(), e.description());
        return 1;
    }

    //std::cout << parsed_shader_text << endl;
    auto out_file = shader_dir.append("parsed.vert");
    ofstream fout(out_file.string());
    fout << parsed_shader_text << endl;
    return 0;
}
