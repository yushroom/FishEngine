#pragma once

#include "Object.hpp"
#include "AudioClipLoadType.hpp"
#include "AudioDataLoadState.hpp"

namespace FMOD
{
	class Sound;
}

namespace FishEngine
{
	class FE_EXPORT AudioClip final: public Object
	{
	public:
		InjectClassName(AudioClip);

		AudioClip();
		~AudioClip();

		void Cleanup();

		// The length of the audio clip in seconds. (Read Only)
		float length() const { return m_length; }

		// The length of the audio clip in samples. (Read Only)
		int samples() const { return m_samples; }

		// The number of channels in the audio clip. (Read Only)
		int channels() const { return m_channels; }

		// The sample frequency of the clip in Hertz. (Read Only)
		int frequency() const { return m_frequency; }

		// Returns true if the AudioClip is ready to play (read-only).
		//bool isReadyToPlay() const;
		

		// The load type of the clip (read-only).
		AudioClipLoadType loadType() const { return m_loadType; }
		
		// Preloads audio data of the clip when the clip asset is loaded.
		// When this flag is off, scripts have to call AudioClip.LoadAudioData() to load the data before the clip can be played.
		// Properties like length, channels and format are available before the audio data has been loaded.
		bool preloadAudioData() const;

		// Returns the current load state of the audio data associated with an AudioClip.
		AudioDataLoadState loadState() const;

		// Corresponding to the "Load In Background" flag in the inspector,
		// when this flag is set, the loading will happen delayed without
		// blocking the main thread.
		bool loadInBackground() const;

		/// <summary>
		///   <para>Loads the audio data of a clip. Clips that have "Preload Audio Data" set will load the audio data automatically.</para>
		/// </summary>
		/// <returns>
		///   <para>Returns true if loading succeeded.</para>
		/// </returns>
		bool LoadAudioData();


		/**
		 * Unloads the audio data associated with the clip. This works only for AudioClips that are based on actual sound file assets.
		 *
		 * @return bool Returns false if unloading failed.
		 */
		 bool UnloadAudioData();

	private:
		friend class FishEditor::AudioImporter;
		friend class AudioSource;

		// The length of the audio clip in seconds.
		float	m_length	= 0;

		// The length of the audio clip in samples.
		int		m_samples	= 0;

		// The number of channels in the audio clip.
		int		m_channels	= 2;

		// The sample frequency of the clip in Hertz.
		int		m_frequency = 44100;

		bool	m_preloadAudioData = true;

		AudioClipLoadType	m_loadType = AudioClipLoadType::DecompressOnLoad;
		
		AudioDataLoadState	m_loadState = AudioDataLoadState::Unloaded;
		
		bool	m_loadInBackground = false;

		Meta(NonSerializable)
		FMOD::Sound * m_fmodSound = nullptr;
	};
}
