#ifndef Texture_hpp
#define Texture_hpp

#include "Object.hpp"
//#include "GLEnvironment.hpp"

namespace FishEngine
{
    // Filtering mode for textures.
    enum class FilterMode
    {
        Point,      // Point filtering - texture pixels become blocky up close.
        Bilinear,   // Bilinear filtering - texture samples are averaged.
        Trilinear,  // Trilinear filtering - texture samples are averaged and also blended between mipmap levels.
    };

    // Wrap mode for textures.
    enum class TextureWrapMode
    {
        Repeat,     // Tiles the texture, creating a repeating pattern.
        Clamp,      // Clamps the texture to the last pixel at the border.
    };
    
    enum class TextureDimension
    {
        Unknown,    // Texture type is not initialized or unknown.
        None,       // No texture is assigned.
        Tex2D,      // 2D texture (Texture2D).
        Tex3D,      // 3D volume texture (Texture3D).
        Cube,       // Cubemap texture.
        Tex2DArray, // 2D array texture (Texture2DArray).
        Any,        // Any texture type.
    };

    class FE_EXPORT Texture : public Object
    {
    public:
        Texture()                       = default;
        Texture(const Texture&)         = delete;
        void operator=(const Texture&)  = delete;
        
        virtual ~Texture();
        
        uint32_t width() const
        {
            return m_width;
        }
        
        uint32_t height() const
        {
            return m_height;
        }
        
        unsigned int GetNativeTexturePtr() const
        {
            return m_texture;
        }

        // Filtering mode of the texture.
        FilterMode filterMode() const
        {
            return m_filterMode;
        }

        void setFilterMode(const FilterMode filterMode)
        {
            m_filterMode = filterMode;
        }

        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode wrapMode() const
        {
            return m_wrapMode;
        }

        void setWrapMode(const TextureWrapMode wrapMode)
        {
            m_wrapMode = wrapMode;
        }

        TextureDimension dimension() const
        {
            return m_dimension;
        }

        void setDimension(TextureDimension dimension)
        {
            m_dimension = dimension;
        }
        
        static const std::vector<TexturePtr>& AllTextures()
        {
            return m_textures;
        }


    protected:
        
        uint32_t m_height;
        uint32_t m_width;
        
        // Anisotropic filtering level of the texture.
        int m_anisoLevel;
        
        // Dimensionality (type) of the texture (Read Only).
        TextureDimension m_dimension;
        
        // Filtering mode of the texture.
        FilterMode m_filterMode = FilterMode::Trilinear;
        
        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode m_wrapMode = TextureWrapMode::Repeat;
        
        //OpenGL
        unsigned int m_texture = 0;

    private:
        friend class TextureImporter;
        
        static std::vector<TexturePtr> m_textures;
    };
    
    enum class TextureFormat
    {
        RGB32,
        RGBA32,
        ARGB32,
        R16,
        DXT1,
        DXT5,
    };
    
    class Texture2D : public Texture
    {
    public:
    private:
        TextureFormat m_format;
        uint32_t m_mipmapCount;
    };
}

#endif /* Texture_hpp */
