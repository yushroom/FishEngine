#pragma once

#include "Texture.hpp"

namespace FishEngine
{
	class FE_EXPORT Cubemap : public Texture
	{
	public:

		InjectClassName(Cubemap);

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

	private:
		// The format of the pixel data in the texture (Read Only).
		TextureFormat m_format;
		uint32_t m_mipmapCount;
	};
}