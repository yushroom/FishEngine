#include <string>
#include "../details/traits.hpp"

namespace FishEngine::Serialization
{
	//! Serialization for basic_string types, if binary data is supported
	template<class Archive, class CharT, class Traits, class Alloc> inline
		typename std::enable_if<traits::is_output_serializable<BinaryData<CharT>, Archive>::value, void>::type
		CEREAL_SAVE_FUNCTION_NAME(Archive & ar, std::basic_string<CharT, Traits, Alloc> const & str)
	{
		// Save number of chars + the data
		ar(make_size_tag(static_cast<size_type>(str.size())));
		ar(binary_data(str.data(), str.size() * sizeof(CharT)));
	}

	//! Serialization for basic_string types, if binary data is supported
	template<class Archive, class CharT, class Traits, class Alloc> inline
		typename std::enable_if<traits::is_input_serializable<BinaryData<CharT>, Archive>::value, void>::type
		CEREAL_LOAD_FUNCTION_NAME(Archive & ar, std::basic_string<CharT, Traits, Alloc> & str)
	{
		size_type size;
		ar(make_size_tag(size));
		str.resize(static_cast<std::size_t>(size));
		ar(binary_data(const_cast<CharT *>(str.data()), static_cast<std::size_t>(size) * sizeof(CharT)));
	}
}