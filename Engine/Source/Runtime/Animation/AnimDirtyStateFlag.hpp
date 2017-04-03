#pragma once

#include "../Flags.hpp"

namespace FishEngine
{
	/** Flags that determine which portion of Animation was changed and needs to be updated. */
	enum class AnimDirtyStateFlag
	{
		Clean = 0,
		Value = 1 << 0,
		Layout = 1 << 1,
		All = 1 << 2,
		Culling = 1 << 3,
		MorphWeights = 1 << 4
	};

	typedef Flags<AnimDirtyStateFlag> AnimDirtyState;
	BS_FLAGS_OPERATORS(AnimDirtyStateFlag)
}