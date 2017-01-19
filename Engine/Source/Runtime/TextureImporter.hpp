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
			m_isDirty = true;
		}

		TextureImporterShape textureShape() const
		{
			return m_textureShape;
		}

		void setTextureShape(const TextureImporterShape textureShape)
		{
			m_textureShape = textureShape;
			m_isDirty = true;
		}
		
		// Filtering mode of the texture.
		FilterMode filterMode() const
		{
			return m_filterMode;
		}
		
		void setFilterMode(const FilterMode filterMode)
		{
			m_filterMode = filterMode;
			m_isDirty = true;
		}
		
		// Wrap mode (Repeat or Clamp) of the texture.
		TextureWrapMode wrapMode() const
		{
			return m_wrapMode;
		}
		
		void setWrapMode(const TextureWrapMode wrapMode)
		{
			m_wrapMode = wrapMode;
			m_isDirty = true;
		}

		bool sRGBTexture() const
		{
			return m_sRGBTexture;
		}

		void setSRGBTexture(const bool sRGBTexture)
		{
			m_sRGBTexture = sRGBTexture;
			m_isDirty = true;
		}

		bool isReadable() const
		{
			return m_isReadable;
		}

		void setIsReadable(const bool isReadable)
		{
			m_isReadable = isReadable;
			m_isDirty = true;
		}

		bool mipmapEnabled() const
		{
			return m_mipmapEnabled;
		}

		void setMipmapEnabled(const bool mipmapEnabled)
		{
			m_mipmapEnabled = mipmapEnabled;
			m_isDirty = true;
		}
		
	private:
		friend class FishEditor::EditorGUI;
		// Which type of texture are we dealing with here.
		TextureImporterType		m_textureType   = TextureImporterType::Default;

		// Shape of imported texture.
		TextureImporterShape	m_textureShape	= TextureImporterShape::Texture2D;

		// Filtering mode of the texture.
		FilterMode				m_filterMode    = FilterMode::Trilinear;

		// Wrap mode (Repeat or Clamp) of the texture.
		TextureWrapMode			m_wrapMode      = TextureWrapMode::Repeat;
		
		// Is texture storing color data?
		bool					m_sRGBTexture	= true;

		// Set this to true if you want texture data to be readable from scripts. Set it to false to prevent scripts from reading texture data.
		bool					m_isReadable    = false;
		
		// Select this to enable mip-map generation. Mip maps are smaller versions of the Texture that get used when the Texture is very small on screen.
		bool					m_mipmapEnabled = true;
	};
}

#endif /* TextureImporter_hpp */
