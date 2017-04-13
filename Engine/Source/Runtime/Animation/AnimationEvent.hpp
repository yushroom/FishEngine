#pragma once

#include "AnimationClipInfo.hpp"
#include "AnimationState.hpp"

namespace FishEngine
{
	// AnimationEvent lets you call a script function similar to SendMessage as part of playing back an animation.
	class AnimationEvent
	{
	public:
		//The animation state that fired this event (Read Only).
		AnimationState animationState;

		// The animator clip info related to this event (Read Only).
		AnimatorClipInfo animatorClipInfo;
	};
}