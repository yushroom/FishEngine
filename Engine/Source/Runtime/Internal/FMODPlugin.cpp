#include "FMODPlugin.hpp"

#include "../AudioClip.hpp"

using namespace FishEngine;

void FMODPlugin::Cleanup()
{
	std::vector<AudioClipPtr> clips;
	Object::FindObjectsOfType<AudioClip>(clips);
	for (auto & clip : clips)
	{
		clip->Cleanup();
	}
	m_system->release();
}

