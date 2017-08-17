#pragma once

#include <ReflectEnum.hpp>
#include <AudioType.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AudioType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AudioType>() { return 13; }

// string array
static const char* AudioTypeStrings[] =
{
    "UNKNOWN",
	"ACC",
	"AIFF",
	"IT",
	"MOD",
	"MPEG",
	"OGGVORBIS",
	"S3M",
	"WAV",
	"XM",
	"XMA",
	"VAG",
	"AUDIOQUEUE"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AudioType>()
{
    return AudioTypeStrings;
}

// index to enum
template<>
inline FishEngine::AudioType ToEnum<FishEngine::AudioType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AudioType::UNKNOWN; break;
	case 1: return FishEngine::AudioType::ACC; break;
	case 2: return FishEngine::AudioType::AIFF; break;
	case 3: return FishEngine::AudioType::IT; break;
	case 4: return FishEngine::AudioType::MOD; break;
	case 5: return FishEngine::AudioType::MPEG; break;
	case 6: return FishEngine::AudioType::OGGVORBIS; break;
	case 7: return FishEngine::AudioType::S3M; break;
	case 8: return FishEngine::AudioType::WAV; break;
	case 9: return FishEngine::AudioType::XM; break;
	case 10: return FishEngine::AudioType::XMA; break;
	case 11: return FishEngine::AudioType::VAG; break;
	case 12: return FishEngine::AudioType::AUDIOQUEUE; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AudioType>(FishEngine::AudioType e)
{
    switch (e) {
    case FishEngine::AudioType::UNKNOWN: return 0; break;
	case FishEngine::AudioType::ACC: return 1; break;
	case FishEngine::AudioType::AIFF: return 2; break;
	case FishEngine::AudioType::IT: return 3; break;
	case FishEngine::AudioType::MOD: return 4; break;
	case FishEngine::AudioType::MPEG: return 5; break;
	case FishEngine::AudioType::OGGVORBIS: return 6; break;
	case FishEngine::AudioType::S3M: return 7; break;
	case FishEngine::AudioType::WAV: return 8; break;
	case FishEngine::AudioType::XM: return 9; break;
	case FishEngine::AudioType::XMA: return 10; break;
	case FishEngine::AudioType::VAG: return 11; break;
	case FishEngine::AudioType::AUDIOQUEUE: return 12; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AudioType ToEnum<FishEngine::AudioType>(const std::string& s)
{
    if (s == "UNKNOWN") return FishEngine::AudioType::UNKNOWN;
	if (s == "ACC") return FishEngine::AudioType::ACC;
	if (s == "AIFF") return FishEngine::AudioType::AIFF;
	if (s == "IT") return FishEngine::AudioType::IT;
	if (s == "MOD") return FishEngine::AudioType::MOD;
	if (s == "MPEG") return FishEngine::AudioType::MPEG;
	if (s == "OGGVORBIS") return FishEngine::AudioType::OGGVORBIS;
	if (s == "S3M") return FishEngine::AudioType::S3M;
	if (s == "WAV") return FishEngine::AudioType::WAV;
	if (s == "XM") return FishEngine::AudioType::XM;
	if (s == "XMA") return FishEngine::AudioType::XMA;
	if (s == "VAG") return FishEngine::AudioType::VAG;
	if (s == "AUDIOQUEUE") return FishEngine::AudioType::AUDIOQUEUE;
	
    abort();
}


} // namespace FishEngine
