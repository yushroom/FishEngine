#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

#include "../Debug.hpp"
#include "../StringFormat.hpp"

void static CheckFMODError(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		LogError(FishEngine::Format("FMOD error! (%1%) %2%", FMOD_ErrorString(result)));
		abort();
	}
}

class FE_EXPORT FMODPlugin
{
public:
	static FMODPlugin * instance()
	{
		static FMODPlugin plugin;
		return &plugin;
	}

	static void Update()
	{
		instance()->m_system->update();
	}

	FMOD::System * system() const
	{
		return m_system;
	}

private:

	FMODPlugin()
	{
		FMOD_RESULT result;
		result = FMOD::System_Create(&m_system);
		CheckFMODError(result);

		result = m_system->init(512, FMOD_INIT_NORMAL, 0);
	}

	~FMODPlugin()
	{
		Cleanup();
	}

	void Cleanup();

	// noncopyable
	FMODPlugin(FMODPlugin const &) = delete;
	FMODPlugin& operator=(FMODPlugin const &) = delete;

	FMOD::System * m_system = nullptr;
};