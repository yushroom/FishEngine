#ifndef Skybox_hpp
#define Skybox_hpp

#include "FishEngine.hpp"
#include "Behaviour.hpp"

namespace FishEngine
{
    // A script interface for the skybox component.
    // see: https://docs.unity3d.com/ScriptReference/Skybox.html
    // see: https://docs.unity3d.com/Manual/class-Skybox.html
    class Skybox : public Behaviour
    {
    public:
        PMaterial material()
        {
            return m_material;
        }

        void setMaterial(PMaterial material)
        {
            m_material = material;
        }

    private:

        // The material used by the skybox.
        PMaterial m_material;
    };
}

#endif // Skybox_hpp