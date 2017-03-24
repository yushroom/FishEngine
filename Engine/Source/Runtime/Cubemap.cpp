#include "Cubemap.hpp"

void FishEngine::Cubemap::UploadToGPU() const
{
	if (m_uploaded)
		return;
	
	GLenum internal_format = GL_RGBA8;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_INT;
	switch (m_format)
	{
		case TextureFormat::RGBAHalf:
			internal_format = GL_RGBA16F;
			format = GL_RGBA;
			type = GL_HALF_FLOAT;
			break;
		default:
			abort();
	}
	
	glCheckError();
	
	glGenTextures(1, &m_GLNativeTexture);
	glCheckError();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLNativeTexture);
	glCheckError();

	
	//GLsizei max_mipmap_level_count = Mathf::FloorToInt(std::log2f((float)std::max(m_width, m_height))) + 1;
	glCheckError();
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, m_mipmapCount, internal_format, m_width, m_height);
	
	for (int face = 0; face < 6; ++face)
	{
		auto target = static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);
		int size = m_width;
		for (int level = 0; level < m_mipmapCount; ++level)
		{
			glTexSubImage2D(target, level, 0, 0, size, size, format, type, m_pixels[face][level].data());
			size /= 2;
		}
	}
	glCheckError();
	//glGenerateMipmap(GL_TEXTURE_2D);
	glCheckError();
	// Parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glCheckError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glCheckError();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	m_uploaded = true;
	glCheckError();
}
