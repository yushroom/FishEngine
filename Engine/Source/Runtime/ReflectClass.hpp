#pragma once

# if defined(__REFLECTION_PARSER__)
#   define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
# else
#   define Meta(...)
# endif

namespace FishEngine
{
    class Archive;
    
    template<typename T>
    static void Serialize(Archive& archive, const T& value);
    
    template<typename T>
    static void Serialize(Archive& archive, const std::shared_ptr<T>& value);
}
