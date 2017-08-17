#pragma once

#include <ReflectEnum.hpp>
#include <ShaderCompiler.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ShaderType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ShaderType>() { return 3; }

// string array
static const char* ShaderTypeStrings[] =
{
    "VertexShader",
	"FragmentShader",
	"GeometryShader"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ShaderType>()
{
    return ShaderTypeStrings;
}

// index to enum
template<>
inline FishEngine::ShaderType ToEnum<FishEngine::ShaderType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ShaderType::VertexShader; break;
	case 1: return FishEngine::ShaderType::FragmentShader; break;
	case 2: return FishEngine::ShaderType::GeometryShader; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ShaderType>(FishEngine::ShaderType e)
{
    switch (e) {
    case FishEngine::ShaderType::VertexShader: return 0; break;
	case FishEngine::ShaderType::FragmentShader: return 1; break;
	case FishEngine::ShaderType::GeometryShader: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ShaderType ToEnum<FishEngine::ShaderType>(const std::string& s)
{
    if (s == "VertexShader") return FishEngine::ShaderType::VertexShader;
	if (s == "FragmentShader") return FishEngine::ShaderType::FragmentShader;
	if (s == "GeometryShader") return FishEngine::ShaderType::GeometryShader;
	
    abort();
}


} // namespace FishEngine
