#pragma once

#include <ReflectEnum.hpp>
#include <AudioClipLoadType.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AudioClipLoadType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AudioClipLoadType>() { return 3; }

// string array
static const char* AudioClipLoadTypeStrings[] =
{
    "DecompressOnLoad",
	"CompressedInMemory",
	"Streaming"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AudioClipLoadType>()
{
    return AudioClipLoadTypeStrings;
}

// index to enum
template<>
inline FishEngine::AudioClipLoadType ToEnum<FishEngine::AudioClipLoadType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AudioClipLoadType::DecompressOnLoad; break;
	case 1: return FishEngine::AudioClipLoadType::CompressedInMemory; break;
	case 2: return FishEngine::AudioClipLoadType::Streaming; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AudioClipLoadType>(FishEngine::AudioClipLoadType e)
{
    switch (e) {
    case FishEngine::AudioClipLoadType::DecompressOnLoad: return 0; break;
	case FishEngine::AudioClipLoadType::CompressedInMemory: return 1; break;
	case FishEngine::AudioClipLoadType::Streaming: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AudioClipLoadType ToEnum<FishEngine::AudioClipLoadType>(const std::string& s)
{
    if (s == "DecompressOnLoad") return FishEngine::AudioClipLoadType::DecompressOnLoad;
	if (s == "CompressedInMemory") return FishEngine::AudioClipLoadType::CompressedInMemory;
	if (s == "Streaming") return FishEngine::AudioClipLoadType::Streaming;
	
    abort();
}


} // namespace FishEngine
