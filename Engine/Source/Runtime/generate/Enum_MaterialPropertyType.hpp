#pragma once

#include <ReflectEnum.hpp>
#include <Material.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::MaterialPropertyType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::MaterialPropertyType>() { return 11; }

// string array
static const char* MaterialPropertyTypeStrings[] =
{
    "Float",
	"Float2",
	"Float3",
	"Float4",
	"Mat3",
	"Mat4",
	"Texture2D",
	"Texture2DArray",
	"Texture2DArrayShadow",
	"Texture3D",
	"TextureCube"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::MaterialPropertyType>()
{
    return MaterialPropertyTypeStrings;
}

// index to enum
template<>
inline FishEngine::MaterialPropertyType ToEnum<FishEngine::MaterialPropertyType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::MaterialPropertyType::Float; break;
	case 1: return FishEngine::MaterialPropertyType::Float2; break;
	case 2: return FishEngine::MaterialPropertyType::Float3; break;
	case 3: return FishEngine::MaterialPropertyType::Float4; break;
	case 4: return FishEngine::MaterialPropertyType::Mat3; break;
	case 5: return FishEngine::MaterialPropertyType::Mat4; break;
	case 6: return FishEngine::MaterialPropertyType::Texture2D; break;
	case 7: return FishEngine::MaterialPropertyType::Texture2DArray; break;
	case 8: return FishEngine::MaterialPropertyType::Texture2DArrayShadow; break;
	case 9: return FishEngine::MaterialPropertyType::Texture3D; break;
	case 10: return FishEngine::MaterialPropertyType::TextureCube; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::MaterialPropertyType>(FishEngine::MaterialPropertyType e)
{
    switch (e) {
    case FishEngine::MaterialPropertyType::Float: return 0; break;
	case FishEngine::MaterialPropertyType::Float2: return 1; break;
	case FishEngine::MaterialPropertyType::Float3: return 2; break;
	case FishEngine::MaterialPropertyType::Float4: return 3; break;
	case FishEngine::MaterialPropertyType::Mat3: return 4; break;
	case FishEngine::MaterialPropertyType::Mat4: return 5; break;
	case FishEngine::MaterialPropertyType::Texture2D: return 6; break;
	case FishEngine::MaterialPropertyType::Texture2DArray: return 7; break;
	case FishEngine::MaterialPropertyType::Texture2DArrayShadow: return 8; break;
	case FishEngine::MaterialPropertyType::Texture3D: return 9; break;
	case FishEngine::MaterialPropertyType::TextureCube: return 10; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::MaterialPropertyType ToEnum<FishEngine::MaterialPropertyType>(const std::string& s)
{
    if (s == "Float") return FishEngine::MaterialPropertyType::Float;
	if (s == "Float2") return FishEngine::MaterialPropertyType::Float2;
	if (s == "Float3") return FishEngine::MaterialPropertyType::Float3;
	if (s == "Float4") return FishEngine::MaterialPropertyType::Float4;
	if (s == "Mat3") return FishEngine::MaterialPropertyType::Mat3;
	if (s == "Mat4") return FishEngine::MaterialPropertyType::Mat4;
	if (s == "Texture2D") return FishEngine::MaterialPropertyType::Texture2D;
	if (s == "Texture2DArray") return FishEngine::MaterialPropertyType::Texture2DArray;
	if (s == "Texture2DArrayShadow") return FishEngine::MaterialPropertyType::Texture2DArrayShadow;
	if (s == "Texture3D") return FishEngine::MaterialPropertyType::Texture3D;
	if (s == "TextureCube") return FishEngine::MaterialPropertyType::TextureCube;
	
    abort();
}


} // namespace FishEngine
