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
        
        static TexturePtr Create();

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

        virtual void Resize(const int newWidth, const int newHeight)
        {
            abort();
        }

    protected:
        
        uint32_t m_height;
        uint32_t m_width;
        
        // Anisotropic filtering level of the texture.
        int m_anisoLevel;
        
        // Dimensionality (type) of the texture (Read Only).
        TextureDimension m_dimension = TextureDimension::Unknown;
        
        // Filtering mode of the texture.
        FilterMode m_filterMode = FilterMode::Trilinear;
        
        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode m_wrapMode = TextureWrapMode::Repeat;
        
        //OpenGL
        unsigned int m_texture = 0;

        static std::vector<TexturePtr> m_textures;

    private:
        friend class TextureImporter;
    };
    
    enum class TextureFormat
    {
        RGB32,
        RGBA32, // Color with alpha texture format, 8-bits per channel.
        ARGB32, // Color with an alpha channel texture format.
        R16,    // A 16 bit color texture format that only has a red channel.
        DXT1,   // Compressed color texture format.
        DXT5,   // Compressed color with alpha channel texture format.
        RG8,    // **New**, uint8_t * 2
        RG16,   // **New**, uint16_t * 2
        RGHalf,	// Two color (RG) texture format, 16 bit floating point per channel.
        RGFloat	// Two color (RG) texture format, 32 bit floating point per channel.
    };

    class ColorBuffer : public Texture
    {
    public:
        static std::shared_ptr<ColorBuffer> Create(const int width, const int height);
        virtual void Resize(const int newWidth, const int newHeight) override;
    };

    class DepthBuffer : public Texture
    {
    public:
        static std::shared_ptr<DepthBuffer> Create(const int width, const int height);
        virtual void Resize(const int newWidth, const int newHeight) override;
    };
    
    class Texture2D : public Texture
    {
    public:
        
        // The format of the pixel data in the texture (Read Only).
        TextureFormat format() const
        {
            return m_format;
        }
        
        // How many mipmap levels are in this texture (Read Only).
        uint32_t mipmapCount() const
        {
            return m_mipmapCount;
        }

    private:
        
        // The format of the pixel data in the texture (Read Only).
        TextureFormat m_format;
        
        // How many mipmap levels are in this texture (Read Only).
        uint32_t m_mipmapCount;
        
        static Texture2DPtr m_blackTexture;
        static Texture2DPtr m_whiteTexture;
    };
    
    enum class CubemapFace
    {
        Unknown,
        PositiveX = 0,
        NegativeX = 1,
        PositiveY = 2,
        NegativeY = 3,
        PositiveZ = 4,
        NegativeZ = 5,
    };

    class Cubemap : public Texture
    {
    public:
        
        // The format of the pixel data in the texture (Read Only).
        TextureFormat format() const
        {
            return m_format;
        }
        
        
        // How many mipmap levels are in this texture (Read Only).
        uint32_t mipmapCount() const
        {
            return m_mipmapCount;
        }
        
    private:
        
        // The format of the pixel data in the texture (Read Only).
        TextureFormat m_format;
        uint32_t m_mipmapCount;
    };
}

#endif /* Texture_hpp */
