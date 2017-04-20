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
		m_fmodSound->release();
		m_fmodSound = nullptr;
	}
}
