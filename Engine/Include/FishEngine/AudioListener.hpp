#pragma once

#include "Behaviour.hpp"
#include "AudioVelocityUpdateMode.hpp"
#include "Mathf.hpp"

namespace FishEngine
{
	class FE_EXPORT AudioListener : public Behaviour
	{
	public:
		DefineComponent(AudioListener);

		// Controls the game sound volume (0.0 to 1.0).
		float volume() const { return m_volume; }
		void setVolume(float value) { m_volume = Mathf::Clamp01( value ); }

		bool pause() const { return m_pause; }

		AudioVelocityUpdateMode velocityUpdateMode() const { return m_velocityUpdateMode; }

	private:
		// Controls the game sound volume (0.0 to 1.0).
		float m_volume = 1.0f;

		// The paused state of the audio system.
		bool m_pause = false;

		// This lets you set whether the Audio Listener should be updated in the fixed or dynamic update.
		AudioVelocityUpdateMode m_velocityUpdateMode;
	};
}