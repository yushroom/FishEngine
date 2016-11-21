#include "Texture.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    std::vector<TexturePtr> Texture::m_textures;
    
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_texture);
    }
}
