#pragma once

namespace FishEngine
{
	class AnimationClip;

	// Information about clip being played and blended by the Animator.
	struct AnimatorClipInfo
	{
		// Returns the animation clip played by the Animator.
		std::shared_ptr<AnimationClip> clip;

		// Returns the blending weight used by the Animator to blend this clip.
		float weight;
	};
}