#pragma once

namespace FishEngine
{
	// Used by Animation.Play function.
	enum class AnimationBlendMode
	{
		Blend,          // Animations will be blended.
		Additive,       // Animations will be added.
	};
}