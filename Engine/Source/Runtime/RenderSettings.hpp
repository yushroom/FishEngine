#ifndef RenderSettings_hpp
#define RenderSettings_hpp

#include "Object.hpp"
#include "RenderSystem.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
    class FE_EXPORT Meta(NonSerializable) RenderSettings : public Object
    {
    public:
        //InjectClassName(RenderSettings);

        RenderSettings() = delete;

        static MaterialPtr skybox()
        {
            return m_skybox;
        }

        static void setSkybox(MaterialPtr& skybox);

        static TexturePtr ambientCubemap()
        {
            return m_ambientCubemap;
        }

        static void setAmbientCubemap(TexturePtr ambientCubemap)
        {
            m_ambientCubemap = ambientCubemap;
        }
		
		static LayeredDepthBufferPtr defaultShadowMap()
		{
			static LayeredDepthBufferPtr m_shadowMap = nullptr;
			if (m_shadowMap == nullptr)
			{
				m_shadowMap = LayeredDepthBuffer::Create(32, 32, 4, false);
				//m_tempColorBuffer = LayeredColorBuffer::Create(shadow_map_size, shadow_map_size, 4, TextureFormat::R32);
				m_shadowMap->setFilterMode(FilterMode::Bilinear);
				m_shadowMap->setWrapMode(TextureWrapMode::Clamp);
			}
			return m_shadowMap;
		}
		
        static TexturePtr preintegratedGF();

    private:

        // The global skybox to use.
        static MaterialPtr m_skybox;
        static TexturePtr m_ambientCubemap;
		
		
    };
}

#endif // RenderSettings_hpp
