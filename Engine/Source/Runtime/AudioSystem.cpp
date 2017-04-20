#include "AudioSystem.hpp"

#include "Internal/FMODPlugin.hpp"

void FishEngine::AudioSystem::Update()
{
	FMODPlugin::instance()->Update();
}

