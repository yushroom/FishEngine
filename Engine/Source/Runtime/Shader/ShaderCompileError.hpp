#pragma once

#include <exception>

namespace FishEngine
{
	enum class ShaderCompileErrorType
	{
		InvalidSyntax = 0,
		UnmatchedBrace,
		UnMatchedParenthese,
		UnknownType,
		EarlyEndOfFile,
		FileNotFound,
	};

	enum class ShaderCompileStage
	{
		Preprocessor,
		Compile,
		Link,
	};

	class ShaderCompileError : public std::exception
	{
	public:
		ShaderCompileError(ShaderCompileStage stage,  int lineNumber, ShaderCompileErrorType errorType)
			: m_stage(stage), m_lineNumber(lineNumber), m_errorType(errorType)
		{
			
		}

	protected:
		ShaderCompileStage 		m_stage 		= ShaderCompileStage::Preprocessor;
		int 					m_lineNumber 	= 0;
		ShaderCompileErrorType 	m_errorType 	= ShaderCompileErrorType::InvalidSyntax;
	};

	// class ShaderPrepropressError : public ShaderError
	// {

	// };

	// class ShaderCompileError : public ShaderError
	// {
	// };

	// class ShaderLinkError : public ShaderError
	// {
	// };
}
