#ifndef Texture_hpp
#define Texture_hpp

#include "Object.hpp"
#include "TextureSampler.hpp"
#include "TextureProperty.hpp"
#include "ReflectClass.hpp"
#include <cstdlib>

namespace FishEngine
{
	class FE_EXPORT Texture : public Object
	{
	public:
		InjectClassName(Texture);

		Texture() = default;
		Texture(const Texture&) = delete;
		void operator=(const Texture&) = delete;

		virtual ~Texture();

		static TexturePtr Create();

		uint32_t width() const
		{
			return m_width;
		}

		uint32_t height() const
		{
			return m_height;
		}

		unsigned int GetNativeTexturePtr() const
		{
			if (!m_uploaded)
				UploadToGPU();
			if (m_GLNativeTexture == 0) abort();
			return m_GLNativeTexture;
		}

		// Filtering mode of the texture.
		FilterMode filterMode() const
		{
			return m_filterMode;
		}

		void setFilterMode(const FilterMode filterMode)
		{
			m_filterMode = filterMode;
			BindSampler();
		}

		// Wrap mode (Repeat or Clamp) of the texture.
		TextureWrapMode wrapMode() const
		{
			return m_wrapMode;
		}

		void setWrapMode(const TextureWrapMode wrapMode)
		{
			m_wrapMode = wrapMode;
			BindSampler();
		}

		TextureDimension dimension() const
		{
			return m_dimension;
		}

		void setDimension(TextureDimension dimension)
		{
			m_dimension = dimension;
		}

		static const std::vector<TexturePtr>& AllTextures()
		{
			return s_textures;
		}

		virtual void Resize(const int, const int)
		{
			abort();
		}

		//std::vector<std::uint8_t> const & rawdata() { return m_data; };

	protected:
		uint32_t            m_height;
		uint32_t            m_width;

		// Anisotropic filtering level of the texture.
		int                 m_anisoLevel;

		// Dimensionality (type) of the texture (Read Only).
		TextureDimension    m_dimension = TextureDimension::Unknown;

		// Filtering mode of the texture.
		FilterMode          m_filterMode = FilterMode::Trilinear;

		// Wrap mode (Repeat or Clamp) of the texture.
		TextureWrapMode     m_wrapMode = TextureWrapMode::Repeat;

		// OpenGL
		Meta(NonSerializable)
		mutable unsigned int        m_GLNativeTexture = 0;

		void BindSampler();

		static std::vector<TexturePtr> s_textures;

		virtual void UploadToGPU() const { m_uploaded = true; };

		Meta(NonSerializable)
		mutable bool m_uploaded = false;

	private:
		friend class FishEditor::TextureImporter;
	};
}

#endif /* Texture_hpp */
