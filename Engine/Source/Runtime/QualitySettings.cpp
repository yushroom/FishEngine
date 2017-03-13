#include "QualitySettings.hpp"
#include "Mathf.hpp"
#include "Screen.hpp"

namespace FishEngine
{

	uint32_t QualitySettings::CalculateShadowMapSize()
	{
		// Unity - Manual: How the Size of a Shadow Map is Calculated;
		// https://docs.unity3d.com/Manual/LightPerformance.html
		int pixel_size = std::max<int>(Screen::height(), Screen::width());
		uint32_t shadow_map_size = Mathf::NextPowerOfTwo(static_cast<uint32_t>(pixel_size * 1.9f));
		if (shadow_map_size > 4096)
			shadow_map_size = 4096;

		// at most 2048 * 2048 * 4 * 4 = 2^6 MByte = 64MB
		// at most 4096 * 4096 * 4(4 splits csm) * 4(32bits depth) = 2^8 MByte = 256MB
		return shadow_map_size;
	}

	ShadowResolution QualitySettings::m_shadowResolution = ShadowResolution::High;

	int QualitySettings::m_shadowCascades = 4;

	float QualitySettings::m_shadowDistance = 150.0f;

	float QualitySettings::m_shadowNearPlaneOffset = 2.0f;

}


