#include "Texture.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    std::vector<TexturePtr> Texture::m_textures;
    
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_texture);
    }

    FishEngine::TexturePtr Texture::Create()
    {
        auto t = std::make_shared<Texture>();
        m_textures.push_back(t);
        return t;
    }
}
