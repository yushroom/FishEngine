#pragma once

#include <ReflectEnum.hpp>
#include <TextureProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::TextureFormat
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::TextureFormat>() { return 55; }

// string array
static const char* TextureFormatStrings[] =
{
    "Alpha8",
	"ARGB4444",
	"RGB24",
	"RGBA32",
	"ARGB32",
	"RGB565",
	"R16",
	"DXT1",
	"DXT5",
	"RGBA4444",
	"BGRA32",
	"RHalf",
	"RGHalf",
	"RGBAHalf",
	"RFloat",
	"RGFloat",
	"RGBAFloat",
	"YUY2",
	"BC4",
	"BC5",
	"BC6H",
	"BC7",
	"DXT1Crunched",
	"DXT5Crunched",
	"PVRTC_RGB2",
	"PVRTC_RGBA2",
	"PVRTC_RGB4",
	"PVRTC_RGBA4",
	"ETC_RGB4",
	"ATC_RGB4",
	"ATC_RGBA8",
	"EAC_R",
	"EAC_R_SIGNED",
	"EAC_RG",
	"EAC_RG_SIGNED",
	"ETC2_RGB",
	"ETC2_RGBA1",
	"ETC2_RGBA8",
	"ASTC_RGB_4x4",
	"ASTC_RGB_5x5",
	"ASTC_RGB_6x6",
	"ASTC_RGB_8x8",
	"ASTC_RGB_10x10",
	"ASTC_RGB_12x12",
	"ASTC_RGBA_4x4",
	"ASTC_RGBA_5x5",
	"ASTC_RGBA_6x6",
	"ASTC_RGBA_8x8",
	"ASTC_RGBA_10x10",
	"ASTC_RGBA_12x12",
	"ETC_RGB4_3DS",
	"ETC_RGBA8_3DS",
	"R8",
	"RG16",
	"RG32"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::TextureFormat>()
{
    return TextureFormatStrings;
}

// index to enum
template<>
inline FishEngine::TextureFormat ToEnum<FishEngine::TextureFormat>(const int index)
{
    switch (index) {
    case 0: return FishEngine::TextureFormat::Alpha8; break;
	case 1: return FishEngine::TextureFormat::ARGB4444; break;
	case 2: return FishEngine::TextureFormat::RGB24; break;
	case 3: return FishEngine::TextureFormat::RGBA32; break;
	case 4: return FishEngine::TextureFormat::ARGB32; break;
	case 5: return FishEngine::TextureFormat::RGB565; break;
	case 6: return FishEngine::TextureFormat::R16; break;
	case 7: return FishEngine::TextureFormat::DXT1; break;
	case 8: return FishEngine::TextureFormat::DXT5; break;
	case 9: return FishEngine::TextureFormat::RGBA4444; break;
	case 10: return FishEngine::TextureFormat::BGRA32; break;
	case 11: return FishEngine::TextureFormat::RHalf; break;
	case 12: return FishEngine::TextureFormat::RGHalf; break;
	case 13: return FishEngine::TextureFormat::RGBAHalf; break;
	case 14: return FishEngine::TextureFormat::RFloat; break;
	case 15: return FishEngine::TextureFormat::RGFloat; break;
	case 16: return FishEngine::TextureFormat::RGBAFloat; break;
	case 17: return FishEngine::TextureFormat::YUY2; break;
	case 18: return FishEngine::TextureFormat::BC4; break;
	case 19: return FishEngine::TextureFormat::BC5; break;
	case 20: return FishEngine::TextureFormat::BC6H; break;
	case 21: return FishEngine::TextureFormat::BC7; break;
	case 22: return FishEngine::TextureFormat::DXT1Crunched; break;
	case 23: return FishEngine::TextureFormat::DXT5Crunched; break;
	case 24: return FishEngine::TextureFormat::PVRTC_RGB2; break;
	case 25: return FishEngine::TextureFormat::PVRTC_RGBA2; break;
	case 26: return FishEngine::TextureFormat::PVRTC_RGB4; break;
	case 27: return FishEngine::TextureFormat::PVRTC_RGBA4; break;
	case 28: return FishEngine::TextureFormat::ETC_RGB4; break;
	case 29: return FishEngine::TextureFormat::ATC_RGB4; break;
	case 30: return FishEngine::TextureFormat::ATC_RGBA8; break;
	case 31: return FishEngine::TextureFormat::EAC_R; break;
	case 32: return FishEngine::TextureFormat::EAC_R_SIGNED; break;
	case 33: return FishEngine::TextureFormat::EAC_RG; break;
	case 34: return FishEngine::TextureFormat::EAC_RG_SIGNED; break;
	case 35: return FishEngine::TextureFormat::ETC2_RGB; break;
	case 36: return FishEngine::TextureFormat::ETC2_RGBA1; break;
	case 37: return FishEngine::TextureFormat::ETC2_RGBA8; break;
	case 38: return FishEngine::TextureFormat::ASTC_RGB_4x4; break;
	case 39: return FishEngine::TextureFormat::ASTC_RGB_5x5; break;
	case 40: return FishEngine::TextureFormat::ASTC_RGB_6x6; break;
	case 41: return FishEngine::TextureFormat::ASTC_RGB_8x8; break;
	case 42: return FishEngine::TextureFormat::ASTC_RGB_10x10; break;
	case 43: return FishEngine::TextureFormat::ASTC_RGB_12x12; break;
	case 44: return FishEngine::TextureFormat::ASTC_RGBA_4x4; break;
	case 45: return FishEngine::TextureFormat::ASTC_RGBA_5x5; break;
	case 46: return FishEngine::TextureFormat::ASTC_RGBA_6x6; break;
	case 47: return FishEngine::TextureFormat::ASTC_RGBA_8x8; break;
	case 48: return FishEngine::TextureFormat::ASTC_RGBA_10x10; break;
	case 49: return FishEngine::TextureFormat::ASTC_RGBA_12x12; break;
	case 50: return FishEngine::TextureFormat::ETC_RGB4_3DS; break;
	case 51: return FishEngine::TextureFormat::ETC_RGBA8_3DS; break;
	case 52: return FishEngine::TextureFormat::R8; break;
	case 53: return FishEngine::TextureFormat::RG16; break;
	case 54: return FishEngine::TextureFormat::RG32; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::TextureFormat>(FishEngine::TextureFormat e)
{
    switch (e) {
    case FishEngine::TextureFormat::Alpha8: return 0; break;
	case FishEngine::TextureFormat::ARGB4444: return 1; break;
	case FishEngine::TextureFormat::RGB24: return 2; break;
	case FishEngine::TextureFormat::RGBA32: return 3; break;
	case FishEngine::TextureFormat::ARGB32: return 4; break;
	case FishEngine::TextureFormat::RGB565: return 5; break;
	case FishEngine::TextureFormat::R16: return 6; break;
	case FishEngine::TextureFormat::DXT1: return 7; break;
	case FishEngine::TextureFormat::DXT5: return 8; break;
	case FishEngine::TextureFormat::RGBA4444: return 9; break;
	case FishEngine::TextureFormat::BGRA32: return 10; break;
	case FishEngine::TextureFormat::RHalf: return 11; break;
	case FishEngine::TextureFormat::RGHalf: return 12; break;
	case FishEngine::TextureFormat::RGBAHalf: return 13; break;
	case FishEngine::TextureFormat::RFloat: return 14; break;
	case FishEngine::TextureFormat::RGFloat: return 15; break;
	case FishEngine::TextureFormat::RGBAFloat: return 16; break;
	case FishEngine::TextureFormat::YUY2: return 17; break;
	case FishEngine::TextureFormat::BC4: return 18; break;
	case FishEngine::TextureFormat::BC5: return 19; break;
	case FishEngine::TextureFormat::BC6H: return 20; break;
	case FishEngine::TextureFormat::BC7: return 21; break;
	case FishEngine::TextureFormat::DXT1Crunched: return 22; break;
	case FishEngine::TextureFormat::DXT5Crunched: return 23; break;
	case FishEngine::TextureFormat::PVRTC_RGB2: return 24; break;
	case FishEngine::TextureFormat::PVRTC_RGBA2: return 25; break;
	case FishEngine::TextureFormat::PVRTC_RGB4: return 26; break;
	case FishEngine::TextureFormat::PVRTC_RGBA4: return 27; break;
	case FishEngine::TextureFormat::ETC_RGB4: return 28; break;
	case FishEngine::TextureFormat::ATC_RGB4: return 29; break;
	case FishEngine::TextureFormat::ATC_RGBA8: return 30; break;
	case FishEngine::TextureFormat::EAC_R: return 31; break;
	case FishEngine::TextureFormat::EAC_R_SIGNED: return 32; break;
	case FishEngine::TextureFormat::EAC_RG: return 33; break;
	case FishEngine::TextureFormat::EAC_RG_SIGNED: return 34; break;
	case FishEngine::TextureFormat::ETC2_RGB: return 35; break;
	case FishEngine::TextureFormat::ETC2_RGBA1: return 36; break;
	case FishEngine::TextureFormat::ETC2_RGBA8: return 37; break;
	case FishEngine::TextureFormat::ASTC_RGB_4x4: return 38; break;
	case FishEngine::TextureFormat::ASTC_RGB_5x5: return 39; break;
	case FishEngine::TextureFormat::ASTC_RGB_6x6: return 40; break;
	case FishEngine::TextureFormat::ASTC_RGB_8x8: return 41; break;
	case FishEngine::TextureFormat::ASTC_RGB_10x10: return 42; break;
	case FishEngine::TextureFormat::ASTC_RGB_12x12: return 43; break;
	case FishEngine::TextureFormat::ASTC_RGBA_4x4: return 44; break;
	case FishEngine::TextureFormat::ASTC_RGBA_5x5: return 45; break;
	case FishEngine::TextureFormat::ASTC_RGBA_6x6: return 46; break;
	case FishEngine::TextureFormat::ASTC_RGBA_8x8: return 47; break;
	case FishEngine::TextureFormat::ASTC_RGBA_10x10: return 48; break;
	case FishEngine::TextureFormat::ASTC_RGBA_12x12: return 49; break;
	case FishEngine::TextureFormat::ETC_RGB4_3DS: return 50; break;
	case FishEngine::TextureFormat::ETC_RGBA8_3DS: return 51; break;
	case FishEngine::TextureFormat::R8: return 52; break;
	case FishEngine::TextureFormat::RG16: return 53; break;
	case FishEngine::TextureFormat::RG32: return 54; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::TextureFormat ToEnum<FishEngine::TextureFormat>(const std::string& s)
{
    if (s == "Alpha8") return FishEngine::TextureFormat::Alpha8;
	if (s == "ARGB4444") return FishEngine::TextureFormat::ARGB4444;
	if (s == "RGB24") return FishEngine::TextureFormat::RGB24;
	if (s == "RGBA32") return FishEngine::TextureFormat::RGBA32;
	if (s == "ARGB32") return FishEngine::TextureFormat::ARGB32;
	if (s == "RGB565") return FishEngine::TextureFormat::RGB565;
	if (s == "R16") return FishEngine::TextureFormat::R16;
	if (s == "DXT1") return FishEngine::TextureFormat::DXT1;
	if (s == "DXT5") return FishEngine::TextureFormat::DXT5;
	if (s == "RGBA4444") return FishEngine::TextureFormat::RGBA4444;
	if (s == "BGRA32") return FishEngine::TextureFormat::BGRA32;
	if (s == "RHalf") return FishEngine::TextureFormat::RHalf;
	if (s == "RGHalf") return FishEngine::TextureFormat::RGHalf;
	if (s == "RGBAHalf") return FishEngine::TextureFormat::RGBAHalf;
	if (s == "RFloat") return FishEngine::TextureFormat::RFloat;
	if (s == "RGFloat") return FishEngine::TextureFormat::RGFloat;
	if (s == "RGBAFloat") return FishEngine::TextureFormat::RGBAFloat;
	if (s == "YUY2") return FishEngine::TextureFormat::YUY2;
	if (s == "BC4") return FishEngine::TextureFormat::BC4;
	if (s == "BC5") return FishEngine::TextureFormat::BC5;
	if (s == "BC6H") return FishEngine::TextureFormat::BC6H;
	if (s == "BC7") return FishEngine::TextureFormat::BC7;
	if (s == "DXT1Crunched") return FishEngine::TextureFormat::DXT1Crunched;
	if (s == "DXT5Crunched") return FishEngine::TextureFormat::DXT5Crunched;
	if (s == "PVRTC_RGB2") return FishEngine::TextureFormat::PVRTC_RGB2;
	if (s == "PVRTC_RGBA2") return FishEngine::TextureFormat::PVRTC_RGBA2;
	if (s == "PVRTC_RGB4") return FishEngine::TextureFormat::PVRTC_RGB4;
	if (s == "PVRTC_RGBA4") return FishEngine::TextureFormat::PVRTC_RGBA4;
	if (s == "ETC_RGB4") return FishEngine::TextureFormat::ETC_RGB4;
	if (s == "ATC_RGB4") return FishEngine::TextureFormat::ATC_RGB4;
	if (s == "ATC_RGBA8") return FishEngine::TextureFormat::ATC_RGBA8;
	if (s == "EAC_R") return FishEngine::TextureFormat::EAC_R;
	if (s == "EAC_R_SIGNED") return FishEngine::TextureFormat::EAC_R_SIGNED;
	if (s == "EAC_RG") return FishEngine::TextureFormat::EAC_RG;
	if (s == "EAC_RG_SIGNED") return FishEngine::TextureFormat::EAC_RG_SIGNED;
	if (s == "ETC2_RGB") return FishEngine::TextureFormat::ETC2_RGB;
	if (s == "ETC2_RGBA1") return FishEngine::TextureFormat::ETC2_RGBA1;
	if (s == "ETC2_RGBA8") return FishEngine::TextureFormat::ETC2_RGBA8;
	if (s == "ASTC_RGB_4x4") return FishEngine::TextureFormat::ASTC_RGB_4x4;
	if (s == "ASTC_RGB_5x5") return FishEngine::TextureFormat::ASTC_RGB_5x5;
	if (s == "ASTC_RGB_6x6") return FishEngine::TextureFormat::ASTC_RGB_6x6;
	if (s == "ASTC_RGB_8x8") return FishEngine::TextureFormat::ASTC_RGB_8x8;
	if (s == "ASTC_RGB_10x10") return FishEngine::TextureFormat::ASTC_RGB_10x10;
	if (s == "ASTC_RGB_12x12") return FishEngine::TextureFormat::ASTC_RGB_12x12;
	if (s == "ASTC_RGBA_4x4") return FishEngine::TextureFormat::ASTC_RGBA_4x4;
	if (s == "ASTC_RGBA_5x5") return FishEngine::TextureFormat::ASTC_RGBA_5x5;
	if (s == "ASTC_RGBA_6x6") return FishEngine::TextureFormat::ASTC_RGBA_6x6;
	if (s == "ASTC_RGBA_8x8") return FishEngine::TextureFormat::ASTC_RGBA_8x8;
	if (s == "ASTC_RGBA_10x10") return FishEngine::TextureFormat::ASTC_RGBA_10x10;
	if (s == "ASTC_RGBA_12x12") return FishEngine::TextureFormat::ASTC_RGBA_12x12;
	if (s == "ETC_RGB4_3DS") return FishEngine::TextureFormat::ETC_RGB4_3DS;
	if (s == "ETC_RGBA8_3DS") return FishEngine::TextureFormat::ETC_RGBA8_3DS;
	if (s == "R8") return FishEngine::TextureFormat::R8;
	if (s == "RG16") return FishEngine::TextureFormat::RG16;
	if (s == "RG32") return FishEngine::TextureFormat::RG32;
	
    abort();
}


} // namespace FishEngine
