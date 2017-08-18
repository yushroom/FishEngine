#include <FishEngine/AudioSystem.hpp>
#include <FishEngine/Internal/FMODPlugin.hpp>

void FishEngine::AudioSystem::Update()
{
	FMODPlugin::GetInstance().Update();
}

void FishEngine::AudioSystem::Stop()
{
	FMODPlugin::GetInstance().Stop();
}
