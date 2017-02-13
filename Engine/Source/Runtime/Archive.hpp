#pragma once

#include <type_traits>
#include <ReflectClass.hpp>

namespace FishEngine
{	
	template <class T>
	constexpr int ArchiveID();
	
	class Meta(NonSerializable) InputArchive
	{
	public:
		
		virtual int ArchiveID() = 0;
	};
	
	
	class Meta(NonSerializable) OutputArchive
	{
	public:
		
		virtual int ArchiveID() = 0;
	};
}
