#ifndef RenderSettings_hpp
#define RenderSettings_hpp

#include "Behaviour.hpp"
#include "RenderSystem.hpp"
#include <imgui/imgui.h>

NAMESPACE_FISHENGINE_BEGIN

class RenderSettings : public Script
{
public:
    InjectClassName(RenderSettings);
    
    bool m_isWireFrameMode      = false;
    bool m_useGammaCorrection   = true;
    
    virtual void OnEditorGUI() override {
        if (ImGui::Checkbox("Wire Frame", &m_isWireFrameMode)) {
            RenderSystem::setWireFrameMode(m_isWireFrameMode);
        }
        if (ImGui::Checkbox("Gamma Correction", &m_useGammaCorrection)) {
            RenderSystem::setGammaCorrection(m_useGammaCorrection);
        }
    }
};

NAMESPACE_FISHENGINE_END

#endif // RenderSettings_hpp
