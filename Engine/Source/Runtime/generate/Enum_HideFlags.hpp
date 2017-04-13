#pragma once

#include <ReflectEnum.hpp>
#include <HideFlags.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::HideFlags
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::HideFlags>() { return 9; }

// string array
static const char* HideFlagsStrings[] =
{
    "None",
	"HideInHierarchy",
	"HideInInspector",
	"DontSaveInEditor",
	"NotEditable",
	"DontSaveInBuild",
	"DontUnloadUnusedAsset",
	"DontSave",
	"HideAndDontSave"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::HideFlags>()
{
    return HideFlagsStrings;
}

// index to enum
template<>
inline FishEngine::HideFlags ToEnum<FishEngine::HideFlags>(const int index)
{
    switch (index) {
    case 0: return FishEngine::HideFlags::None; break;
	case 1: return FishEngine::HideFlags::HideInHierarchy; break;
	case 2: return FishEngine::HideFlags::HideInInspector; break;
	case 3: return FishEngine::HideFlags::DontSaveInEditor; break;
	case 4: return FishEngine::HideFlags::NotEditable; break;
	case 5: return FishEngine::HideFlags::DontSaveInBuild; break;
	case 6: return FishEngine::HideFlags::DontUnloadUnusedAsset; break;
	case 7: return FishEngine::HideFlags::DontSave; break;
	case 8: return FishEngine::HideFlags::HideAndDontSave; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::HideFlags>(FishEngine::HideFlags e)
{
    switch (e) {
    case FishEngine::HideFlags::None: return 0; break;
	case FishEngine::HideFlags::HideInHierarchy: return 1; break;
	case FishEngine::HideFlags::HideInInspector: return 2; break;
	case FishEngine::HideFlags::DontSaveInEditor: return 3; break;
	case FishEngine::HideFlags::NotEditable: return 4; break;
	case FishEngine::HideFlags::DontSaveInBuild: return 5; break;
	case FishEngine::HideFlags::DontUnloadUnusedAsset: return 6; break;
	case FishEngine::HideFlags::DontSave: return 7; break;
	case FishEngine::HideFlags::HideAndDontSave: return 8; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::HideFlags ToEnum<FishEngine::HideFlags>(const std::string& s)
{
    if (s == "None") return FishEngine::HideFlags::None;
	if (s == "HideInHierarchy") return FishEngine::HideFlags::HideInHierarchy;
	if (s == "HideInInspector") return FishEngine::HideFlags::HideInInspector;
	if (s == "DontSaveInEditor") return FishEngine::HideFlags::DontSaveInEditor;
	if (s == "NotEditable") return FishEngine::HideFlags::NotEditable;
	if (s == "DontSaveInBuild") return FishEngine::HideFlags::DontSaveInBuild;
	if (s == "DontUnloadUnusedAsset") return FishEngine::HideFlags::DontUnloadUnusedAsset;
	if (s == "DontSave") return FishEngine::HideFlags::DontSave;
	if (s == "HideAndDontSave") return FishEngine::HideFlags::HideAndDontSave;
	
    abort();
}


} // namespace FishEngine
