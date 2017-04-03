#pragma once

#include "AnimWrapMode.hpp"
#include <cstdint>

namespace FishEngine
{
	/** Contains information about a currently playing animation clip. */
	struct AnimationClipState
	{
		AnimationClipState() { }

		/** Layer the clip is playing on. Multiple clips can be played simulatenously on different layers. */
		uint32_t layer = 0;
		float time = 0.0f; /**< Current time the animation is playing from. */
		float speed = 1.0f; /**< Speed at which the animation is playing. */
		float weight = 1.0f; /**< Determines how much of an influence does the clip have on the final pose. */
							 /** Determines what happens to other animation clips when a new clip starts playing. */
		AnimWrapMode wrapMode = AnimWrapMode::Loop;
		/**
		* Determines should the time be advanced automatically. Certain type of animation clips don't involve playback
		* (e.g. for blending where animation weight controls the animation).
		*/
		bool stopped = false;
	};
}