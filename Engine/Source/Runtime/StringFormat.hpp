#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "FishEngine.hpp"

namespace FishEngine
{
	inline std::string ToString(int value)
	{
		return boost::lexical_cast<std::string>(value);
	}

	template<class TFirst>
	void Format(boost::format& fmt, TFirst&& first)
	{
		fmt % first;
	}

	template<class TFirst, class... TOther>
	void Format(boost::format& fmt, TFirst&& first, TOther&&... other)
	{
		fmt % first;
		Format(fmt, other...);
	}

	template<class TFirst, class... TOther>
	std::string Format(const char* format, TFirst&& first, TOther&&... other)
	{
		boost::format fmt(format);
		Format(fmt, first, other...);
		return fmt.str();
	}
}