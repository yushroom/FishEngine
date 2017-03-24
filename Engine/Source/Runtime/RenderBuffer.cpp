#include "RenderBuffer.hpp"

#include <Debug.hpp>
#include <cassert>

using namespace FishEngine;

std::shared_ptr<ColorBuffer> ColorBuffer::Create(const int width, const int height, TextureFormat  format /*= TextureFormat::RGBA32*/)
{
	auto t = std::make_shared<ColorBuffer>();
	Texture::s_textures.push_back(t);
	t->m_dimension = TextureDimension::Tex2D;
	t->m_format = format;
	t->m_width = width;
	t->m_height = height;
	glGenTextures(1, &t->m_GLNativeTexture);
	assert(t->m_GLNativeTexture > 0);
	glBindTexture(GL_TEXTURE_2D, t->m_GLNativeTexture);
	GLenum internal_format, external_format, pixel_type;
	TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, t->m_width, t->m_height, 0, external_format, pixel_type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glCheckError();
	t->m_uploaded = true;
	return t;
}


void ColorBuffer::Resize(const int newWidth, const int newHeight)
{
	Debug::Log("ColorBuffer::Resize");
	if (newWidth == m_width && newHeight == m_height)
		return;
	m_width = newWidth;
	m_height = newHeight;
	GLenum internal_format, external_format, pixel_type;
	TextureFormat2GLFormat(m_format, internal_format, external_format, pixel_type);
	glBindTexture(GL_TEXTURE_2D, m_GLNativeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, external_format, pixel_type, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glCheckError();
}

std::shared_ptr<DepthBuffer> DepthBuffer::Create(const int width, const int height)
{
	auto t = std::make_shared<DepthBuffer>();
	Texture::s_textures.push_back(t);
	t->m_dimension = TextureDimension::Tex2D;
	t->m_width = width;
	t->m_height = height;
	glGenTextures(1, &t->m_GLNativeTexture);
	glBindTexture(GL_TEXTURE_2D, t->m_GLNativeTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rt->m_width, rt->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, t->m_width, t->m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glCheckError();
	t->m_uploaded = true;
	return t;
}

void DepthBuffer::Resize(const int newWidth, const int newHeight)
{
	Debug::Log("DepthBuffer::Resize");
	if (newWidth == m_width && newHeight == m_height)
		return;
	m_width = newWidth;
	m_height = newHeight;
	glBindTexture(GL_TEXTURE_2D, m_GLNativeTexture);
	if (m_useStencil)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glCheckError();
}

//std::shared_ptr<LayeredColorBuffer> LayeredColorBuffer::Create(const int width, const int height, const int layers, TextureFormat format /*= TextureFormat::RGBA32*/)
//{
//    auto t = std::make_shared<LayeredColorBuffer>();
//    Texture::m_textures.push_back(t);
//    t->m_dimension = TextureDimension::Tex2DArray;
//    t->m_filterMode = FilterMode::Point;
//    t->m_wrapMode = TextureWrapMode::Clamp;
//    t->m_width = width;
//    t->m_height = height;
//    t->m_format = format;
//    t->m_layers = layers;

//    glGenTextures(1, &t->m_GLNativeTexture);
//    glBindTexture(GL_TEXTURE_2D_ARRAY, t->m_GLNativeTexture);
//    GLenum internal_format, external_format, pixel_type;
//    TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);
//    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, t->m_width, t->m_height, layers, 0, external_format, pixel_type, NULL);
//    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
//    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
//    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
//    glCheckError();
//    return t;
//}

std::shared_ptr<LayeredDepthBuffer> LayeredDepthBuffer::Create(const int width, const int height, const int depth, bool useStencil /*= true*/)
{
	auto t = std::make_shared<LayeredDepthBuffer>();
	Texture::s_textures.push_back(t);
	t->m_useStencil = useStencil;
	t->m_dimension = TextureDimension::Tex2DArray;
	t->m_width = width;
	t->m_height = height;
	//t->m_format = format;
	t->m_depth = depth;
	t->m_filterMode = FilterMode::Point;
	t->m_wrapMode = TextureWrapMode::Clamp;
	
	glGenTextures(1, &t->m_GLNativeTexture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, t->m_GLNativeTexture);
	if (useStencil)
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH24_STENCIL8, t->m_width, t->m_height, depth, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	else
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, t->m_width, t->m_height, depth, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	//if (useStencil)
	//    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH24_STENCIL8, width, height, depth);
	//else
	//    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32, width, height, depth);
	glCheckError();
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glCheckError();
	t->m_uploaded = true;
	return t;
}
