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
    bool IsDerivedFrom(const std::string& className, const std::string& baseClassName);

    template<typename BaseClass>
    bool IsSubClassOf(const std::string& className)
    {
        return IsDerivedFrom(className, T::StaticClassName());
    }

    static bool IsScript(const std::string& className)
    {
        return IsDerivedFrom(className, "Script");
    }

#if 0
    class Archive
    {
        template<typename T>
        friend operator<<(Archive& archive, const T& value);

        template<typename T>
        friend operator>>(Archive& archive, T& value);
    };
    
    template<typename T>
    static void Serialize(Archive& archive, const T& value);
    
    template<typename T>
    static void Serialize(Archive& archive, const std::shared_ptr<T>& value);

    template<typename T>
    static void Deserialize(Archive& archive, T& value);

    template<typename T>
    static void Deserialize(Archive& archive, std::shared_ptr<T>& value);
#endif
}
