#ifndef Texture_hpp
#define Texture_hpp

#include "Object.hpp"
#include "TextureSampler.hpp"
#include "TextureProperty.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
    class FE_EXPORT Texture : public Object
    {
    public:
        InjectClassName(Texture)

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
            return s_textures;
        }

		virtual void Resize(const int, const int)
        {
            abort();
        }

    protected:
        uint32_t            m_height;
        uint32_t            m_width;
        
        // Anisotropic filtering level of the texture.
        int                 m_anisoLevel;
        
        // Dimensionality (type) of the texture (Read Only).
        TextureDimension    m_dimension     = TextureDimension::Unknown;
        
        // Filtering mode of the texture.
        FilterMode          m_filterMode    = FilterMode::Trilinear;
        
        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode     m_wrapMode      = TextureWrapMode::Repeat;
        
        // OpenGL
		Meta(NonSerializable)
        unsigned int        m_GLNativeTexture = 0;

        void BindSampler();

        static std::vector<TexturePtr> s_textures;

    private:
        friend class TextureImporter;
    };

    class Meta(NonSerializable) ColorBuffer : public Texture
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

    class Meta(NonSerializable) DepthBuffer : public Texture
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
        
        InjectClassName(Texture2D)

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

    class Cubemap : public Texture
    {
    public:
        
        InjectClassName(Cubemap)

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
