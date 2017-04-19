#pragma once

#include "Object.hpp"

namespace FishEngine
{
	class AudioClip : public Object
	{
	public:

		// The length of the audio clip in seconds. (Read Only)
		float length() const;

		// The length of the audio clip in samples. (Read Only)
		int samples() const;


		// The number of channels in the audio clip. (Read Only)
		int channels() const;

		// The sample frequency of the clip in Hertz. (Read Only)
		int frequency() const;

		// Returns true if the AudioClip is ready to play (read-only).
		bool isReadyToPlay() const;

	};
}