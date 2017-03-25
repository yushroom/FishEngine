#pragma once

#include "FishEngine.hpp"
#include "Resources.hpp"
#include "Macro.hpp"
#include "Shader/ShaderBlendFactor.hpp"

namespace FishEditor
{
	class EditorResources;
}

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

	class FE_EXPORT ShaderCompiler
	{
	public:
		InjectSerializationFunctionsNonPolymorphic(ShaderCompiler);

		bool                                m_hasGeometryShader = false;
		bool				m_blendEnabled = false;
		int					m_blendFactorCount = 0;
		ShaderBlendFactor	m_blendFactors[4];
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

		static void setShaderIncludeDir(const std::string& path)
		{
			s_shaderIncludeDir = path;
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

		friend class FishEditor::EditorResources;
		static Path s_shaderIncludeDir;
	};
}
