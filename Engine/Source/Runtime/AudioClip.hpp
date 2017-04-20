#pragma once

#include "Object.hpp"
#include "AudioClipLoadType.hpp"
#include "AudioDataLoadState.hpp"

namespace FishEngine
{
	class AudioClip : public Object
	{
	public:
		InjectClassName(AudioClip);

		// The length of the audio clip in seconds. (Read Only)
		float length() const;

		// The length of the audio clip in samples. (Read Only)
		int samples() const;

		// The number of channels in the audio clip. (Read Only)
		int channels() const;

		// The sample frequency of the clip in Hertz. (Read Only)
		int frequency() const;

		// Returns true if the AudioClip is ready to play (read-only).
		//bool isReadyToPlay() const;
		
		AudioClipLoadType loadType() const;
		
	protected:
		float	m_length	= 0;
		float	m_samples	= 0;
		int		m_channels	= 2;
		int		m_frequency = 44100;
		AudioClipLoadType	m_loadType;
		AudioDataLoadState	m_loadState;
		bool	m_loadInBackground;
	};
}
