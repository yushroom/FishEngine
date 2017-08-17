#pragma once

#include "ReflectClass.hpp"

namespace FishEngine
{
	class FE_EXPORT AudioSystem final
	{
	public:
		//static AudioSystem* instance()
		//{
		//	static AudioSystem ins;
		//	return &ins;
		//}

		//void Update();

		static void Init();
		static void Update();

	private:
		AudioSystem();
		~AudioSystem();

		// noncopyable
		AudioSystem(const AudioSystem &) = delete;
		AudioSystem(AudioSystem &&) = delete;
		AudioSystem & operator=(AudioSystem const &) = delete;
	};
}