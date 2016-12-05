#include "QualitySettings.hpp"

namespace FishEngine
{
     ShadowResolution QualitySettings::m_shadowResolution = ShadowResolution::High;

     int QualitySettings::m_shadowCascades = 4;

     float QualitySettings::m_shadowDistance = 150.0f;

     float QualitySettings::m_shadowNearPlaneOffset = 2.0f;

}


