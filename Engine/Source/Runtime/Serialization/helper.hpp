#pragma once
#include "ReflectClass.hpp"

namespace FishEngine
{
	template <class T>
	class Meta(NonSerializable) SizeTag
	{
	private:
		// Store a reference if passed an lvalue reference, otherwise
		// make a copy of the data
		using Type = std::conditional_t<std::is_lvalue_reference<T>::value, T, std::decay_t<T>>;

		SizeTag & operator=(SizeTag const &) = delete;

	public:
		SizeTag(T && sz) : size(std::forward<T>(sz)) {}

		Type size;
	};

	template <class T>
	inline SizeTag<T> make_size_tag(T && sz)
	{
		return{ std::forward<T>(sz) };
	}

	template<class Base>
	struct BaseClassWrapper
	{
		template<class Derived>
		BaseClassWrapper(Derived const & derived) :
			base_ref(static_cast<Base const &>(derived))
		{
			static_assert(std::is_base_of<Base, Derived>::value, "Can only use base_class on a valid base class");
		}

		Base const & base_ref;
	};
}