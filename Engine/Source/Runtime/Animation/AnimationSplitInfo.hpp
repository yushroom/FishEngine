#pragma once

#include "../ReflectClass.hpp"
#include <cstdint>

namespace FishEngine
{
	/** Information about how to split an AnimationClip into multiple separate clips. */
	struct FE_EXPORT AnimationSplitInfo
	{
		AnimationSplitInfo() { }

		std::string name;
		uint32_t startFrame = 0;
		uint32_t endFrame = 0;
		bool isAdditive = false;
	};
}