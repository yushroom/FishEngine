#pragma once

#include <string>

namespace FishEngine
{
	template<class T>
	class NameValuePair
	{
	private:
		// If we get passed an array, keep the type as is, otherwise store
		// a reference if we were passed an l value reference, else copy the value
		using Type = std::conditional_t<std::is_array<std::remove_reference_t<T>>::value,
			std::remove_cv_t<T>, 
			std::conditional_t<std::is_lvalue_reference<T>::value,
				T,
				std::decay_t<T>>>;
		static_assert(!std::is_base_of<NameValuePair, T>::value, "Cannot pair a name to a NameValuePair");

		NameValuePair & operator= ( NameValuePair const & ) = delete;

	public:
		//! Constructs a new NameValuePair
		/*! @param n The name of the pair
		@param v The value to pair.  Ideally this should be an l-value reference so that
		the value can be both loaded and saved to.  If you pass an r-value reference,
		the NameValuePair will store a copy of it instead of a reference.  Thus you should
		only pass r-values in cases where this makes sense, such as the result of some
		size() call.
		@internal */
		NameValuePair(char const * n, T && v) : name(n), value(std::forward<T>(v)) {}

		char const * name;
		Type value;
	};

	// ######################################################################
	//! Creates a name value pair
	/*! @relates NameValuePair
	  @ingroup Utility */
	template <class T> inline
	NameValuePair<T> make_nvp( std::string const & name, T && value )
	{
		return { name.c_str(), std::forward<T>(value) };
	}

	//! Creates a name value pair
	/*! @relates NameValuePair
	  @ingroup Utility */
	template <class T> inline
	NameValuePair<T> make_nvp( const char * name, T && value )
	{
		return { name, std::forward<T>(value) };
	}

}
