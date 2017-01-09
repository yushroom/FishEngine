#pragma once

# if defined(__REFLECTION_PARSER__)
#   define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
# else
#   define Meta(...)
# endif

#include "FishEngine.hpp"
#include "Attribute.hpp"

namespace FishEngine
{
    bool IsDerivedFrom(const std::string& derivedClassName, const std::string& baseClassName);

    template<typename BaseClass>
    bool IsSubClassOf(const std::string& derivedClassName)
    {
        return IsDerivedFrom(derivedClassName, BaseClass::StaticClassName());
    }

    inline bool IsScript(const std::string& derivedClassName)
    {
        return IsDerivedFrom(derivedClassName, "Script");
    }
}
