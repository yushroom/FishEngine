#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <FishEngine/PrimitiveType.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::PrimitiveType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::PrimitiveType>() { return 8; }

// string array
static const char* PrimitiveTypeStrings[] =
{
    "Sphere",
	"Capsule",
	"Cylinder",
	"Cube",
	"Plane",
	"Quad",
	"Cone",
	"ScreenAlignedQuad"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::PrimitiveType>()
{
    return PrimitiveTypeStrings;
}

// index to enum
template<>
inline FishEngine::PrimitiveType ToEnum<FishEngine::PrimitiveType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::PrimitiveType::Sphere; break;
	case 1: return FishEngine::PrimitiveType::Capsule; break;
	case 2: return FishEngine::PrimitiveType::Cylinder; break;
	case 3: return FishEngine::PrimitiveType::Cube; break;
	case 4: return FishEngine::PrimitiveType::Plane; break;
	case 5: return FishEngine::PrimitiveType::Quad; break;
	case 6: return FishEngine::PrimitiveType::Cone; break;
	case 7: return FishEngine::PrimitiveType::ScreenAlignedQuad; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::PrimitiveType>(FishEngine::PrimitiveType e)
{
    switch (e) {
    case FishEngine::PrimitiveType::Sphere: return 0; break;
	case FishEngine::PrimitiveType::Capsule: return 1; break;
	case FishEngine::PrimitiveType::Cylinder: return 2; break;
	case FishEngine::PrimitiveType::Cube: return 3; break;
	case FishEngine::PrimitiveType::Plane: return 4; break;
	case FishEngine::PrimitiveType::Quad: return 5; break;
	case FishEngine::PrimitiveType::Cone: return 6; break;
	case FishEngine::PrimitiveType::ScreenAlignedQuad: return 7; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::PrimitiveType ToEnum<FishEngine::PrimitiveType>(const std::string& s)
{
    if (s == "Sphere") return FishEngine::PrimitiveType::Sphere;
	if (s == "Capsule") return FishEngine::PrimitiveType::Capsule;
	if (s == "Cylinder") return FishEngine::PrimitiveType::Cylinder;
	if (s == "Cube") return FishEngine::PrimitiveType::Cube;
	if (s == "Plane") return FishEngine::PrimitiveType::Plane;
	if (s == "Quad") return FishEngine::PrimitiveType::Quad;
	if (s == "Cone") return FishEngine::PrimitiveType::Cone;
	if (s == "ScreenAlignedQuad") return FishEngine::PrimitiveType::ScreenAlignedQuad;
	
    abort();
}


} // namespace FishEngine
