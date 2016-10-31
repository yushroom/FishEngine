#include "Resources.hpp"

namespace FishEngine
{
#if FISHENGINE_PLATFORM_WINDOWS
    std::string Resources::m_shaderRootDirectory = "D:/program/FishEngine/Engine/Shaders/";
#else
std::string Resources::m_shaderRootDirectory = "../Shaders/";
#endif
std::string Resources::m_textureRootDirectory;
}
