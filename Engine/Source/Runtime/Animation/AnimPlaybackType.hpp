#pragma once

namespace FishEngine
{
	/** Type of playback for animation clips. */
	enum class AnimPlaybackType
	{
		/** Play back the animation normally by advancing time. */
		Normal,
		/** Sample only a single frame from the animation. */
		Sampled,
		/** Do not play the animation. */
		None
	};
}