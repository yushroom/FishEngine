#pragma once

#include "FishEngine.hpp"
#include "Resources.hpp"

namespace FishEngine
{

    enum class ShaderType
    {
        VertexShader,
        FragmentShader,
        GeometryShader,
    };

    enum class ShaderCompilerErrorCode
    {
        None,
        EarlyEOF,
        GrammarError,
        FileNotExist,
    };

    class ShaderCompiler
    {
    public:
        bool                                m_hasGeometryShader = false;
        std::map<std::string, std::string>  m_settings;
        ShaderCompilerErrorCode             m_error = ShaderCompilerErrorCode::None;
        std::string                         m_errorString;

    public:
        Path m_path;
        uint32_t m_includeDepth = 0;
        // absolute path to preprocessed text
        static std::map<std::string, std::string> s_cachedHeaders;

        ShaderCompiler(const Path& shaderFilePath)
            : m_path(shaderFilePath)
        {
        }

        std::string Preprocess()
        {
            return PreprocessShaderFile(m_path);
        }

    private:

        std::string PreprocessShaderFile(const Path& path);

        std::string PreprocessImpl(
            const std::string&  shaderText, 
            const Path&         localDir);

        std::string parseSubShader(
            const std::string&  shaderText,
            size_t&             cursor,
            const std::string&  str,
            const Path&         localDir);

        static std::string nextTok(const std::string &shaderText, size_t& cursor);

        static void readToNewline(const std::string& shaderText, size_t& cursor);

        static void ignoreSpace(const std::string& text, size_t& cursor);

        static bool expect(
            const std::string&  text,
            size_t&             cursor,
            const std::string&  target);

        static size_t findPair(const std::string& text, const size_t cursor);
    };
}
