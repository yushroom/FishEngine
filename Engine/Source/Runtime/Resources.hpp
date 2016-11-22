#ifndef Resources_hpp
#define Resources_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    class FE_EXPORT Resources
    {
    public:
        Resources() = delete;

        //static Object Load(const std::string path);

        static const std::string& shaderRootDirectory()
        {
            return m_shaderRootDirectory;
        }

    private:
        static std::list<std::string> m_rootAssetDirectorys;

        static std::string m_shaderRootDirectory;
        static std::string m_textureRootDirectory;
    };
}

#endif // Resources_hpp
