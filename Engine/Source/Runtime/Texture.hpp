#ifndef Texture_hpp
#define Texture_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    // Filtering mode for textures.
    enum class FilterMode
    {
        Point       = 0,    // Point filtering - texture pixels become blocky up close.
        Bilinear    = 1,    // Bilinear filtering - texture samples are averaged.
        Trilinear   = 2,    // Trilinear filtering - texture samples are averaged and also blended between mipmap levels.
    };

    // Wrap mode for textures.
    enum class TextureWrapMode
    {
        Repeat      = 0,    // Tiles the texture, creating a repeating pattern.
        Clamp       = 1,    // Clamps the texture to the last pixel at the border.
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


    class TextureSampler
    {
    public:
        TextureSampler() = default;
        TextureSampler(const TextureSampler&) = delete;
        TextureSampler& operator=(const TextureSampler&) = delete;
        ~TextureSampler();

        static void Init();

        static const TextureSampler& GetSampler(FilterMode filterMode, TextureWrapMode wrapMode);

    private:
        friend class Texture;
        unsigned int m_nativeGLSampler = 0;

        static TextureSampler s_samplers[6];
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
            return m_GLNativeTexture;
        }

        // Filtering mode of the texture.
        FilterMode filterMode() const
        {
            return m_filterMode;
        }

        void setFilterMode(const FilterMode filterMode)
        {
            m_filterMode = filterMode;
            BindSampler();
        }

        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode wrapMode() const
        {
            return m_wrapMode;
        }

        void setWrapMode(const TextureWrapMode wrapMode)
        {
            m_wrapMode = wrapMode;
            BindSampler();
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
        unsigned int m_GLNativeTexture = 0;

        void BindSampler();

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
        R8,
        R32,    // **New**, float * 1
        RG8,    // **New**, uint8_t * 2
        RG16,   // **New**, uint16_t * 2
        RGHalf,	// Two color (RG) texture format, 16 bit floating point per channel.
        RGFloat	// Two color (RG) texture format, 32 bit floating point per channel.
    };

    static void TextureFormat2GLFormat(
        TextureFormat format,
        GLenum& out_internalFormat,
        GLenum& out_externalFormat,
        GLenum& out_pixelType)
    {
        switch (format)
        {
        case TextureFormat::RGBA32:
            out_internalFormat = GL_RGBA8;
            out_externalFormat = GL_RGBA;
            out_pixelType = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::RG16:
            out_internalFormat = GL_RG16;
            out_externalFormat = GL_RG;
            out_pixelType = GL_UNSIGNED_SHORT;
            break;
        case TextureFormat::RG8:
            out_internalFormat = GL_RG8;
            out_externalFormat = GL_RG;
            out_pixelType = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::RGFloat:
            out_internalFormat = GL_RG32F;
            out_externalFormat = GL_RG;
            out_pixelType = GL_FLOAT;
            break;
        case TextureFormat::R32:
            out_internalFormat = GL_R32F;
            out_externalFormat = GL_RED;
            out_pixelType = GL_FLOAT;
            break;
        case TextureFormat::R8:
            out_internalFormat = GL_R8;
            out_externalFormat = GL_RED;
            out_pixelType = GL_UNSIGNED_BYTE;
            break;
        default:
            //Debug::LogError("Unknown texture format");
            abort();
        }
    }

    class ColorBuffer : public Texture
    {
    public:
        static std::shared_ptr<ColorBuffer> Create(const int width, const int height, TextureFormat format = TextureFormat::RGBA32);
        virtual void Resize(const int newWidth, const int newHeight) override;
    protected:
        TextureFormat m_format;
    };

    //class LayeredColorBuffer : public ColorBuffer
    //{
    //public:
    //    static std::shared_ptr<LayeredColorBuffer> Create(const int width, const int height, const int layers, TextureFormat format = TextureFormat::RGBA32);
    //    //virtual void Resize(const int newWidth, const int newHeight) override;
    //protected:
    //    int m_layers;
    //};

    class DepthBuffer : public Texture
    {
    public:
        static std::shared_ptr<DepthBuffer> Create(const int width, const int height);
        virtual void Resize(const int newWidth, const int newHeight) override;
        bool m_useStencil = true;
    };

    class LayeredDepthBuffer : public DepthBuffer
    {
    public:
        static std::shared_ptr<LayeredDepthBuffer> Create(const int width, const int height, const int depth, bool useStencil = true);
        //virtual void Resize(const int newWidth, const int newHeight) override;
    protected:
        int m_depth;
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
