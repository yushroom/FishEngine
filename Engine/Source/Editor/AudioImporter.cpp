#include "AudioImporter.hpp"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <Debug.hpp>
#include <AudioClip.hpp>
#include <Internal/FMODPlugin.hpp>

using namespace FishEngine;
using namespace FishEditor;


AudioClipPtr AudioImporter::Import(Path const & path)
{
	FMOD::Sound * sound;
	auto result = FMODPlugin::instance()->system()->createSound(path.string().c_str(), FMOD_CREATESAMPLE, nullptr, &sound);
	CheckFMODError(result);

	FMOD_SOUND_FORMAT format;
	int numChannels = 0;
	int numBits = 0;
	sound->getFormat(nullptr, &format, &numChannels, &numBits);

	float frequency = 0.0f;
	sound->getDefaults(&frequency, nullptr);
	
	unsigned int length_ms;
	sound->getLength(&length_ms, FMOD_TIMEUNIT_MS);
	unsigned int length_samples;
	sound->getLength(&length_samples, FMOD_TIMEUNIT_PCM);

	auto clip = std::make_shared<AudioClip>();
	clip->m_frequency = static_cast<int>(frequency);
	clip->m_channels = numChannels;
	clip->m_length = length_ms / 1000.0f;
	clip->m_samples = length_samples;
	clip->m_fmodSound = sound;

	return clip;
}
