#include <iostream>
#include <cassert>

#include <Debug.hpp>
//#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

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


int main()
{
    Debug::Init();
    const char* shader_path = R"(D:\program\FishEngine\Engine\Shaders\PBR2.surf)";
    auto path = boost::filesystem::path(shader_path);
    auto shader_dir = path.parent_path();

    cout << shader_dir << endl;

    auto shader_include_dir = R"(D:\program\FishEngine\Engine\Shaders\include)";

    auto shader_text = ReadFile(shader_path);
    if (path.extension() == ".surf")
    {
        shader_text = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n" + shader_text + "#endif\n";
    }
    cout << shader_text << endl;

    typedef boost::wave::cpplexer::lex_iterator<
        boost::wave::cpplexer::lex_token<> >
        lex_iterator_type;
    typedef boost::wave::context<
        std::string::iterator, lex_iterator_type>
        context_type;

    context_type ctx(shader_text.begin(), shader_text.end(), shader_path);

    //ctx.set_language(boost::wave::language_support::support_option_preserve_comments);
    //int language = boost::wave::language_support::support_option_emit_line_directives;
    //language = language & boost::wave::language_support::support_option_preserve_comments;
    //ctx.set_language(boost::wave::language_support(language));
    //ctx.set_language(boost::wave::language_support::support_option_emit_line_directives, false);
    //ctx.set_language(boost::wave::language_support::support_option_preserve_comments, false);
    ctx.add_sysinclude_path(shader_include_dir);
    //ctx.add_include_path(".");
    //ctx.set_current_filename(shader_path);
    ctx.add_macro_definition("VERTEX_SHADER");

    auto first = ctx.begin();
    auto last = ctx.end();

    Debug::LogWarning("==================================================");
    
//    std::string parsed_shader_text = R"(#version 410 core
//#extension GL_GOOGLE_cpp_style_line_directive : require
//)";
    std::string parsed_shader_text;
    parsed_shader_text.reserve(shader_text.size());
    
    int intent_level = 0;
    string last_tok = "";
    try
    {
        while (first != last) {
            //std::cout << first->get_value();
            string tok = first->get_value().c_str();

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

            parsed_shader_text += tok;
            last_tok = tok;
            ++first;
        }
    }
    catch (const boost::wave::preprocess_exception& e)
    {
        //cout.flush();
        Debug::LogError("%d : %s : %s\n", e.line_no(), e.file_name(), e.description());
        return 1;
    }
    catch (const boost::wave::cpplexer::lexing_exception& e)
    {
        //cout.flush();
        Debug::LogError(e.description());
        return 1;
    }

    //std::cout << parsed_shader_text << endl;
    auto out_file = shader_dir.append("parsed.vert");
    ofstream fout(out_file.string());
    fout << parsed_shader_text << endl;
    return 0;
}
