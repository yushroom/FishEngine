#ifndef TextureImporter_hpp
#define TextureImporter_hpp

#include "FishEngine.hpp"
#include "AssetImporter.hpp"
#include "Texture.hpp"
#include "Resources.hpp"

namespace FishEngine
{
    
    // Texture importer lets you modify Texture2D import settings from editor scripts.
    // Settings of this class match the ones exposed in Texture Import Settings.
    class FE_EXPORT TextureImporter : public AssetImporter
    {
    public:
		InjectClassName(TextureImporter)

		virtual void SaveAndReimport() override {};
		
		TextureImporter() = default;

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
        TextureImporterType     m_textureType   = TextureImporterType::Default;

        // Shape of imported texture.
        TextureImporterShape    m_textureShadpe = TextureImporterShape::Texture2D;

        // Filtering mode of the texture.
        FilterMode              m_filterMode    = FilterMode::Trilinear;

        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode         m_wrapMode      = TextureWrapMode::Repeat;

        bool                    m_isReadable    = false;
        bool                    m_mipmapEnabled = false;
    };
}

#endif /* TextureImporter_hpp */
