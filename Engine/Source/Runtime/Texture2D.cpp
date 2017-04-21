#include "Texture2D.hpp"
#include "GLEnvironment.hpp"
#include "Debug.hpp"
#include "Mathf.hpp"

namespace FishEngine
{

	Texture2D::Texture2D(int width, int height, TextureFormat format, const uint8_t* data, int byteCount /* = -1 */)
	{
		if (width <= 0 || height <= 0)
		{
			abort();
		}

		int bpp = BytePerPixel(format);
		if (bpp <= 0 && byteCount <= 0)
		{
			abort();
		}
		if (bpp > 0 && byteCount > 0 && bpp * width * height != byteCount)
		{
			abort();
		}

		m_width = width;
		m_height = height;
		m_format = format;
		m_data.resize(byteCount);
		std::copy(data, data + byteCount, m_data.begin());
	}


	void Texture2D::UploadToGPU()
	{
		if (m_uploaded)
			return;

		GLenum internal_format = GL_RGBA8;
		GLenum format = GL_RGBA;
		GLenum type = GL_UNSIGNED_INT;

		TextureFormat2GLFormat(m_format, &internal_format, &format, &type);

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
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, type, m_data.data());
#else
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);
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
		m_data.clear();
		m_data.shrink_to_fit();
		glCheckError();
	}

	const uint8_t allWhite[] = {
		255,255,255,255,
		255,255,255,255,
		255,255,255,255,
		255,255,255,255,
	};

	const uint8_t allBlack[] = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
	};

	FishEngine::Texture2DPtr Texture2D::whiteTexture()
	{
		static Texture2DPtr m_whiteTexture = MakeShared<Texture2D>(2, 2, TextureFormat::RGBA32, allWhite, 2 * 2 * 4);
		return m_whiteTexture;
	}

	FishEngine::Texture2DPtr Texture2D::blackTexture()
	{
		static Texture2DPtr m_blackTexture = MakeShared<Texture2D>(2, 2, TextureFormat::RGBA32, allBlack, 2 * 2 * 4);
		return m_blackTexture;
	}

}
