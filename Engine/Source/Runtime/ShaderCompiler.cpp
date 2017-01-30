#include "ShaderCompiler.hpp"

#include <iostream>
#include <cctype>
#include <boost/algorithm/string.hpp>

#include "Debug.hpp"

using namespace std;

static std::string ReadFile(const FishEngine::Path& path)
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


namespace FishEngine
{

    std::map<std::string, std::string> ShaderCompiler::s_cachedHeaders;

    std::string ShaderCompiler::PreprocessShaderFile(const Path& path)
    {
        if (path.extension() == ".inc")
        {
            std::string full_path = boost::filesystem::absolute(path).string();
            auto it = s_cachedHeaders.find(full_path);
            if (it != s_cachedHeaders.end())
            {
                return it->second;
            }
            //Debug::LogWarning("Open header %s", path.string().c_str());
            const std::string& shaderText = ReadFile(path);
            auto parsed = PreprocessImpl(shaderText, m_path.parent_path());
            s_cachedHeaders[full_path] = parsed;
            return parsed;
        }

        auto shaderText = ReadFile(path);
        if (path.extension() == ".surf")
        {
            shaderText = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n" +
                shaderText + "\n#endif\n";
        }

        return PreprocessImpl(shaderText, m_path.parent_path());
    }

    std::string ShaderCompiler::PreprocessImpl(const std::string& shaderText, const Path& localDir)
    {
        std::string parsed;
        parsed.reserve(shaderText.size());

        size_t cursor = 0;
        size_t end = shaderText.size();

        while (cursor < end)
        {
            size_t begin_of_this_tok = cursor;
            string tok = nextTok(shaderText, cursor);
            if (tok == "//")
            {
                readToNewline(shaderText, cursor);
                parsed += shaderText.substr(begin_of_this_tok, cursor - begin_of_this_tok);
            }
            else if (tok == "/*")
            {
                bool found_end_of_comment = false;
                size_t i = cursor;
                for (; i < shaderText.size() - 1; ++i)
                {
                    if (shaderText[i] == '*' && shaderText[i + 1] == '/')
                    {
                        found_end_of_comment = true;
                        break;
                    }
                }
                assert(found_end_of_comment);
                cursor = i + 2;
                parsed += shaderText.substr(begin_of_this_tok, cursor - begin_of_this_tok);
            }
            else if (tok == "#include")
            {
                ignoreSpace(shaderText, cursor);
                auto tok = nextTok(shaderText, cursor);
                bool is_system_include = tok[0] == '<';
                auto header = tok.substr(1, tok.size() - 2);
                //cout << "include " << header << " " << is_system_include << endl;
                Path header_path = is_system_include ?
                    (Resources::shaderHeaderDirectory() / header) : (localDir / header);
                m_includeDepth++;
                string parsed_header_text = PreprocessShaderFile(header_path);
                //out_parsedShaderText += shaderText.substr(begin_of_this_tok, begin-begin_of_this_tok);
                parsed += parsed_header_text + "\n";
            }
            else if (tok == "uniform")
            {
                ignoreSpace(shaderText, cursor);
                auto type = nextTok(shaderText, cursor);
                ignoreSpace(shaderText, cursor);
                auto name = nextTok(shaderText, cursor);
                //cout << "uniform " << type << " " << name << endl;
                parsed += shaderText.substr(begin_of_this_tok, cursor - begin_of_this_tok);
            }
            else if (tok.size() > 0 && tok[0] == '@')
            {
                if (tok == "@vertex")
                {
                    parsed += parseSubShader(shaderText, cursor, "VERTEX_SHADER", localDir);
                }
                else if (tok == "@geometry")
                {
                    Debug::Log("Geometry shader enabled");
                    m_hasGeometryShader = true;
                    parsed += parseSubShader(shaderText, cursor, "GEOMETRY_SHADER", localDir);
                }
                else if (tok == "@fragment")
                {
                    parsed += parseSubShader(shaderText, cursor, "FRAGMENT_SHADER", localDir);
                }
                else    // keyword
                {
                    auto name = tok.substr(1);
                    boost::to_lower(name);
                    if (name == "deferred")
                    {
                        //cout << "Keyword " << name << endl;
                        m_settings[name] = "on";
                    }
                    else
                    {
                        ignoreSpace(shaderText, cursor);
                        auto setting = nextTok(shaderText, cursor);
                        boost::to_lower(setting);
                        //cout << "Keyword " << name << " " << setting << endl;
                        // TODO: override if smaller depth
                        // do NOT override settings;
                        auto it = m_settings.find(name);
                        if (it == m_settings.end())
                            m_settings[name] = setting;
                    }
                    
                    readToNewline(shaderText, cursor);
                }
            }
            else
            {
                parsed += tok;
            }
        }
        return parsed;
    }

    std::string ShaderCompiler::parseSubShader(const std::string& shaderText, size_t& cursor, const std::string& str, const Path& localDir)
    {
        std::string out_parsedShaderText;
        ignoreSpace(shaderText, cursor);
        expect(shaderText, cursor, "{");
        //cout << "vertex begin"  << endl;
        out_parsedShaderText += "#ifdef " + str;
        size_t right = findPair(shaderText, cursor);
        //std::cout << shaderText.substr(begin, right - begin) << std::endl;
        string parsed = PreprocessImpl(shaderText.substr(cursor, right - cursor), localDir);
        out_parsedShaderText += parsed;
        out_parsedShaderText += "#endif /*" + str + "*/\n";
        cursor = right + 1;
        return out_parsedShaderText;
    }

    std::string ShaderCompiler::nextTok(const std::string &shaderText, size_t& cursor)
    {
        size_t start = cursor;
        size_t end = shaderText.size();

        string test = shaderText.substr(start, 2);
        if (test == "//" || test == "/*" || test == "*/")
        {
            return test;
        }

        test = shaderText.substr(start, 8);
        if (test == "#include")
        {
            cursor += 8;
            return test;
        }
        bool first_is_space = (std::isspace(shaderText[start]) != 0);
        while (cursor < end)
        {
            char c = shaderText[cursor];
            bool is_space = (std::isspace(c) != 0);
            if ((first_is_space && !is_space) ||
                (!first_is_space && is_space))
                break;
            cursor++;
        }
        if (start == cursor)
            cursor++;
        return shaderText.substr(start, cursor - start);
    }

    void ShaderCompiler::readToNewline(const std::string& shaderText, size_t& cursor)
    {
        for (; cursor < shaderText.size(); cursor++)
        {
            if (shaderText[cursor] == '\n')
                return;
        }
    }

    void ShaderCompiler::ignoreSpace(const std::string& text, size_t& cursor)
    {
        if (std::isspace(text[cursor]))
            nextTok(text, cursor);
    }

    bool ShaderCompiler::expect(const std::string& text, size_t& cursor, const std::string& target)
    {
        for (int i = 0; i < target.size(); ++i)
        {
            if (text[cursor + i] != target[i])
                return false;
        }
        cursor += target.size();
        return true;
    }

    size_t ShaderCompiler::findPair(const std::string& text, const size_t cursor)
    {
        int left_count = 1;
        size_t i = cursor;
        for ( ; i < text.size(); ++i)
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

}
