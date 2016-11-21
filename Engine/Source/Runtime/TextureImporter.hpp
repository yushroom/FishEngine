#ifndef TextureImporter_hpp
#define TextureImporter_hpp

#include "FishEngine.hpp"
#include "AssetImporter.hpp"
#include "Texture.hpp"

namespace FishEngine
{
    // Select this to set basic parameters depending on the purpose of your texture.
    enum class TextureImporterType
    {
        Image,      // This is the most common setting used for all the textures in general.
        Bump,       // Select this to turn the color channels into a format suitable for real-time normal mapping.
        GUI,        // Use this if your texture is going to be used on any HUD/GUI Controls.
        Sprite,     // Select this if you will be using your texture for Sprite graphics.
        Cursor,     // Use this if your texture is going to be used as a cursor.
        Cubemap,    // This converts your texture into Cubemap suitable for Skyboxes, Environment Reflections or Image Based Lighting (IBL).
        Cookie,     // This sets up your texture with the basic parameters used for the Cookies of your lights.
        Lightmap,   // This sets up your texture with the parameters used by the lightmap.
        Advance,    // Select this when you want to have specific parameters on your texture and you want to have total control over your texture.
    };

    
    // Texture importer lets you modify Texture2D import settings from editor scripts.
    // Settings of this class match the ones exposed in Texture Import Settings.
    class TextureImporter : public AssetImporter
    {
    public:
        TexturePtr FromFile(const std::string& path);

        // Which type of texture are we dealing with here.
        TextureImporterType textureType() const
        {
            return m_textureType;
        }
        
        void setTextureType(const TextureImporterType textureType)
        {
            m_textureType = textureType;
        }
        
    private:
        
        // Which type of texture are we dealing with here.
        TextureImporterType m_textureType = TextureImporterType::Image;
    };
}

#endif /* TextureImporter_hpp */
