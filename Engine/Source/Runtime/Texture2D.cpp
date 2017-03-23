#include "Texture2D.hpp"
#include "GLEnvironment.hpp"
#include "Debug.hpp"
#include "Mathf.hpp"

namespace FishEngine
{

	Texture2D::Texture2D(int width, int height, TextureFormat format, uint8_t* data, int byteCount)
	{
		m_width = width;
		m_height = height;
		m_format = format;
		m_data.resize(byteCount);
		std::copy(data, data + byteCount, m_data.begin());
	}

	void Texture2D::UploadToGPU() const
	{
		if (m_uploaded)
			return;

		GLenum internal_format = GL_RGBA8;
		GLenum format = GL_RGBA;
		switch (m_format)
		{
		case TextureFormat::R8:
			internal_format = GL_R8;
			format = GL_RED;
			break;
		case TextureFormat::RG16:
			internal_format = GL_RG8;
			format = GL_RG;
			break;
		case TextureFormat::RGB24:
			internal_format = GL_RGB8;
			format = GL_RGB;
			break;
		case TextureFormat::RGBA32:
			internal_format = GL_RGBA8;
			format = GL_RGBA;
			break;
		case TextureFormat::BGRA32:
			internal_format = GL_RGBA8;
			format = GL_BGRA;
			break;
		default:
			abort();
		}

		glCheckError();

		glGenTextures(1, &m_GLNativeTexture);
		glCheckError();
		glBindTexture(GL_TEXTURE_2D, m_GLNativeTexture);
		glCheckError();
#if 1
		GLsizei max_mipmap_level_count = Mathf::FloorToInt(std::log2f((float)std::max(m_width, m_height))) + 1;
		glCheckError();
		glTexStorage2D(GL_TEXTURE_2D, max_mipmap_level_count, internal_format, m_width, m_height);
		glCheckError();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, m_data.data());
#else
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
#endif
		glCheckError();
		glGenerateMipmap(GL_TEXTURE_2D);
		glCheckError();
		// Parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glCheckError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glCheckError();
		glBindTexture(GL_TEXTURE_2D, 0);
		m_uploaded = true;
		glCheckError();
	}
}
