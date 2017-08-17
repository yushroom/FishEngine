#include <FishEngine/Internal/FMODPlugin.hpp>

#include <fmod.hpp>
#include <fmod_errors.h>

#include <FishEngine/Debug.hpp>
#include <FishEngine/StringFormat.hpp>
#include <FishEngine/AudioClip.hpp>

using namespace FishEngine;

void FMODPlugin::Update()
{
	GetInstance().m_system->update();
}

FMODPlugin::FMODPlugin()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_system);
	CheckFMODError(result);

	result = m_system->init(512, FMOD_INIT_NORMAL, 0);
	CheckFMODError(result);
}


void FMODPlugin::Cleanup()
{
	std::vector<AudioClipPtr> clips;
	Object::FindObjectsOfType<AudioClip>(clips);
	for (auto & clip : clips)
	{
		clip->Cleanup();
	}
	clips.clear();
	auto result = m_system->close();
	CheckFMODError(result);
	result = m_system->release();
	CheckFMODError(result);
	m_system = nullptr;
	LogWarning("Cleanup");
}

