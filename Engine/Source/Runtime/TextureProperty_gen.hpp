#pragma once

#include "Common.hpp"
#include "TextureProperty.hpp"

namespace FishEngine
{
    // enum count
    template<>
    constexpr int EnumCount<TextureImporterType>() { return 8; }

    // string array
    static const char* TextureImporterTypeStrings[] =
    {
        "Default",
        "NormalMap",
        "GUI",
        "Sprite",
        "Cursor",
        "Cookie",
        "Lightmap",
        "SingleChannel"
    };

    // cstring array
    template<>
    inline constexpr const char** EnumToCStringArray<TextureImporterType>()
    {
        return TextureImporterTypeStrings;
    }

    // index to enum
    template<>
    inline TextureImporterType ToEnum<TextureImporterType>(const int index)
    {
        switch (index) {
        case 0: return TextureImporterType::Default; break;
        case 1: return TextureImporterType::NormalMap; break;
        case 2: return TextureImporterType::GUI; break;
        case 3: return TextureImporterType::Sprite; break;
        case 4: return TextureImporterType::Cursor; break;
        case 5: return TextureImporterType::Cookie; break;
        case 6: return TextureImporterType::Lightmap; break;
        case 7: return TextureImporterType::SingleChannel; break;

        default: abort(); break;
        }
    }

    // enum to index
    template<>
    inline int EnumToIndex<TextureImporterType>(TextureImporterType e)
    {
        switch (e) {
        case TextureImporterType::Default: return 0; break;
        case TextureImporterType::NormalMap: return 1; break;
        case TextureImporterType::GUI: return 2; break;
        case TextureImporterType::Sprite: return 3; break;
        case TextureImporterType::Cursor: return 4; break;
        case TextureImporterType::Cookie: return 5; break;
        case TextureImporterType::Lightmap: return 6; break;
        case TextureImporterType::SingleChannel: return 7; break;

        default: abort(); break;
        }
    }

    // string to enum
    template<>
    inline TextureImporterType ToEnum<TextureImporterType>(const std::string& s)
    {
        if (s == "Default") return TextureImporterType::Default;
        if (s == "NormalMap") return TextureImporterType::NormalMap;
        if (s == "GUI") return TextureImporterType::GUI;
        if (s == "Sprite") return TextureImporterType::Sprite;
        if (s == "Cursor") return TextureImporterType::Cursor;
        if (s == "Cookie") return TextureImporterType::Cookie;
        if (s == "Lightmap") return TextureImporterType::Lightmap;
        if (s == "SingleChannel") return TextureImporterType::SingleChannel;

        abort();
    }
}