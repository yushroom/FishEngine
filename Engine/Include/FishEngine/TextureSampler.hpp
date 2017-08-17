#pragma once

#include "FishEngine.hpp"
#include "TextureProperty.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
	class Meta(NonSerializable) TextureSampler
	{
	public:
		TextureSampler() = default;
		TextureSampler(const TextureSampler&) = delete;
		TextureSampler& operator=(const TextureSampler&) = delete;
		~TextureSampler();

		static void Init();

		static const TextureSampler& GetSampler(FilterMode filterMode, TextureWrapMode wrapMode);

	private:
		friend class Texture;
		unsigned int m_nativeGLSampler = 0;

		static TextureSampler s_samplers[6];
	};


	inline const TextureSampler& TextureSampler::GetSampler(FilterMode filterMode, TextureWrapMode wrapMode)
	{
		auto idx = static_cast<int>(filterMode) * 3 + static_cast<int>(wrapMode);
		return s_samplers[idx];
	}
}