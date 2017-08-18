#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include "../ReflectClass.hpp"
#include "../Debug.hpp"

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
	static FMODPlugin & GetInstance()
	{
		static FMODPlugin plugin;
		return plugin;
	}

	static void Update();
	static void Stop();

	FMOD::System * system() const
	{
		return m_system;
	}

private:

	FMODPlugin();

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
