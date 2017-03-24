#pragma once

#include "Texture.hpp"
#include <array>

namespace FishEngine
{
	class FE_EXPORT Cubemap : public Texture
	{
		friend FishEditor::DDSImporter;
		
	public:

		InjectClassName(Cubemap);
		
		Cubemap(int size, TextureFormat format, bool mipmap)
		{
			m_width = size;
			m_height = size;
			m_format = format;
			//m_mipmapCount = mipmap ? 10 : 1;
			m_mipmapCount = 1;
		}
		
		void SetPixel(CubemapFace face, int x, int y, Color color);
		
		Color GetPixel(CubemapFace face, int x, int y);
		
		std::vector<Color> GetPixels(CubemapFace face, int miplevel = 0);
		void SetPixels(std::vector<Color> const & colors, CubemapFace face, int miplevel = 0);
		
		void Apply(bool updateMipmaps = true, bool makeNoLongerReadable = false);
		

		// The format of the pixel data in the texture (Read Only).
		TextureFormat format() const
		{
			return m_format;
		}

		// How many mipmap levels are in this texture (Read Only).
		// The returned value includes the base level as well, so it is always 1 or more.
		uint32_t mipmapCount() const
		{
			return m_mipmapCount;
		}
		
	protected:
		virtual void UploadToGPU() const override;

	private:
		// The format of the pixel data in the texture (Read Only).
		TextureFormat m_format;
		uint32_t m_mipmapCount;
		
		//std::vector<std::uint8_t> m_data;
		// face->mipmap
		std::array<std::vector<std::vector<std::uint8_t>>, 6> m_pixels;
	};
}
