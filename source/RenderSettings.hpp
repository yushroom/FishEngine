#ifndef RenderSettings_hpp
#define RenderSettings_hpp

#include "Script.hpp"
#include "RenderSystem.hpp"
#include <imgui/imgui.h>

namespace FishEngine
{
    class RenderSettings : public Script
    {
    public:
        InjectClassName(RenderSettings);
    };
}

#endif // RenderSettings_hpp
