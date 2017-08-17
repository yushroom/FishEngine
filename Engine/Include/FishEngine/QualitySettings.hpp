#pragma once

#include "FishEngine.hpp"
#include "ReflectClass.hpp"
#include "Object.hpp"

namespace FishEngine
{
	enum class ShadowResolution
	{
		Low,        // Low shadow map resolution.
		Medium,     // Medium shadow map resolution.
		High,       // High shadow map resolution.
		VeryHigh,   // Very high shadow map resolution.
	};

	class Meta(NonSerializable) QualitySettings final : public FishEngine::Object
	{
	public:
		QualitySettings() = delete;

		FE_EXPORT static void setShadowDistance(float shadowDistance)
		{
			s_shadowDistance = shadowDistance;
		}

		FE_EXPORT static float shadowDistance()
		{
			return s_shadowDistance;
		}

		static uint32_t CalculateShadowMapSize();

	private:
		// Shadows	This determines which type of shadows should be used.The available options are Hard and Soft Shadows, Hard Shadows Only and Disable Shadows.

		// Shadow resolution	Shadows can be rendered at several different resolutions : Low, Medium, High and Very High.The higher the resolution, the greater the processing overhead.
		static ShadowResolution m_shadowResolution;

		// Shadow Projection	There are two different methods for projecting shadows from a directional light.Close Fit renders higher resolution shadows but they can sometimes wobble slightly if the camera moves.Stable Fit renders lower resolution shadows but they don't wobble with camera movements.


		// Shadow Cascades	The number of shadow cascades can be set to zero, two or four.A higher number of cascades gives better quality but at the expense of processing overhead(see Directional Light Shadows for further details).
		static int m_shadowCascades;

		// Shadow Distance	The maximum distance from camera at which shadows will be visible.Shadows that fall beyond this distance will not be rendered.
		static float s_shadowDistance;

		// Shadow Near Plane Offset	Offset shadow near plane to account for large triangles being distorted by shadow pancaking.
		static float m_shadowNearPlaneOffset;
	};
}
