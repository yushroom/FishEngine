#ifndef RenderSettings_hpp
#define RenderSettings_hpp

#include "Script.hpp"
#include "RenderSystem.hpp"
#include <imgui/imgui.h>

NAMESPACE_FISHENGINE_BEGIN

class RenderSettings : public Script
{
public:
    InjectClassName(RenderSettings);
};

NAMESPACE_FISHENGINE_END

#endif // RenderSettings_hpp
