#pragma once

#include "Behaviour.hpp"

namespace FishEngine
{
	class AudioSource : public Behaviour
	{
	public:
		
		float volume();

		float pitch();

		float time();

		int timeSamples();

		AudioClipPtr clip();

		// AudioMixerGroup outputAudioMixerGroup

		bool isPlaying() const;

		bool isVirtual() const;

		bool loop();
	};
}