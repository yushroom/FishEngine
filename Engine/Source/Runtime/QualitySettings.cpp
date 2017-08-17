#include <FishEngine/QualitySettings.hpp>
#include <FishEngine/Mathf.hpp>
#include <FishEngine/Screen.hpp>

namespace FishEngine
{

	uint32_t QualitySettings::CalculateShadowMapSize()
	{
#if 0
		// Unity - Manual: How the Size of a Shadow Map is Calculated;
		// https://docs.unity3d.com/Manual/LightPerformance.html
		int pixel_size = std::max<int>(Screen::height(), Screen::width());
		uint32_t shadow_map_size = Mathf::NextPowerOfTwo(static_cast<uint32_t>(pixel_size * 1.9f));
		if (shadow_map_size > 4096)
			shadow_map_size = 4096;
		
		return shadow_map_size;
#else
		// It seems that unity gets all csm into one texture(sampler2d rather than sampler2dArray),
		// so half of the size is what we need.
		int pixel_size = std::max<int>(Screen::height(), Screen::width());
		uint32_t shadow_map_size = Mathf::NextPowerOfTwo(static_cast<uint32_t>(pixel_size * 0.95f));
		if (shadow_map_size > 2048)	// at most 2048 * 2048 * 4 * 4 = 2^6 MByte = 64MB
			shadow_map_size = 2048;
		return shadow_map_size;
#endif
	}

	ShadowResolution QualitySettings::m_shadowResolution = ShadowResolution::High;

	int QualitySettings::m_shadowCascades = 4;

	float QualitySettings::s_shadowDistance = 150.0f;

	float QualitySettings::m_shadowNearPlaneOffset = 2.0f;

}


