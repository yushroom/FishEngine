#pragma once

#include "Behaviour.hpp"

namespace FishEngine
{
	class FE_EXPORT AudioSource final: public Behaviour
	{
	public:
		DefineComponent(AudioSource);
		
		float volume() const { return m_volume; }

		float pitch() const { return m_pitch; }

		float time() const { return m_time; }

		int timeSamples() const { return m_timeSamples; }

		AudioClipPtr clip() const { return m_clip; }
		void setClip(AudioClipPtr clip) { m_clip = clip; }

		// AudioMixerGroup outputAudioMixerGroup

		bool isPlaying() const;

		bool isVirtual() const;

		// Is the audio clip looping?
		bool loop() const { return m_loop; }
		void setLoop(bool value) { m_loop = value; }

		// If set to true, the audio source will automatically start playing on awake.
		bool playOnAwake() const { return m_playOnAwake; }
		void setPlayOnAwake(bool value) { m_playOnAwake = value;  }

		void Play(uint32_t delay = 0);
		void PlayDelayed(float delay);
		void PlayScheduled(double time);
		void SetScheduledEndTime(double time);
		void Stop();
		void Pause();
		void UnPause();
		void PlayOneShot(AudioClipPtr clip, float volumeScale = 1.0f);
		static void PlayClipAtPoint(AudioClipPtr clip, Vector3 const & position, float volume = 1.0f);
		//void SetCustomCurve

		virtual void Start() override;

	private:
		friend class FishEditor::Inspector;

		// The volume of the audio source (0.0 to 1.0).
		float m_volume = 1.0f;

		// The pitch of the audio source.
		float m_pitch = 1.0f;

		// Playback position in seconds.
		float m_time = 0;

		// Playback position in PCM samples.
		int m_timeSamples = 0;

		// The default AudioClip to play.
		AudioClipPtr m_clip = nullptr;

		// The target group to which the AudioSource should route its signal.
		// AudioMixerGroup outputAudioMixerGroup;

		// Is the clip playing right now (Read Only)?
		bool m_isPlaying = false;

		// True if all sounds played by the AudioSource (main sound started by Play() or playOnAwake as well as one-shots) are culled by the audio system.
		bool m_isVirtual;

		// Is the audio clip looping?
		bool m_loop = false;

		bool m_ignoreListenerVolume = false;
		bool m_ignoreListenerPause = false;

		bool m_playOnAwake = true;

		//AudioVelocityUpdateMode velocityUpdateMode
		float m_panStereo = 0;

		float m_spatialBlend = 0;
		bool m_spatialize;
		bool m_spatializePostEffects;

		float m_reverbZoneMix;
		bool m_bypassEffects = false;
		bool m_bypassListenerEffects = false;
		bool m_bypassReverbZones = false;
		float m_dopplerLevel = 1;
		float m_spread = 0;
		int m_priority = 128;
		bool m_mute = false;
		float m_minDistance = 1.0f;
		float m_maxDistance = 500.0f;
		//AudioRolloffMode m_rolloffMode
	};
}