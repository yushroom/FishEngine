#include "AudioClip.hpp"
#include "Internal/FMODPlugin.hpp"

FishEngine::AudioClip::AudioClip()
{

}

FishEngine::AudioClip::~AudioClip()
{
	Cleanup();
}

void FishEngine::AudioClip::Cleanup()
{
	if (m_fmodSound != nullptr)
	{
		FMOD_RESULT result = m_fmodSound->release();
		CheckFMODError(result);
		m_fmodSound = nullptr;
	}
}
