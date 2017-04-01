#pragma once

#include "Texture.hpp"

namespace FishEngine
{
	class FE_EXPORT Texture2D : public Texture
	{
	public:

		InjectClassName(Texture2D);

		Texture2D() = default;

		Texture2D(int width, int height, TextureFormat format, const uint8_t* data, int byteCount = -1);

		// The format of the pixel data in the texture (Read Only).
		TextureFormat format() const
		{
			return m_format;
		}

		// How many mipmap levels are in this texture (Read Only).
		uint32_t mipmapCount() const
		{
			return m_mipmapCount;
		}

		// Get a small texture with all white pixels.
		static Texture2DPtr whiteTexture();

		static Texture2DPtr blackTexture();
		

	protected:

		virtual void UploadToGPU() override;

	protected:

		friend class FishEditor::TextureImporter;
		friend class FishEditor::DDSImporter;

		Meta(NonSerializable)
		std::vector<std::uint8_t> m_data;
		
		// The format of the pixel data in the texture (Read Only).
		TextureFormat m_format;

		// How many mipmap levels are in this texture (Read Only).
		uint32_t m_mipmapCount;

		
	};
}
