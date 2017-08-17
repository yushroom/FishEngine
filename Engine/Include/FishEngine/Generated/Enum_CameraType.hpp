#pragma once

#include <ReflectEnum.hpp>
#include <Camera.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::CameraType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::CameraType>() { return 3; }

// string array
static const char* CameraTypeStrings[] =
{
    "Game",
	"SceneView",
	"Preview"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::CameraType>()
{
    return CameraTypeStrings;
}

// index to enum
template<>
inline FishEngine::CameraType ToEnum<FishEngine::CameraType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::CameraType::Game; break;
	case 1: return FishEngine::CameraType::SceneView; break;
	case 2: return FishEngine::CameraType::Preview; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::CameraType>(FishEngine::CameraType e)
{
    switch (e) {
    case FishEngine::CameraType::Game: return 0; break;
	case FishEngine::CameraType::SceneView: return 1; break;
	case FishEngine::CameraType::Preview: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::CameraType ToEnum<FishEngine::CameraType>(const std::string& s)
{
    if (s == "Game") return FishEngine::CameraType::Game;
	if (s == "SceneView") return FishEngine::CameraType::SceneView;
	if (s == "Preview") return FishEngine::CameraType::Preview;
	
    abort();
}


} // namespace FishEngine
