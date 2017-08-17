#include <FishEngine/TextureSampler.hpp>

namespace FishEngine
{
	FishEngine::TextureSampler TextureSampler::s_samplers[6];

	TextureSampler::~TextureSampler()
	{
		if (m_nativeGLSampler != 0)
			glDeleteSamplers(1, &m_nativeGLSampler);
	}

	void TextureSampler::Init()
	{
		// point, repeat
		int idx = 0;
		glGenSamplers(1, &s_samplers[0].m_nativeGLSampler);
		glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// point, clamp
		idx++;
		glGenSamplers(1, &s_samplers[1].m_nativeGLSampler);
		glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// bilinear, repeat
		idx++;
		glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		// bilinear, clamp
		idx++;
		glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		// trilinear, repeat
		idx++;
		glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// trilinear, clamp
		idx++;
		glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

}