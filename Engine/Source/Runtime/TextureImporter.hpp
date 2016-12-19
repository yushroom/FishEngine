#ifndef TextureImporter_hpp
#define TextureImporter_hpp

#include "FishEngine.hpp"
#include "AssetImporter.hpp"
#include "Texture.hpp"
#include "Resources.hpp"

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

    enum class TextureImporterShape
    {
        Texture2D,
        TextureCube,
    };

    enum class TextureImporterCompression
    {
        Uncompressed,
        Compressed,
        CompressedHQ,
        CompressedLQ,
    };
    
    // Texture importer lets you modify Texture2D import settings from editor scripts.
    // Settings of this class match the ones exposed in Texture Import Settings.
    class FE_EXPORT TextureImporter : public AssetImporter
    {
    public:
        TexturePtr FromFile(const Path& path);

        TexturePtr FromRawData(const uint8_t* data, int width, int height, TextureFormat format);
        
        // Which type of texture are we dealing with here.
        TextureImporterType textureType() const
        {
            return m_textureType;
        }
        
        void setTextureType(const TextureImporterType textureType)
        {
            m_textureType = textureType;
        }
        
        // Filtering mode of the texture.
        FilterMode filterMode() const
        {
            return m_filterMode;
        }
        
        void setFilterMode(const FilterMode filterMode)
        {
            m_filterMode = filterMode;
        }
        
        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode wrapMode() const
        {
            return m_wrapMode;
        }
        
        void setWrapMode(const TextureWrapMode wrapMode)
        {
            m_wrapMode = wrapMode;
        }

        
    private:
        
        // Which type of texture are we dealing with here.
        TextureImporterType     m_textureType   = TextureImporterType::Image;

        // Filtering mode of the texture.
        FilterMode              m_filterMode    = FilterMode::Trilinear;

        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode         m_wrapMode      = TextureWrapMode::Repeat;

        bool                    m_isReadable    = false;
        bool                    m_mipmapEnabled = false;

        bool mipmapEnabled = true;
    };
}

#endif /* TextureImporter_hpp */
