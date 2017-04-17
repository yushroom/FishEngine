#pragma once

#include "AnimationClipInfo.hpp"
#include "AnimationBlendMode.hpp"
#include "WrapMode.hpp"

namespace FishEngine
{
	class AnimationClip;

	// The AnimationState gives full control over animation blending.
	class AnimationState
	{
	public:
		// Which blend mode should be used?
		AnimationBlendMode blendMode;

		// The clip that is being played by this animation state.
		std::shared_ptr<AnimationClip> clip;

		// Enables / disables the animation.
		bool enabled;

		// The length of the animation clip in seconds.
		float length;

		// The name of the animation.
		std::string name;

		// The normalized playback speed.
		float normalizedSpeed;

		// The normalized time of the animation.
		float normalizedTime;

		// The playback speed of the animation. 1 is normal playback speed.
		float speed;

		// The current time of the animation.
		float time;

		// The weight of animation.
		float weight;

		// Wrapping mode of the animation.
		WrapMode wrapMode;
	};
}
