#pragma once

#include "../helper.hpp"

namespace FishEngine
{
	template<class Archive, class T, class B>
	static void Save ( Archive & archive, std::map<T, B> const & v )
	{
		archive << make_size_tag(v.size());
		for (auto& p : v)
		{
			archive << p.first << p.second;
		}
	}

	template<class Archive, class T, class B>
	static void Load ( Archive & archive, std::map<T, B> & v )
	{
		std::size_t size = 0;
		archive >> make_size_tag(size);
		v.clear();
		auto hint = v.begin();
		for (size_t i = 0; i < size; ++i)
		{
			T key;
			B value;
			archive >> key >> value;
			hint = v.emplace_hint(hint, std::move(key), std::move(value));
		}
	}
}
