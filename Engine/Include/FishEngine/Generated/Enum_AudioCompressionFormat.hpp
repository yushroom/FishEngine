#pragma once

#include <ReflectEnum.hpp>
#include <AudioCompressionFormat.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AudioCompressionFormat
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AudioCompressionFormat>() { return 10; }

// string array
static const char* AudioCompressionFormatStrings[] =
{
    "PCM",
	"Vorbis",
	"ADPCM",
	"MP3",
	"VAG",
	"HEVAG",
	"XMA",
	"AAC",
	"GCADPCM",
	"ATRAC9"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AudioCompressionFormat>()
{
    return AudioCompressionFormatStrings;
}

// index to enum
template<>
inline FishEngine::AudioCompressionFormat ToEnum<FishEngine::AudioCompressionFormat>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AudioCompressionFormat::PCM; break;
	case 1: return FishEngine::AudioCompressionFormat::Vorbis; break;
	case 2: return FishEngine::AudioCompressionFormat::ADPCM; break;
	case 3: return FishEngine::AudioCompressionFormat::MP3; break;
	case 4: return FishEngine::AudioCompressionFormat::VAG; break;
	case 5: return FishEngine::AudioCompressionFormat::HEVAG; break;
	case 6: return FishEngine::AudioCompressionFormat::XMA; break;
	case 7: return FishEngine::AudioCompressionFormat::AAC; break;
	case 8: return FishEngine::AudioCompressionFormat::GCADPCM; break;
	case 9: return FishEngine::AudioCompressionFormat::ATRAC9; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AudioCompressionFormat>(FishEngine::AudioCompressionFormat e)
{
    switch (e) {
    case FishEngine::AudioCompressionFormat::PCM: return 0; break;
	case FishEngine::AudioCompressionFormat::Vorbis: return 1; break;
	case FishEngine::AudioCompressionFormat::ADPCM: return 2; break;
	case FishEngine::AudioCompressionFormat::MP3: return 3; break;
	case FishEngine::AudioCompressionFormat::VAG: return 4; break;
	case FishEngine::AudioCompressionFormat::HEVAG: return 5; break;
	case FishEngine::AudioCompressionFormat::XMA: return 6; break;
	case FishEngine::AudioCompressionFormat::AAC: return 7; break;
	case FishEngine::AudioCompressionFormat::GCADPCM: return 8; break;
	case FishEngine::AudioCompressionFormat::ATRAC9: return 9; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AudioCompressionFormat ToEnum<FishEngine::AudioCompressionFormat>(const std::string& s)
{
    if (s == "PCM") return FishEngine::AudioCompressionFormat::PCM;
	if (s == "Vorbis") return FishEngine::AudioCompressionFormat::Vorbis;
	if (s == "ADPCM") return FishEngine::AudioCompressionFormat::ADPCM;
	if (s == "MP3") return FishEngine::AudioCompressionFormat::MP3;
	if (s == "VAG") return FishEngine::AudioCompressionFormat::VAG;
	if (s == "HEVAG") return FishEngine::AudioCompressionFormat::HEVAG;
	if (s == "XMA") return FishEngine::AudioCompressionFormat::XMA;
	if (s == "AAC") return FishEngine::AudioCompressionFormat::AAC;
	if (s == "GCADPCM") return FishEngine::AudioCompressionFormat::GCADPCM;
	if (s == "ATRAC9") return FishEngine::AudioCompressionFormat::ATRAC9;
	
    abort();
}


} // namespace FishEngine
