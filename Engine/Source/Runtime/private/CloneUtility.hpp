#pragma once

#include "../ReflectClass.hpp"
#include <type_traits>

namespace FishEngine
{
	Meta(NonSerializable)
	class CloneUtility
	{
	public:

		CloneUtility() = delete;

		template<class T>
		static void Clone(T const & source, T & dest);

		template<class T, std::enable_if_t<std::is_trivially_copyable<T>::type, int> = 0>
		static void Clone(T const & source, T & dest)
		{
			dest = source;
		}
	};
}