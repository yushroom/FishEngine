#include "./details/helpers.hpp"
#include "./types/string.hpp"

namespace FishEngine::Serialization
{
	// ######################################################################
	//! Convenience function to create binary data for both const and non const pointers
	/*! @param data Pointer to beginning of the data
	@param size The size in bytes of the data
	@relates BinaryData
	@ingroup Utility */
	template <class T> inline
	BinaryData<T> binary_data( T && data, size_t size )
	{
		return{ std::forward<T>(data), size };
	}

	// ######################################################################
	//! Creates a size tag from some variable.
	/*! Will normally be used to serialize size (e.g. size()) information for
	variable size containers.  If you have a variable sized container,
	the very first thing it serializes should be its size, wrapped in
	a SizeTag.

	@relates SizeTag
	@ingroup Utility */
	template <class T> inline
	SizeTag<T> make_size_tag( T && sz )
	{
		return{ std::forward<T>(sz) };
	}
}