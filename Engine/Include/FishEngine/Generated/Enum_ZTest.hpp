#pragma once

#include <ReflectEnum.hpp>
#include <ShaderProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ZTest
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ZTest>() { return 7; }

// string array
static const char* ZTestStrings[] =
{
    "Less",
	"Greater",
	"LEqual",
	"GEqual",
	"Equal",
	"NotEqual",
	"Always"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ZTest>()
{
    return ZTestStrings;
}

// index to enum
template<>
inline FishEngine::ZTest ToEnum<FishEngine::ZTest>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ZTest::Less; break;
	case 1: return FishEngine::ZTest::Greater; break;
	case 2: return FishEngine::ZTest::LEqual; break;
	case 3: return FishEngine::ZTest::GEqual; break;
	case 4: return FishEngine::ZTest::Equal; break;
	case 5: return FishEngine::ZTest::NotEqual; break;
	case 6: return FishEngine::ZTest::Always; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ZTest>(FishEngine::ZTest e)
{
    switch (e) {
    case FishEngine::ZTest::Less: return 0; break;
	case FishEngine::ZTest::Greater: return 1; break;
	case FishEngine::ZTest::LEqual: return 2; break;
	case FishEngine::ZTest::GEqual: return 3; break;
	case FishEngine::ZTest::Equal: return 4; break;
	case FishEngine::ZTest::NotEqual: return 5; break;
	case FishEngine::ZTest::Always: return 6; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ZTest ToEnum<FishEngine::ZTest>(const std::string& s)
{
    if (s == "Less") return FishEngine::ZTest::Less;
	if (s == "Greater") return FishEngine::ZTest::Greater;
	if (s == "LEqual") return FishEngine::ZTest::LEqual;
	if (s == "GEqual") return FishEngine::ZTest::GEqual;
	if (s == "Equal") return FishEngine::ZTest::Equal;
	if (s == "NotEqual") return FishEngine::ZTest::NotEqual;
	if (s == "Always") return FishEngine::ZTest::Always;
	
    abort();
}


} // namespace FishEngine
