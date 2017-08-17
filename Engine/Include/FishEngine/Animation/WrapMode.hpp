#pragma once

namespace FishEngine
{
	// Determines how time is treated outside of the keyframed range of an AnimationClip or AnimationCurve.
	enum class WrapMode
	{
		Once,           // When time reaches the end of the animation clip, the clip will automatically stop playing and time will be reset to beginning of the clip.
		Loop,           // When time reaches the end of the animation clip, time will continue at the beginning.
		PingPong,       // When time reaches the end of the animation clip, time will ping pong back between beginning and end.
		Default,        // Reads the default repeat mode set higher up.
		ClampForever,   // Plays back the animation. When it reaches the end, it will keep playing the last frame and never stop playing.
	};
}