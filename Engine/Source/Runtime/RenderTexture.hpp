#ifndef RenderTexture_hpp
#define RenderTexture_hpp

#include "GLEnvironment.hpp"
#include "Texture.hpp"

namespace FishEngine {
    
    enum class RenderTextureFormat
    {
        ARGB32, // 4 channel, 8 bit per channel
        Dpeth,  //
        Shadowmap,
    };
    
    class RenderTexture : public Texture
    {
    public:
        RenderTexture() = default;
        ~RenderTexture();
        
        static PRenderTexture CreateShadowMap();

        static PRenderTexture CreateColorMap(const int width, const int height);

        int width() const
        {
            return m_width;
        }
        
        int height() const
        {
            return m_height;
        }
        
        GLuint FBO() const
        {
            return m_FBO;
        }

        GLuint depthBuffer() const
        {
            return m_depthBuffer;
        }

        GLuint colorBuffer() const
        {
            return m_texture;
        }
        
        void Resize(const int newWidth, const int newHeight);

    private:
        RenderTextureFormat m_format;
        int m_width;
        int m_height;
        GLuint m_FBO = 0;
        GLuint m_depthBuffer = 0;
    };
}

#endif /* RenderTexture_hpp */
