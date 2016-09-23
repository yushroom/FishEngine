#ifndef RenderTexture_hpp
#define RenderTexture_hpp

#include "GLEnvironment.hpp"
#include "Texture.hpp"

namespace FishEngine {
    
    enum class RenderTextureFormat
    {
        ARGB32, // 4 channel, 8bie per channel
        Dpeth,  //
        Shadowmap,
    };
    
    class RenderTexture : public Texture
    {
    public:
        RenderTexture() = default;
        
        static std::shared_ptr<RenderTexture> CreateShadowMap();
        
        int width() const {
            return m_width;
        }
        
        int height() const {
            return m_height;
        }
        
        GLuint depthBufferFBO() const {
            return m_depthMapFBO;
        }
        
    private:
        RenderTextureFormat m_format;
        int m_width;
        int m_height;
        GLuint m_depthMapFBO;
    };
}

#endif /* RenderTexture_hpp */
