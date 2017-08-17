#pragma once

#include <ReflectEnum.hpp>
#include <ShaderBlendFactor.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ShaderBlendFactor
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ShaderBlendFactor>() { return 10; }

// string array
static const char* ShaderBlendFactorStrings[] =
{
    "One",
	"Zero",
	"SrcColor",
	"SrcAlpha",
	"DstColor",
	"DstAlpha",
	"OneMinusSrcColor",
	"OneMinusSrcAlpha",
	"OneMinusDstColor",
	"OneMinusDstAlpha"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ShaderBlendFactor>()
{
    return ShaderBlendFactorStrings;
}

// index to enum
template<>
inline FishEngine::ShaderBlendFactor ToEnum<FishEngine::ShaderBlendFactor>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ShaderBlendFactor::One; break;
	case 1: return FishEngine::ShaderBlendFactor::Zero; break;
	case 2: return FishEngine::ShaderBlendFactor::SrcColor; break;
	case 3: return FishEngine::ShaderBlendFactor::SrcAlpha; break;
	case 4: return FishEngine::ShaderBlendFactor::DstColor; break;
	case 5: return FishEngine::ShaderBlendFactor::DstAlpha; break;
	case 6: return FishEngine::ShaderBlendFactor::OneMinusSrcColor; break;
	case 7: return FishEngine::ShaderBlendFactor::OneMinusSrcAlpha; break;
	case 8: return FishEngine::ShaderBlendFactor::OneMinusDstColor; break;
	case 9: return FishEngine::ShaderBlendFactor::OneMinusDstAlpha; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ShaderBlendFactor>(FishEngine::ShaderBlendFactor e)
{
    switch (e) {
    case FishEngine::ShaderBlendFactor::One: return 0; break;
	case FishEngine::ShaderBlendFactor::Zero: return 1; break;
	case FishEngine::ShaderBlendFactor::SrcColor: return 2; break;
	case FishEngine::ShaderBlendFactor::SrcAlpha: return 3; break;
	case FishEngine::ShaderBlendFactor::DstColor: return 4; break;
	case FishEngine::ShaderBlendFactor::DstAlpha: return 5; break;
	case FishEngine::ShaderBlendFactor::OneMinusSrcColor: return 6; break;
	case FishEngine::ShaderBlendFactor::OneMinusSrcAlpha: return 7; break;
	case FishEngine::ShaderBlendFactor::OneMinusDstColor: return 8; break;
	case FishEngine::ShaderBlendFactor::OneMinusDstAlpha: return 9; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ShaderBlendFactor ToEnum<FishEngine::ShaderBlendFactor>(const std::string& s)
{
    if (s == "One") return FishEngine::ShaderBlendFactor::One;
	if (s == "Zero") return FishEngine::ShaderBlendFactor::Zero;
	if (s == "SrcColor") return FishEngine::ShaderBlendFactor::SrcColor;
	if (s == "SrcAlpha") return FishEngine::ShaderBlendFactor::SrcAlpha;
	if (s == "DstColor") return FishEngine::ShaderBlendFactor::DstColor;
	if (s == "DstAlpha") return FishEngine::ShaderBlendFactor::DstAlpha;
	if (s == "OneMinusSrcColor") return FishEngine::ShaderBlendFactor::OneMinusSrcColor;
	if (s == "OneMinusSrcAlpha") return FishEngine::ShaderBlendFactor::OneMinusSrcAlpha;
	if (s == "OneMinusDstColor") return FishEngine::ShaderBlendFactor::OneMinusDstColor;
	if (s == "OneMinusDstAlpha") return FishEngine::ShaderBlendFactor::OneMinusDstAlpha;
	
    abort();
}


} // namespace FishEngine
