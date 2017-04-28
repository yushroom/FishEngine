#include "AudioSource.hpp"

#include "AudioClip.hpp"
#include "AudioSystem.hpp"
#include "Internal/FMODPlugin.hpp"
#include "Transform.hpp"

void FishEngine::AudioSource::PlayClipAtPoint(AudioClipPtr clip, Vector3 const & position, float volume /*= 1.0f*/)
{
	FMODPlugin::GetInstance().system()->playSound(clip->m_fmodSound, 0, false, nullptr);
}

void FishEngine::AudioSource::Start()
{
	if (m_playOnAwake)
	{
		PlayClipAtPoint(m_clip, transform()->position(), m_volume);
	}
}
