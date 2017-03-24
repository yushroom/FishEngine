#include "Texture.hpp"
#include "GLEnvironment.hpp"
#include "Debug.hpp"
#include "Mathf.hpp"

#include <cassert>

namespace FishEngine
{
	void Texture::BindSampler()
	{
		assert(m_GLNativeTexture != 0);
		const auto& sampler = TextureSampler::GetSampler(m_filterMode, m_wrapMode);
		glBindSampler(m_GLNativeTexture, sampler.m_nativeGLSampler);
	}

	std::vector<TexturePtr> Texture::s_textures;

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_GLNativeTexture);
	}

	FishEngine::TexturePtr Texture::Create()
	{
		auto t = std::make_shared<Texture>();
		s_textures.push_back(t);
		return t;
	}
}
