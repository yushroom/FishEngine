#ifndef TextureImporter_hpp
#define TextureImporter_hpp

#include "FishEngine.hpp"
#include "AssetImporter.hpp"

namespace FishEngine
{
    // Filtering mode for textures.
    enum class FilterMode
    {
        Point,
        Bilinear,
        Trilinear,
    };
    
    // Texture importer lets you modify Texture2D import settings from editor scripts.
    // Settings of this class match the ones exposed in Texture Import Settings.
    class TextureImporter : public AssetImporter
    {
        
    };
}

#endif /* TextureImporter_hpp */
