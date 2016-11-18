#ifndef RenderSettings_hpp
#define RenderSettings_hpp

#include "Object.hpp"
#include "RenderSystem.hpp"

namespace FishEngine
{
    class RenderSettings : public Object
    {
    public:
        //InjectClassName(RenderSettings);

        RenderSettings() = delete;

        static MaterialPtr skybox()
        {
            return m_skybox;
        }

        static void setSkybox(MaterialPtr& skybox)
        {
            m_skybox = skybox;
        }

    private:

        // The global skybox to use.
        static MaterialPtr m_skybox;
    };
}

#endif // RenderSettings_hpp
