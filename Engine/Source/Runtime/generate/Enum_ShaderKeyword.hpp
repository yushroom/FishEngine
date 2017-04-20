#pragma once

#include <ReflectEnum.hpp>
#include <ShaderProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ShaderKeyword
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ShaderKeyword>() { return 3; }

// string array
static const char* ShaderKeywordStrings[] =
{
    "None",
	"AmbientIBL",
	"All"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ShaderKeyword>()
{
    return ShaderKeywordStrings;
}

// index to enum
template<>
inline FishEngine::ShaderKeyword ToEnum<FishEngine::ShaderKeyword>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ShaderKeyword::None; break;
	case 1: return FishEngine::ShaderKeyword::AmbientIBL; break;
	case 2: return FishEngine::ShaderKeyword::All; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ShaderKeyword>(FishEngine::ShaderKeyword e)
{
    switch (e) {
    case FishEngine::ShaderKeyword::None: return 0; break;
	case FishEngine::ShaderKeyword::AmbientIBL: return 1; break;
	case FishEngine::ShaderKeyword::All: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ShaderKeyword ToEnum<FishEngine::ShaderKeyword>(const std::string& s)
{
    if (s == "None") return FishEngine::ShaderKeyword::None;
	if (s == "AmbientIBL") return FishEngine::ShaderKeyword::AmbientIBL;
	if (s == "All") return FishEngine::ShaderKeyword::All;
	
    abort();
}


} // namespace FishEngine
