#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <PrefabUtility.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ReplacePrefabOptions
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ReplacePrefabOptions>() { return 3; }

// string array
static const char* ReplacePrefabOptionsStrings[] =
{
    "Default",
	"ConnectToPrefab",
	"ReplaceNameBased"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ReplacePrefabOptions>()
{
    return ReplacePrefabOptionsStrings;
}

// index to enum
template<>
inline FishEditor::ReplacePrefabOptions ToEnum<FishEditor::ReplacePrefabOptions>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ReplacePrefabOptions::Default; break;
	case 1: return FishEditor::ReplacePrefabOptions::ConnectToPrefab; break;
	case 2: return FishEditor::ReplacePrefabOptions::ReplaceNameBased; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ReplacePrefabOptions>(FishEditor::ReplacePrefabOptions e)
{
    switch (e) {
    case FishEditor::ReplacePrefabOptions::Default: return 0; break;
	case FishEditor::ReplacePrefabOptions::ConnectToPrefab: return 1; break;
	case FishEditor::ReplacePrefabOptions::ReplaceNameBased: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ReplacePrefabOptions ToEnum<FishEditor::ReplacePrefabOptions>(const std::string& s)
{
    if (s == "Default") return FishEditor::ReplacePrefabOptions::Default;
	if (s == "ConnectToPrefab") return FishEditor::ReplacePrefabOptions::ConnectToPrefab;
	if (s == "ReplaceNameBased") return FishEditor::ReplacePrefabOptions::ReplaceNameBased;
	
    abort();
}


} // namespace FishEngine
