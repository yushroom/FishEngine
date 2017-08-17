#pragma once

//#include <boost/preprocessor/cat.hpp>
//#include <boost/preprocessor/stringize.hpp>

//#define QUOTEM(M) #M

//#define EnumHeader(M) BOOST_PP_STRINGIZE(BOOST_PP_CAT(generate/Enum_, M))) ## ".hpp"
//#define EnumHeader(T) QUOTEM(generate/##Enum_##T##.hpp)

namespace FishEngine
{
	template<typename T>
	inline T ToEnum(const std::string& s);

	template<typename T>
	inline T ToEnum(const int index);

	template<typename T>
	inline int EnumToIndex(T e);

	template<typename T>
	inline constexpr const char** EnumToCStringArray();

	template<typename T>
	inline const char* EnumToString(T e)
	{
		return EnumToCStringArray<T>()[EnumToIndex<T>(e)];
	}

	template<typename T>
	constexpr int EnumCount();

}