#ifndef TextureImporter_hpp
#define TextureImporter_hpp

#include "AssetImporter.hpp"
#include "TextureImporterProperties.hpp"

#include <Texture.hpp>
#include <Resources.hpp>

class TextureImporterInspector;

namespace FishEditor
{
	struct TextureSettings
	{
		FishEngine::FilterMode		m_filterMode;
		int							m_aniso;
		float						m_mipBias;
		FishEngine::TextureWrapMode m_wrapMode;
	};

	// Texture importer lets you modify Texture2D import settings from editor scripts.
	// Settings of this class match the ones exposed in Texture Import Settings.
	class FE_EXPORT TextureImporter final : public AssetImporter
	{
	public:
		InjectClassName(TextureImporter)

		TextureImporter() = default;
		
		TextureImporter& operator=(TextureImporter const & rhs);

		FishEngine::TexturePtr Import(FishEngine::Path const & path);

		//FishEngine::TexturePtr FromFile(const FishEngine::Path& path);

		//FishEngine::TexturePtr FromRawData(const uint8_t* data, int width, int height, FishEngine::TextureFormat format);
		
		/************************************************************************/
		/* properties                                                           */
		/************************************************************************/

		// Which type of texture are we dealing with here.
		TextureImporterType textureType() const
		{
			return m_textureType;
		}
		
		void setTextureType(const TextureImporterType textureType)
		{
			m_textureType = textureType;
		}

		TextureImporterShape textureShape() const
		{
			return m_textureShape;
		}

		void setTextureShape(const TextureImporterShape textureShape)
		{
			m_textureShape = textureShape;
		}
		
		// Filtering mode of the texture.
		FishEngine::FilterMode filterMode() const
		{
			return m_textureSettings.m_filterMode;
		}
		
		void setFilterMode(const FishEngine::FilterMode filterMode)
		{
			m_textureSettings.m_filterMode = filterMode;
		}
		
		// Wrap mode (Repeat or Clamp) of the texture.
		FishEngine::TextureWrapMode wrapMode() const
		{
			return m_textureSettings.m_wrapMode;
		}
		
		void setWrapMode(const FishEngine::TextureWrapMode wrapMode)
		{
			m_textureSettings.m_wrapMode = wrapMode;
		}

		bool sRGBTexture() const
		{
			return m_sRGBTexture;
		}

		void setSRGBTexture(const bool sRGBTexture)
		{
			m_sRGBTexture = sRGBTexture;
		}

		bool isReadable() const
		{
			return m_isReadable;
		}

		void setIsReadable(const bool isReadable)
		{
			m_isReadable = isReadable;
		}

		bool mipmapEnabled() const
		{
			return m_mipmapEnabled;
		}

		void setMipmapEnabled(const bool mipmapEnabled)
		{
			m_mipmapEnabled = mipmapEnabled;
		}
		
	protected:
		void ImportTo(FishEngine::Texture2DPtr & texture);
		
		virtual void Reimport() override;
		
	private:
		friend class Inspector;
		friend class ::TextureImporterInspector;

		// Allows alpha splitting on relevant platforms for this texture.
		bool m_allowAlphaSplitting;

		// If the provided alpha channel is transparency, enable this to prefilter the color to avoid filtering artifacts.
		bool m_alphaIsTransparency;

		// Select how the alpha of the imported texture is generated.
		TextureImporterAlphaSource m_alphaSource;

		// Anisotropic filtering level of the texture.
		int m_anisoLevel;

		// Keep texture borders the same when generating mipmaps?
		bool m_borderMipmap;

		// Quality of Texture Compression in the range[0..100].
		int m_compressionQuality;

		// Convert heightmap to normal map?
		bool m_convertToNormalmap;

		// Use crunched compression when available.
		bool m_crunchedCompression;

		// Fade out mip levels to gray color?
		bool m_fadeout;

		// Amount of bumpyness in the heightmap.
		float m_heightmapScale;

		// Cubemap generation mode.
		TextureImporterGenerateCubemap m_generateCubemap;

		// Which type of texture are we dealing with here.
		TextureImporterType m_textureType = TextureImporterType::Default;

		// Shape of imported texture.
		TextureImporterShape m_textureShape = TextureImporterShape::Texture2D;

		TextureSettings m_textureSettings;

		// Is texture storing color data?
		bool m_sRGBTexture	= true;

		// Set this to true if you want texture data to be readable from scripts. Set it to false to prevent scripts from reading texture data.
		bool m_isReadable    = false;
		
		// Select this to enable mip-map generation. Mip maps are smaller versions of the Texture that get used when the Texture is very small on screen.
		bool m_mipmapEnabled = true;
	};

}

#endif /* TextureImporter_hpp */
