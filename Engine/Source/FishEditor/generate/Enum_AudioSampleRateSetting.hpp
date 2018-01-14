#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <AudioSampleRateSetting.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::AudioSampleRateSetting
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::AudioSampleRateSetting>() { return 3; }

// string array
static const char* AudioSampleRateSettingStrings[] =
{
    "PreserveSampleRate",
	"OptimizeSampleRate",
	"OverrideSampleRate"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::AudioSampleRateSetting>()
{
    return AudioSampleRateSettingStrings;
}

// index to enum
template<>
inline FishEditor::AudioSampleRateSetting ToEnum<FishEditor::AudioSampleRateSetting>(const int index)
{
    switch (index) {
    case 0: return FishEditor::AudioSampleRateSetting::PreserveSampleRate; break;
	case 1: return FishEditor::AudioSampleRateSetting::OptimizeSampleRate; break;
	case 2: return FishEditor::AudioSampleRateSetting::OverrideSampleRate; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::AudioSampleRateSetting>(FishEditor::AudioSampleRateSetting e)
{
    switch (e) {
    case FishEditor::AudioSampleRateSetting::PreserveSampleRate: return 0; break;
	case FishEditor::AudioSampleRateSetting::OptimizeSampleRate: return 1; break;
	case FishEditor::AudioSampleRateSetting::OverrideSampleRate: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::AudioSampleRateSetting ToEnum<FishEditor::AudioSampleRateSetting>(const std::string& s)
{
    if (s == "PreserveSampleRate") return FishEditor::AudioSampleRateSetting::PreserveSampleRate;
	if (s == "OptimizeSampleRate") return FishEditor::AudioSampleRateSetting::OptimizeSampleRate;
	if (s == "OverrideSampleRate") return FishEditor::AudioSampleRateSetting::OverrideSampleRate;
	
    abort();
}


} // namespace FishEngine
