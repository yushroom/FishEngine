#pragma once

namespace FishEngine
{
	template <class Archive, class T>
	inline void prologue(Archive& arhive, T const & t)
	{ }

	template <class Archive, class T>
	inline void epilogue(Archive& arhive, T const & t)
	{ }
}

#include "Serialization/archives/binary.hpp"
#include "Serialization/archives/text.hpp"