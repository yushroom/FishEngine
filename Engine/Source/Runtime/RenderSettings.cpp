#include "RenderSettings.hpp"
#include "Material.hpp"

namespace FishEngine
{
    MaterialPtr RenderSettings::m_skybox;
    TexturePtr RenderSettings::m_ambientCubemap;

    void RenderSettings::setSkybox(MaterialPtr& skybox)
    {
        m_skybox = skybox;
        m_skybox->DisableKeyword(ShaderKeyword::All);
    }
}

