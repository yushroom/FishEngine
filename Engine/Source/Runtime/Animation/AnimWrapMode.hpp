#pragma once

namespace FishEngine
{
	/** Determines how an animation clip behaves when it reaches the end. */
	enum class AnimWrapMode
	{
		Loop, /**< Loop around to the beginning/end when the last/first frame is reached. */
		Clamp /**< Clamp to end/beginning, keeping the last/first frame active. */
	};
}