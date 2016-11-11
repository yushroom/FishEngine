#ifndef Texture_hpp
#define Texture_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    // Select this to set basic parameters depending on the purpose of your texture.
    enum class TextureTypeImporterType
    {
        Image,      // This is the most common setting used for all the textures in general.
        Bump,       // Select this to turn the color channels into a format suitable for real-time normal mapping.
        GUI,        // Use this if your texture is going to be used on any HUD/GUI Controls.
        Sprite,     // Select this if you will be using your texture for Sprite graphics.
        Cursor,     // Use this if your texture is going to be used as a cursor.
        Cubemap,    // This converts your texture into Cubemap suitable for Skyboxes, Environment Reflections or Image Based Lighting (IBL).
        Cookie,     // This sets up your texture with the basic parameters used for the Cookies of your lights.
        Lightmap,   // This sets up your texture with the parameters used by the lightmap.
        Advance,    // Select this when you want to have specific parameters on your texture and you want to have total control over your texture.
    };

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

    class Texture : public Object
    {
    public:
        Texture() {}
        //Texture(GLuint texture) : m_texture(texture) {};
        Texture(const std::string& path);
        Texture(const Texture&) = delete;
        void operator=(const Texture&) = delete;
        virtual ~Texture();

        //static Texture& GetSimpleTexutreCubeMap();

        static PTexture CreateFromFile(const std::string& path);

        void FromFile(const std::string& path);

        GLuint GLTexuture() const {
            return m_texture;
        }

        // Which type of texture are we dealing with here.
        TextureTypeImporterType textureType() const
        {
            return m_textureType;
        }

        void setTextureType(const TextureTypeImporterType textureType)
        {
            m_textureType = textureType;
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

    protected:
        GLuint m_texture = 0;

        // Which type of texture are we dealing with here.
        TextureTypeImporterType m_textureType = TextureTypeImporterType::Image;

        // Filtering mode of the texture.
        FilterMode m_filterMode = FilterMode::Trilinear;

        // Wrap mode (Repeat or Clamp) of the texture.
        TextureWrapMode m_wrapMode = TextureWrapMode::Repeat;
    };
}

#endif /* Texture_hpp */
