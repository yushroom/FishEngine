#pragma once

#include <ReflectEnum.hpp>
#include <ShaderCompiler.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ShaderCompilerErrorCode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ShaderCompilerErrorCode>() { return 4; }

// string array
static const char* ShaderCompilerErrorCodeStrings[] =
{
    "None",
	"EarlyEOF",
	"GrammarError",
	"FileNotExist"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ShaderCompilerErrorCode>()
{
    return ShaderCompilerErrorCodeStrings;
}

// index to enum
template<>
inline FishEngine::ShaderCompilerErrorCode ToEnum<FishEngine::ShaderCompilerErrorCode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ShaderCompilerErrorCode::None; break;
	case 1: return FishEngine::ShaderCompilerErrorCode::EarlyEOF; break;
	case 2: return FishEngine::ShaderCompilerErrorCode::GrammarError; break;
	case 3: return FishEngine::ShaderCompilerErrorCode::FileNotExist; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ShaderCompilerErrorCode>(FishEngine::ShaderCompilerErrorCode e)
{
    switch (e) {
    case FishEngine::ShaderCompilerErrorCode::None: return 0; break;
	case FishEngine::ShaderCompilerErrorCode::EarlyEOF: return 1; break;
	case FishEngine::ShaderCompilerErrorCode::GrammarError: return 2; break;
	case FishEngine::ShaderCompilerErrorCode::FileNotExist: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ShaderCompilerErrorCode ToEnum<FishEngine::ShaderCompilerErrorCode>(const std::string& s)
{
    if (s == "None") return FishEngine::ShaderCompilerErrorCode::None;
	if (s == "EarlyEOF") return FishEngine::ShaderCompilerErrorCode::EarlyEOF;
	if (s == "GrammarError") return FishEngine::ShaderCompilerErrorCode::GrammarError;
	if (s == "FileNotExist") return FishEngine::ShaderCompilerErrorCode::FileNotExist;
	
    abort();
}


} // namespace FishEngine
