#ifndef Texture_hpp
#define Texture_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Texture : public Object
{
public:
    Texture() {}
    //Texture(GLuint texture) : m_texture(texture) {};
    Texture(const std::string& path);
    Texture(const Texture&) = delete;
    void operator=(const Texture&) = delete;
    virtual ~Texture();

    typedef std::shared_ptr<Texture> PTexture;
    
    //static Texture& GetSimpleTexutreCubeMap();

    static PTexture CreateFromFile(const std::string& path);

    void FromFile(const std::string& path);
    
    GLuint GLTexuture() const {
        return m_texture;
    }
    
protected:
    GLuint m_texture = 0;
};

NAMESPACE_FISHENGINE_END

#endif /* Texture_hpp */
